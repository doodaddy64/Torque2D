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

#ifndef _XCBUILDCONFIGURATION_H
#define _XCBUILDCONFIGURATION_H

#include "xcodeTemplate/PBXBase.h"
#include "tinyXML/tinyxml.h"
#include <string>
#include <vector>

using namespace std;

class PBXFileReference;

class XCBuildConfiguration : public PBXBase
{
protected:
    // The file reference path to search for
    string m_BaseConfigurationReferencePath;

public:
    enum BuildSettingType {
        BST_Single,
        BST_List,
    };

    vector<string> m_BuildSettingName;

    vector< vector<string> > m_BuildSettingValues;

    vector<S32> m_BuildSettingType;

    PBXFileReference* m_BaseConfigurationReference;

public:
    XCBuildConfiguration();
    virtual ~XCBuildConfiguration();

    void SetBaseConfigurationReferencePath(const string& path) { m_BaseConfigurationReferencePath = path; }

    void ResolveConfigFileReference();

    bool ProcessConfigXML(TiXmlElement& xml);

    DEFINE_ISA(XCBuildConfiguration)
    DEFINE_GLOBAL_LIST(XCBuildConfiguration)
    DEFINE_WRITE_GLOBAL_LIST(PBXTargetDependency)
};

#endif  // _XCBUILDCONFIGURATION_H
