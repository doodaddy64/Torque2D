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

#ifndef _FILEIO_H
#define _FILEIO_H

#include <string>
#include <vector>

using namespace std;

namespace Platform
{
    void MakeFullPathName(const string& inPath, string& outPath, const string* cwd=NULL);

    bool IsFullPath(const char *path);

    bool CreatePath(const char* path);

    void GetFileExtension(const string& file, string& ext);

    void getCurrentDirectory(string& cwd);

    void DumpFiles(const string& path, vector<string>& outList);

    void DumpDirectories(const string& path, vector<string>& outList);

    void AddExcludedDirectory(const string& dir);

    void CleanExcludedDirectories();

    bool IsExcludedDirectory(const string& dir);
}

#endif  // _FILEIO_H
