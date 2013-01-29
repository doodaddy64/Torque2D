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

#ifndef _PBXNATIVETARGET_H
#define _PBXNATIVETARGET_H

#include "xcodeTemplate/PBXBase.h"
#include <string>
#include <vector>

using namespace std;

class XCConfigurationList;
class PBXFileReference;
class PBXProject;
class PBXBaseBuildPhase;
class PBXBuildFile;

class PBXNativeTarget : public PBXBase
{
protected:
    PBXProject* m_ParentProject;

public:
    XCConfigurationList* m_BuildConfigurationList;

    vector<PBXBaseBuildPhase*> m_BuildPhases;

    string m_ProductName;

    PBXFileReference* m_ProductReference;

    string m_ProductType;

public:
    PBXNativeTarget();
    virtual ~PBXNativeTarget();

    bool ProcessXMLFile(const string& filePath, PBXProject* parent);

    bool DoesBuildPhaseExist(const string& buildPhase);

    void AddBuildFileToBuildPhase(PBXBuildFile* buildFile);

    DEFINE_ISA(PBXNativeTarget)
    DEFINE_GLOBAL_LIST(PBXNativeTarget)
    DEFINE_WRITE_GLOBAL_LIST(PBXNativeTarget)
};

#endif  // _PBXNATIVETARGET_H
