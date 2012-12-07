//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
#include "console/console.h"
#include "platform/nativeDialogs/fileDialog.h"

ConsoleMethod( FileDialog, Execute, bool, 2, 2, "%fileDialog.Execute();" )
{
    return object->Execute();
}