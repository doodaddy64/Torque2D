//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#include "platformOSX/platformOSX.h"
#include "console/console.h"

//-----------------------------------------------------------------------------
void Processor::init()
{
    // Currently there is no way to get hardware information using Cocoa
    // The previous Carbon API had access to Gestalt, but this is deprecated
    // Until Apple can provide an API, there is no way to initialize this
    Con::printf("CPU initialization:");
    Con::printf("   Not supported in OS X (Cocoa)");
}
