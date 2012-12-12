//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#include "platform/nativeDialogs/fileDialog.h"
#include "console/console.h"

ConsoleMethod( FileDialog, Execute, bool, 2, 2, "() Executes the dialog\n"
			  "@return Returns true on success and false otherwise")
{
   return object->Execute();
}