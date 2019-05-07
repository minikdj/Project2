#include "PerFragmentOperations.h"

bool FragmentOperations::perPixelLightingEnabled = false;

fogType FragmentOperations::fogSetting = NO_FOG;


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
    if (FragmentOperations::perPixelLightingEnabled) {
        FragmentOperations::applyLighting(fragment);
    }
    // Set the color for the pixel 
    if (frameBuffer.getDepth((int)fragment.windowPosition.x, (int)fragment.windowPosition.y) > fragment.windowPosition.z) {
        frameBuffer.setPixel((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.shadedColor);
        frameBuffer.setDepth((int)fragment.windowPosition.x, (int)fragment.windowPosition.y, fragment.windowPosition.z);
    }
} // end processFragment
