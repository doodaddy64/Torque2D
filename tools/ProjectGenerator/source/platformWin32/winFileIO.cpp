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

#include "platform/types.h"
#include "platform/fileIO.h"
#include "platform/unicode.h"
#include "platformWin32/platformWin32.h"

#include <string>
#include <vector>

//-------------------------------------- Helper Functions
static void forwardslash(char *str)
{
    while(*str)
    {
        if(*str == '\\')
            *str = '/';
        str++;
    }
}

static void backslash(char *str)
{
    while(*str)
    {
        if(*str == '/')
            *str = '\\';
        str++;
    }
}

//-----------------------------------------------------------------------------

void Platform::getCurrentDirectory(string& cwd)
{
   char cwd_buf[2048];
   GetCurrentDirectoryA(2047, cwd_buf);
   forwardslash(cwd_buf);

   cwd = cwd_buf;
}

//-----------------------------------------------------------------------------

void Platform::DumpFiles(const string& path, vector<string>& outList)
{
    string searchPath;
    Platform::MakeFullPathName("*", searchPath, &path);

#ifdef UNICODE
    UTF16 search[1024];
    convertUTF8toUTF16((UTF8 *)searchPath.c_str(), search, sizeof(search));
#else
    char *search = searchPath.c_str();
#endif

    WIN32_FIND_DATA findData;
    HANDLE handle = FindFirstFile((LPCWSTR)search, &findData);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
#ifdef UNICODE
        char fnbuf[1024];
        Platform::convertUTF16toUTF8((UTF16*)findData.cFileName, (UTF8 *)fnbuf, sizeof(fnbuf));
#else
        char *fnbuf = findData.cFileName;
#endif

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            continue;

        outList.push_back(fnbuf);
    }
    while(FindNextFile(handle, &findData));

    FindClose(handle);
}

void Platform::DumpDirectories(const string& path, vector<string>& outList)
{
    string searchPath;
    Platform::MakeFullPathName("*", searchPath, &path);

#ifdef UNICODE
    UTF16 search[1024];
    convertUTF8toUTF16((UTF8 *)searchPath.c_str(), search, sizeof(search));
#else
    char *search = searchPath.c_str();
#endif

    WIN32_FIND_DATA findData;
    HANDLE handle = FindFirstFile((LPCWSTR)search, &findData);
    if (handle == INVALID_HANDLE_VALUE)
    {
        return;
    }

    do
    {
#ifdef UNICODE
        char fnbuf[1024];
        Platform::convertUTF16toUTF8((UTF16*)findData.cFileName, (UTF8 *)fnbuf, sizeof(fnbuf));
#else
        char *fnbuf = findData.cFileName;
#endif

        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // make sure it is a directory
            if (findData.dwFileAttributes & (FILE_ATTRIBUTE_OFFLINE|FILE_ATTRIBUTE_SYSTEM) )                             
                continue;

            // skip . and .. directories
            if (strcmp(fnbuf, ".") == 0 || strcmp(fnbuf, "..") == 0)
                continue;

            // Skip excluded directores
            if(Platform::IsExcludedDirectory(fnbuf))
                continue;

            outList.push_back(fnbuf);
        }
    }
    while(FindNextFile(handle, &findData));

    FindClose(handle);
}

//-----------------------------------------------------------------------------

bool Platform::CreatePath(const char* path)
{
    char pathbuf[1024];
    const char *dir;
    pathbuf[0] = 0;
    U32 pathLen = 0;

    while((dir = strchr(path, '/')) != NULL)
    {
        strncpy(pathbuf + pathLen, path, dir - path);
        pathbuf[pathLen + dir-path] = 0;

#ifdef UNICODE
        UTF16 b[1024];
        Platform::convertUTF8toUTF16((UTF8 *)pathbuf, b, sizeof(b));
        BOOL ret = CreateDirectory((LPCWSTR)b, NULL);
#else
        BOOL ret = CreateDirectory(pathbuf, NULL);
#endif
        pathLen += dir - path;
        pathbuf[pathLen++] = '/';
        path = dir + 1;
    }
    return true;
}
