//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "platformOSX/platformOSX.h"
#include "platform/platformVideo.h"

Resolution Video::getDesktopResolution()
{
    // Get the screen the application window resides in and is receiving input
    NSScreen* mainScreen = [NSScreen mainScreen];
    
    // Get the visible boundaries
    NSRect screenRect = [mainScreen visibleFrame];

    // Get the screen depth. You cannot access depth directly. It must be passed
    // into a function that will return the bpp
    int bpp = NSBitsPerPixelFromDepth([mainScreen depth]);
    
    // Build the return resolution
    Resolution resolution;
    resolution.w = (U32)screenRect.size.width;
    resolution.h = (U32)screenRect.size.height;
    resolution.bpp = (U32)bpp;
    
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    [platState setDesktopWidth:resolution.w];
    [platState setDesktopHeight:resolution.h];
    [platState setDesktopBitsPixel:resolution.bpp];
    
    // Return the new resolution
    return resolution;
}