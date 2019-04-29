#include "PerFragmentOperations.h"

bool FragmentOperations::perPixelLightingEnabled = false;

fogType FragmentOperations::fogSetting = NO_FOG;



void FragmentOperations::processFragment(Fragment & fragment)
{
	// Set the color for the pixel 
    if (frameBuffer.getDepth((int)fragment.windowPosition.x, (int)fragment.windowPosition.y) > fragment.windowPosition.z) {


    }
    // Set the color for the pixel 
    frameBuffer.setPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.shadedColor);

    frameBuffer.setDepth((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.windowPosition.z);
} // end processFragment
