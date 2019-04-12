#define _USE_MATH_DEFINES

#include "Swimmer.h"
#include "ExtendedMath.h"
#include <cmath>

//Buoyancy consts
#define MASS 0.5f
#define GRAVITY 9.81f
#define FLUID_DENSITY 2.0f
#define DRAG_COEFF 1.05f
#define AIR_DENSITY 0.1225f
#define SURFACE_Y 0

using namespace DirectX;

//Snake follow logic from:
//https://github.com/rimij405/ggp-smij/blob/Unity-Prototype/Prototype/Boat-Snake-Prototype/Assets/Scripts/BoatFollower.cs

Swimmer::Swimmer(Mesh* mesh, Material* material, std::string name)
	: Entity(mesh, material, name)
{
	//Create buffers
	bufferLength = (int)ceil(lagSeconds * MAX_FPS);
	positionBuffer = new XMFLOAT3[bufferLength];
	timeBuffer = new float[bufferLength];

	//Set default vals
	swmrState = SwimmerState::Entering;
	this->leader = nullptr;
	positionBuffer[0] = positionBuffer[1] = DirectX::XMFLOAT3(0, 0, 0);
	timeBuffer[0] = timeBuffer[1] = timer = 0;
	hitTimer = 0;

	//Buoyancy vals
	velocity = 0;
	acceleration = 0;
	sinAmnt = 0;
	gravityMult = 0;

	oldestIndex = 0;
	newestIndex = 1;
}

Swimmer::~Swimmer()
{
	//Delete buffers
	delete[] positionBuffer;
	delete[] timeBuffer;
}

//Update the swimmer every frame
void Swimmer::Update(float deltaTime)
{
	//Run the current state's behavior
	switch (swmrState)
	{
		case SwimmerState::Entering:
			Enter(deltaTime);
			break;

		case SwimmerState::Floating:
			Float(deltaTime);
			break;

		case SwimmerState::Joining:
			Join(deltaTime);
			break;

		case SwimmerState::Following:
			Follow(deltaTime);
			break;

		case SwimmerState::Still:
			if (leader->GetName() != "player" && ((Swimmer*)leader)->CheckHit())
				swmrState = SwimmerState::Hitting;
			break;

		case SwimmerState::Hitting:
			Hit(deltaTime);
			break;

		case SwimmerState::Nothing:
			break;
	}	
}

// --------------------------------------------------------
// Run this swimmer's entering behaviour
//---------------------------------------------------------
void Swimmer::Enter(float deltaTime)
{
	ApplyBuoyancy(deltaTime);
	if (GetPosition().y > SURFACE_Y)
		swmrState = SwimmerState::Floating;
}

void Swimmer::ApplyCos(float deltaTime)
{
	XMFLOAT3 position = GetPosition();
	sinAmnt += deltaTime;
	float y = (-cos(3 * sinAmnt) / sinAmnt) * 2;
	SetPosition(position.x, y, position.z);
}

// --------------------------------------------------------
// Apply buoyancy to the swimmer
//---------------------------------------------------------
void Swimmer::ApplyBuoyancy(float deltaTime)
{
	//printf("%9.6f\n", GetPosition().y);
	Collider* col = GetCollider();
	XMFLOAT3 position = GetPosition();
	XMFLOAT3 halves = col->GetHalfSize();

	//Thanks Khan once again
	//https://www.khanacademy.org/science/physics/fluids/buoyant-force-and-archimedes-principle/a/buoyant-force-and-archimedes-principle-article
	//Calculate displaced volume and buoyancy
	float hTop = position.y + halves.y > SURFACE_Y ? SURFACE_Y : position.y + halves.y;
	float hBot = position.y - halves.y > SURFACE_Y ? SURFACE_Y : position.y - halves.y;
	float area = (2 * halves.x) * (2 * halves.z);
	float watDisplaced = area * -(hBot - hTop);
	float buoyancy = FLUID_DENSITY * GRAVITY * watDisplaced;

	//https://www.grc.nasa.gov/WWW/K-12/airplane/falling.html
	float drag = 0;
	if(position.y > SURFACE_Y)
	{
		//Calculate air drag if we are above the water
		drag = DRAG_COEFF * AIR_DENSITY * ((velocity * velocity * area) / 2);
	}
	else
	{
		//Calculate fluid drag if we are below the water
		drag = DRAG_COEFF * FLUID_DENSITY * ((velocity * velocity * area) / 2);
	}

	//Apply bouyancy and gravity to acceleration
	acceleration += buoyancy / MASS;
	acceleration += -GRAVITY;

	//Add acceleration to velocity
	velocity += (acceleration * deltaTime);

	//Add drag based on velocity's direction
	if(velocity < 0)
		velocity += (drag * deltaTime);
	else velocity -= (drag * deltaTime);

	//Add velocity to position
	position.y += velocity * deltaTime;

	//Reset
	acceleration = 0;

	SetPosition(position);
}

