//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "console/console.h"
#include "math/mMath.h"

ConsoleFunction(MathInit, void, 1, 10, "(DETECT|C|VEC)")
{
    Math::init(0);
}
