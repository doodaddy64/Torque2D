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

#ifndef _PBXBUILDFILE_H
#define _PBXBUILDFILE_H

#include "xcodeTemplate/PBXBase.h"
#include <string>
#include <vector>

using namespace std;

class PBXFileReference;

class PBXBuildFile : public PBXBase
{
protected:
    // The name of the build phase this file belongs to.
    // Used in comments when writing to project file.
    string m_BuildPhase;
public:
    PBXBase* m_FileReference;

public:
    PBXBuildFile();
    virtual ~PBXBuildFile();

    const string& GetBuildPhase() { return m_BuildPhase; }
    void SetBuildPhase(const string& phase) { m_BuildPhase = phase; }

    DEFINE_ISA(PBXBuildFile)
    DEFINE_GLOBAL_LIST(PBXBuildFile)
    DEFINE_WRITE_GLOBAL_LIST(PBXBuildFile)
};

#endif  // _PBXBUILDFILE_H
