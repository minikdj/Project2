#include "PerFragmentOperations.h"

bool FragmentOperations::perPixelLightingEnabled = false;

fogType FragmentOperations::fogSetting = NO_FOG;

const double fogEnd = 20.0;
const double fogStart = 1.0;
const double fogDensity = 0.2;
color fogColor(0.75, 0.75, 0.75, 1.0);

void FragmentOperations::applyFog(Fragment & fragment)
{
	double fogFactor = 1.0;
	double distanceFromViewPoint = glm::length(fragment.worldPosition - PerVertex::eyePositionInWorldCoords);

	switch (fogSetting) {

	case LINEAR_FOG:

		fogFactor = glm::max((fogEnd - distanceFromViewPoint) / (fogEnd - fogStart), 0.0);
		break;

	case EXPONENTIAL_FOG:

		fogFactor = glm::exp(-(fogDensity * distanceFromViewPoint));
		break;

	case EXPONENTIAL_2_FOG:
		fogFactor = glm::exp(-glm::pow((fogDensity * distanceFromViewPoint), 2));
		break;

	default:
		std::cout << "Illegal Fog Type" << std::endl;

	}

	fragment.shadedColor = fogFactor * fragment.shadedColor + (1.0 - fogFactor) * fogColor;

} // end applyFog

void FragmentOperations::applyLighting( Fragment & fragment)
{
    color totalLight = fragment.material.emissiveColor;
    double alpha = fragment.material.diffuseColor.a;
    
    for(auto & light : lights) {
        totalLight += light->illuminate(PerVertex::eyePositionInWorldCoords, 
                                                        fragment.worldPosition, 
                                                        fragment.worldNormal, 
                                                        fragment.material);
    }
    fragment.material.diffuseColor.a = alpha;
    fragment.shadedColor = totalLight;

} // end applyLighting



void FragmentOperations::processFragment(Fragment & fragment)
{


    // Set the color for the pixel 
    if (frameBuffer.getDepth((int)fragment.windowPosition.x, (int)fragment.windowPosition.y) > fragment.windowPosition.z) {
        if (FragmentOperations::perPixelLightingEnabled) {
            FragmentOperations::applyLighting(fragment);
        }
        if( fogSetting != NO_FOG ) {

	        fogColor = frameBuffer.getClearColor( );
		    applyFog( fragment );
	    }

        //Save alpha value for use in blending
	    double alpha = fragment.material.diffuseColor.a;	



    	if (alpha < 1.0) {

        	color destination = frameBuffer.getPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y);

	    	color source = fragment.shadedColor;

		    color blendedColor = alpha * source + (1.0 - alpha ) * destination;

		    // Set the color for the pixel 
		    frameBuffer.setPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, blendedColor);

	    }
	    else {
		    // Set the color for the pixel 
		    frameBuffer.setPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.shadedColor);
	    }
        // frameBuffer.setPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.shadedColor);
        frameBuffer.setDepth((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.windowPosition.z);
    }
} // end processFragment

