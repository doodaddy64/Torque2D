//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "console/console.h"
#include "string/stringTable.h"

typedef long SHANDLE_PTR;
#include <shlobj.h>
#include <windows.h>
#include <lmcons.h>

#define CSIDL_PROFILE 0x0028

const char *Platform::getUserDataDirectory() 
{
   char szBuffer[512];
   if(! SHGetSpecialFolderPathA( NULL, LPSTR( szBuffer ), CSIDL_APPDATA, true ) )
      return "";

   char *ptr = szBuffer;
   while(*ptr)
   {
      if(*ptr == '\\')
         *ptr = '/';
      ++ptr;
   }

   return StringTable->insert(szBuffer);
}

const char *Platform::getUserHomeDirectory() 
{
   char szBuffer[512];
   if(! SHGetSpecialFolderPathA( NULL, LPSTR( szBuffer ), CSIDL_PERSONAL, false ) )
      if(! SHGetSpecialFolderPathA( NULL, LPSTR( szBuffer ), CSIDL_COMMON_DOCUMENTS, false ) )
         return "";

   char *ptr = szBuffer;
   while(*ptr)
   {
      if(*ptr == '\\')
         *ptr = '/';
      ++ptr;
   }

   return StringTable->insert(szBuffer);
}
