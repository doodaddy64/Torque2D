//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#include "platform/platform.h"
#include "platformOSX/platformOSX.h"
#include "math/mMath.h"

extern void mInstallLibrary_C();

//------------------------------------------------------------------------------
// Initialize math libraries
void Math::init(U32 properties)
{
    Con::printSeparator();
    Con::printf("Math initialization:");
    Con::printf("   Installing Standard C extensions");
    mInstallLibrary_C();
}

//------------------------------------------------------------------------------
// Get a platform abstracted random number
F32 Platform::getRandom()
{
    osxPlatState *platState = [osxPlatState sharedPlatState];
    
    RandomLCG* random = [platState platformRandom];
    
    return random->randF();
}
