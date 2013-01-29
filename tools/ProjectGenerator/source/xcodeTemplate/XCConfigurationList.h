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

#ifndef _XCCONFIGURATIONLIST_H
#define _XCCONFIGURATIONLIST_H

#include "xcodeTemplate/PBXBase.h"
#include "tinyXML/tinyxml.h"
#include <string>
#include <vector>

using namespace std;

class XCBuildConfiguration;

class XCConfigurationList : public PBXBase
{
protected:
    PBXBase* m_Owner;

    // Holds a reference to a config file that will be resolved later
    // by the created XCBuildConfiguration
    string m_BaseConfigurationReferencePath;

public:
    vector<XCBuildConfiguration*> m_BuildConfigurations;

    string m_DefaultConfigurationIsVisible;

    string m_DefaultConfigurationName;

public:
    XCConfigurationList();
    virtual ~XCConfigurationList();

    PBXBase* GetOwner() { return m_Owner; }
    void SetOwner(PBXBase* owner) { m_Owner = owner; }

    void SetBaseConfigurationReferencePath(const string& path) { m_BaseConfigurationReferencePath = path; }

    bool ProcessBuildConfigXML(TiXmlElement& buildConfigs);

    DEFINE_ISA(XCConfigurationList)
    DEFINE_GLOBAL_LIST(XCConfigurationList)
    DEFINE_WRITE_GLOBAL_LIST(XCConfigurationList)
};

#endif  // _XCCONFIGURATIONLIST_H
