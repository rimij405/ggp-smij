#include "FXAA.h"

// Load values based off of a given preset.
void FXAA_DESC::LoadPreset(unsigned int _presetID)
{
	// Cast input preset flag as the enum value.
	FXAA_PRESET_FLAG presetID = (FXAA_PRESET_FLAG)_presetID;

	// Assign the preset value.
	this->FXAA_PRESET = presetID;

	// Call appropriate LoadPresetX method based on input preset ID.
	switch (presetID)
	{
	case FXAA_PRESET_CUSTOM:
		return;	// If 'custom' preset, early exit.
	case FXAA_PRESET_0: // DEFAULT_FXAA_PRESET
		LoadPreset0();
		break;
	case FXAA_PRESET_1:
		LoadPreset1();
		break;
	case FXAA_PRESET_2:
		LoadPreset2();
		break;
	case FXAA_PRESET_3:
		LoadPreset3();
		break;
	case FXAA_PRESET_4:
		LoadPreset4();
		break;
	case FXAA_PRESET_5:
		LoadPreset5();
		break;
	}
}

// Initialize settings with default values.
void FXAA_DESC::Init()
{
	// Set preset settings.
	this->LoadPreset(DEFAULT_FXAA_PRESET);

	// Set other settings.
	this->FXAA = FXAA_BOOLEAN_FLAG::ON;
	this->LUMINANCE_METHOD = DEFAULT_LUMINANCE_METHOD;
	this->DEBUG_DISCARD = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_PASSTHROUGH = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_HORZVERT = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_PAIR = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_NEGPOS = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_OFFSET = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_HIGHLIGHT = FXAA_BOOLEAN_FLAG::OFF;
	this->DEBUG_GRAYSCALE = 0.0;
	this->DEBUG_GRAYSCALE_CHANNEL = 1;
}

// Load preset zero.
void FXAA_DESC::LoadPreset0()
{
	this->EDGE_THRESHOLD = PRESET0_EDGE_THRESHOLD;
	this->EDGE_THRESHOLD_MIN = PRESET0_EDGE_THRESHOLD_MINIMUM;
	this->SEARCH_STEPS = PRESET0_SEARCH_STEPS;
	this->SEARCH_ACCELERATION = PRESET0_SEARCH_ACCELERATION;
	this->SEARCH_THRESHOLD = PRESET0_SEARCH_THRESHOLD;
	this->SUBPIX = PRESET0_SUBPIX;
	this->SUBPIX_FASTER = PRESET0_SUBPIX_FASTER;
	this->SUBPIX_CAP = PRESET0_SUBPIX_CAP;
	this->SUBPIX_TRIM = PRESET0_SUBPIX_TRIM;
}

// Load preset 1.
void FXAA_DESC::LoadPreset1()
{
	this->EDGE_THRESHOLD = PRESET1_EDGE_THRESHOLD;
	this->EDGE_THRESHOLD_MIN = PRESET1_EDGE_THRESHOLD_MINIMUM;
	this->SEARCH_STEPS = PRESET1_SEARCH_STEPS;
	this->SEARCH_ACCELERATION = PRESET1_SEARCH_ACCELERATION;
	this->SEARCH_THRESHOLD = PRESET1_SEARCH_THRESHOLD;
	this->SUBPIX = PRESET1_SUBPIX;
	this->SUBPIX_FASTER = PRESET1_SUBPIX_FASTER;
	this->SUBPIX_CAP = PRESET1_SUBPIX_CAP;
	this->SUBPIX_TRIM = PRESET1_SUBPIX_TRIM;
}

// Load preset 2.
void FXAA_DESC::LoadPreset2()
{
	this->EDGE_THRESHOLD = PRESET2_EDGE_THRESHOLD;
	this->EDGE_THRESHOLD_MIN = PRESET2_EDGE_THRESHOLD_MINIMUM;
	this->SEARCH_STEPS = PRESET2_SEARCH_STEPS;
	this->SEARCH_ACCELERATION = PRESET2_SEARCH_ACCELERATION;
	this->SEARCH_THRESHOLD = PRESET2_SEARCH_THRESHOLD;
	this->SUBPIX = PRESET2_SUBPIX;
	this->SUBPIX_FASTER = PRESET2_SUBPIX_FASTER;
	this->SUBPIX_CAP = PRESET2_SUBPIX_CAP;
	this->SUBPIX_TRIM = PRESET2_SUBPIX_TRIM;
}

// Load preset 3.
void FXAA_DESC::LoadPreset3()
{
	this->EDGE_THRESHOLD = PRESET3_EDGE_THRESHOLD;
	this->EDGE_THRESHOLD_MIN = PRESET3_EDGE_THRESHOLD_MINIMUM;
	this->SEARCH_STEPS = PRESET3_SEARCH_STEPS;
	this->SEARCH_ACCELERATION = PRESET3_SEARCH_ACCELERATION;
	this->SEARCH_THRESHOLD = PRESET3_SEARCH_THRESHOLD;
	this->SUBPIX = PRESET3_SUBPIX;
	this->SUBPIX_FASTER = PRESET3_SUBPIX_FASTER;
	this->SUBPIX_CAP = PRESET3_SUBPIX_CAP;
	this->SUBPIX_TRIM = PRESET3_SUBPIX_TRIM;
}

// Load preset 4.
void FXAA_DESC::LoadPreset4()
{
	this->EDGE_THRESHOLD = PRESET4_EDGE_THRESHOLD;
	this->EDGE_THRESHOLD_MIN = PRESET4_EDGE_THRESHOLD_MINIMUM;
	this->SEARCH_STEPS = PRESET4_SEARCH_STEPS;
	this->SEARCH_ACCELERATION = PRESET4_SEARCH_ACCELERATION;
	this->SEARCH_THRESHOLD = PRESET4_SEARCH_THRESHOLD;
	this->SUBPIX = PRESET4_SUBPIX;
	this->SUBPIX_FASTER = PRESET4_SUBPIX_FASTER;
	this->SUBPIX_CAP = PRESET4_SUBPIX_CAP;
	this->SUBPIX_TRIM = PRESET4_SUBPIX_TRIM;
}


// Load preset 5.
void FXAA_DESC::LoadPreset5()
{
	this->EDGE_THRESHOLD = PRESET5_EDGE_THRESHOLD;
	this->EDGE_THRESHOLD_MIN = PRESET5_EDGE_THRESHOLD_MINIMUM;
	this->SEARCH_STEPS = PRESET5_SEARCH_STEPS;
	this->SEARCH_ACCELERATION = PRESET5_SEARCH_ACCELERATION;
	this->SEARCH_THRESHOLD = PRESET5_SEARCH_THRESHOLD;
	this->SUBPIX = PRESET5_SUBPIX;
	this->SUBPIX_FASTER = PRESET5_SUBPIX_FASTER;
	this->SUBPIX_CAP = PRESET5_SUBPIX_CAP;
	this->SUBPIX_TRIM = PRESET5_SUBPIX_TRIM;
}