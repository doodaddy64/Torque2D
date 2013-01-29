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

#ifndef _PBXAGGREGATETARGET_H
#define _PBXAGGREGATETARGET_H

#include "xcodeTemplate/PBXBase.h"

#include <string>
#include <vector>

using namespace std;

class XCConfigurationList;
class PBXTargetDependency;
class PBXProject;

class PBXAggregateTarget : public PBXBase
{
protected:
    PBXProject* m_ParentProject;

public:
    vector<string> m_BuildPhases;

    XCConfigurationList* m_BuildConfigurationList;

    vector<PBXTargetDependency*> m_Dependencies;

    string m_ProductName;

public:
    PBXAggregateTarget();
    virtual ~PBXAggregateTarget();

    bool ProcessXMLFile(const string& filePath, PBXProject* parent);

    DEFINE_ISA(PBXAggregateTarget)
    DEFINE_GLOBAL_LIST(PBXAggregateTarget)
    DEFINE_WRITE_GLOBAL_LIST(PBXAggregateTarget)
};

#endif  // _PBXAGGREGATETARGET_H
