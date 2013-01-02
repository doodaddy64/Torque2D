//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platformiOS/platformiOS.h"
#include "sim/simBase.h"
#include "platform/nativeDialogs/fileDialog.h"
#include "platform/threads/mutex.h"
#include "memory/safeDelete.h"
#include "math/mMath.h"
#include "string/Unicode.h"
#include "console/consoleTypes.h"
#include "platform/threads/thread.h"

bool FileDialog::Execute()
{
    return true;
}

//-----------------------------------------------------------------------------
// Default Path Property - String Validated on Write
//-----------------------------------------------------------------------------
bool FileDialog::setDefaultPath(void* obj, const char* data)
{

   if( !data )
      return true;

   return true;

};

//-----------------------------------------------------------------------------
// Default File Property - String Validated on Write
//-----------------------------------------------------------------------------
bool FileDialog::setDefaultFile(void* obj, const char* data)
{
   if( !data )
      return true;

   return true;
};