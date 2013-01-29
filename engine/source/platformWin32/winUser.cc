//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
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