// Run this swimmer's floating behaviour
void Swimmer::Float(float deltaTime)
{
	ApplyBuoyancy(deltaTime);

	// Rotate when idle.
	Rotate(0, 5 * deltaTime, 0);
}

// Update the swimmer's buffers for snake movement
XMFLOAT3 Swimmer::GetTrailPos(float deltaTime)
{
	// Update internal timer.
	timer += deltaTime;

	// Insert newest position into our cache.
	// If the cache is full, overwrite the latest sample.
	int newIndex = (newestIndex + 1) % bufferLength;
	if (newIndex != oldestIndex)
		newestIndex = newIndex;

	positionBuffer[newestIndex] = leader->GetPosition();
	timeBuffer[newestIndex] = timer;

	// Skip ahead in the buffer to the segment containing our target time.
	float targetTime = timer - lagSeconds;
	int nextIndex;
	while (timeBuffer[nextIndex = (oldestIndex + 1) % bufferLength] < targetTime)
		oldestIndex = nextIndex;
	float span = timeBuffer[nextIndex] - timeBuffer[oldestIndex];
	float progress = 0;
	if (span > 0)
	{
		progress = (targetTime - timeBuffer[oldestIndex]) / span;
	}

	//Lerp to find the position
	XMFLOAT3 lerp;
	XMStoreFloat3(&lerp, XMVectorLerp(XMLoadFloat3(&(positionBuffer[oldestIndex])), XMLoadFloat3(&(positionBuffer[nextIndex])), progress));
	return lerp;
}

// --------------------------------------------------------
// Causes this swimmer to seek the surface y's position
// --------------------------------------------------------
void Swimmer::SeekSurfaceY()
{
	XMFLOAT3 position = GetPosition();
	if (position.y == 0)
		return;
	
	float lerp = ExtendedMath::LerpThreshhold(position.y, SURFACE_Y, 0.2f);
	position.y = lerp;
	SetPosition(position);
}

// Run this swimmer's joining behaviour
void Swimmer::Join(float deltaTime)
{
	//Seek surface
	SeekSurfaceY();

	//Seek trail
	XMFLOAT3 trailPos = GetTrailPos(deltaTime);
	float dist = ExtendedMath::DistanceFloat3(trailPos, GetPosition());
	if (dist > 0.1f)
	{
		XMFLOAT3 lerp;
		XMStoreFloat3(&lerp, XMVectorScale(XMVector3Normalize(
			XMLoadFloat3(&trailPos) - XMLoadFloat3(&GetPosition())), 5 * deltaTime)
		);
		MoveAbsolute(lerp);
	}
	//XMFLOAT3 trailPos = GetTrailPos(deltaTime);
	//float dist = ExtendedMath::DistanceFloat3(GetPosition(), trailPos);
	//if (dist > 0.25f)
	//{
	//	XMFLOAT3 lerp;
	//	XMStoreFloat3(&lerp, XMVectorLerp(
	//			XMLoadFloat3(&GetPosition()), XMLoadFloat3(&trailPos), 3 * deltaTime)
	//	);
	//	SetPosition(lerp);
	//}
	else
	{
		swmrState = SwimmerState::Following;
		printf("hit\n");
	}
}

// Run this swimmer's following behaviour
void Swimmer::Follow(float deltaTime)
{
	//Seek surface
	SeekSurfaceY();

	// Interpolate between the two samples on either side of our target time.
	SetPosition(GetTrailPos(deltaTime));
}

// Run this swimmer's hitting behaviour
void Swimmer::Hit(float deltaTime)
{
	hitTimer += deltaTime;

	XMFLOAT3 position = GetPosition();
	if (hitTimer < M_PI / 8)
	{
		//Do a little jump up
		position.y = 2 * sin(8 * hitTimer);
		SetPosition(position);
	}
	else
	{
		//Reset to 0 and effectively "kill" the swimmer
		position.y = 0;
		SetPosition(position);
		swmrState = SwimmerState::Nothing;
	}
}

// Check if the boat is in the following state
bool Swimmer::IsFollowing()
{
	return this->enabled && (swmrState == SwimmerState::Following);
}

// Set Swimmer to follow a game object.
void Swimmer::JoinTrail(Entity* newLeader)
{
	swmrState = SwimmerState::Joining;
	this->leader = newLeader;
	positionBuffer[0] = positionBuffer[1] = leader->GetPosition();
}

// Check if the swimmer is in the hitting state for the correct amount of time
bool Swimmer::CheckHit()
{
	return (swmrState == SwimmerState::Hitting) && (hitTimer > (M_PI / 8) / 4);
}

// Set Swimmer's state to a new state
void Swimmer::SetSwimmerState(SwimmerState newState)
{
	swmrState = newState;
}
