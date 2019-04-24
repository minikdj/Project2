#pragma once

#include "Defines.h"
#include "FrameBuffer.h"
#include "Lights.h"
#include "PerVertexOperations.h"

extern FrameBuffer frameBuffer;

extern std::vector<LightSource*> lights;

enum fogType { NO_FOG, LINEAR_FOG, EXPONENTIAL_FOG, EXPONENTIAL_2_FOG };


struct Fragment {

	color shadedColor; // Result of lighting calculations
	dvec3 windowPosition;
	Material material;
	dvec3 worldNormal;
	dvec3 worldPosition;

};

class FragmentOperations
{
	public:

		static fogType fogSetting;

		static bool perPixelLightingEnabled;
	
		static void processFragment(Fragment & fragment);

	protected:

		static void applyLighting(Fragment & fragment);

		static void applyFog(Fragment & fragment);


};// end FragmentOperations



