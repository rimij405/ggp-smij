#pragma once
#include "MAT_PBRTexture.h"
class MAT_Water :
	public MAT_PBRTexture
{
private:
	float* translate; //value that counts from 0.0 to 1.0, keeps track of the scrolling of the normal map
public:
	MAT_Water::MAT_Water(SimpleVertexShader* vertexShader, SimplePixelShader* pixelShader,
		float shininess, DirectX::XMFLOAT2 uvScale,
		ID3D11ShaderResourceView* albedo, ID3D11ShaderResourceView* normals,
		ID3D11ShaderResourceView* roughness, ID3D11ShaderResourceView* metal,
		ID3D11SamplerState* sampler, float* translate);
	~MAT_Water();

	void PrepareMaterialObject(GameObject* entityObj);
};

