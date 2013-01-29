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

#include <string>
#include <vector>
#include <stdio.h>
//#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>

//-----------------------------------------------------------------------------

void Platform::getCurrentDirectory(string& cwd)
{
    // get the current directory, the one that would be opened if we did a fopen(".")
    cwd = getcwd(NULL, 0);
    char* dir = getcwd(NULL, 0);
    cwd = dir;
    free(dir);
}

//-----------------------------------------------------------------------------

void Platform::DumpFiles(const string& path, vector<string>& outList)
{
    DIR *dir;
    dirent *entry;
    
    // be sure it opens.
    const char* curPath = path.c_str();
    dir = opendir(curPath);
    if(!dir)
        return;
    
    // look inside the current directory
    while( (entry = readdir(dir)) )
    {
        if( entry->d_type == DT_DIR )
        {
            // Skip directories
            continue;
        }
        else
        {
            // Found file
            outList.push_back(entry->d_name);
        }
    }
    closedir(dir);
}

//-----------------------------------------------------------------------------

void Platform::DumpDirectories(const string& path, vector<string>& outList)
{
    DIR *dir;
    dirent *entry;
    
    // be sure it opens.
    const char* curPath = path.c_str();
    dir = opendir(curPath);
    if(!dir)
        return;
    
    // look inside the current directory
    while( (entry = readdir(dir)) )
    {
        if( entry->d_type == DT_DIR )
        {
            // Skip . and .. directories
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            
            // Skip excluded directories
            if(Platform::IsExcludedDirectory(entry->d_name))
                continue;
            
            outList.push_back(entry->d_name);
        }
    }
    closedir(dir);
}

//-----------------------------------------------------------------------------

bool Platform::CreatePath(const char* path)
{
    // if the path exists, we're done.
    struct stat statData;
    if( stat(path, &statData) == 0 )
    { 
        return true;               // exists, rejoice.
    }
    
    // get the parent path.
    // we're not using basename because it's not thread safe.
    U32 len = strlen(path);
    char parent[len];
    bool isDirPath = false;
    
    strncpy(parent,path,len);
    parent[len] = '\0';
    if(parent[len - 1] == '/')
    {
        parent[len - 1] = '\0';    // cut off the trailing slash, if there is one
        isDirPath = true;          // we got a trailing slash, so file is a directory.
    }
    
    // recusively create the parent path.
    // only recurse if newpath has a slash that isn't a leading slash.
    char *slash = strrchr(parent,'/');
    if( slash  && slash != parent)
    {
        // snip the path just after the last slash.
        slash[1] = '\0';
        // recusively create the parent path. fail if parent path creation failed.
        if(!Platform::CreatePath(parent))
            return false;
    }
    
    // create *file if it is a directory path.
    if(isDirPath)
    {
        // try to create the directory
        if( mkdir(path, 0777) != 0) // app may reside in global apps dir, and so must be writable to all.
            return false;
    }
    
    return true;
}
