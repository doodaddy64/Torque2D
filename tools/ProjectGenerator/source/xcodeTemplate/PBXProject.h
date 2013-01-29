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

#ifndef _PBXPROJECT_H
#define _PBXPROJECT_H

#include "xcodeTemplate/PBXBase.h"
#include "tinyXML/tinyxml.h"
#include <string>
#include <vector>
#include <fstream>

using namespace std;

class PBXGroup;
class XCConfigurationList;
class PBXNativeTarget;
class PBXBuildFile;

class PBXProject : public PBXBase
{
protected:
    // Stores the path to engine source.  Becomes $TSS_SOURCE_PATH
    string m_EngineSourcePath;

    // Stores the path to the engine libraries.  Becomes $TSS_LIB_PATH
    string m_EngineLibPath;

public:
    XCConfigurationList* m_BuildConfigurationList;

    string m_CompatibilityVersion;

    string m_DevelopmentRegion;

    S32 m_HasScannedForEncodings;

    vector<string> m_KnownRegions;

    PBXGroup* m_MainGroup;

    PBXGroup* m_ProductRefGroup;

    string m_ProjectDirPath;

    vector<PBXBase*> m_ProjectReferences; // This is the wrong type, but maybe should be calculated

    string m_ProjectRoot;

    vector<string> m_AttributeName;
    vector<string> m_AttributeValue;

    // Native or Aggregate targets
    vector<PBXBase*> m_Targets;

public:
    PBXProject();
    virtual ~PBXProject();

    const string& GetEngineSourcePath() { return m_EngineSourcePath; }
    const string& GetEngineLibPath() { return m_EngineLibPath; }

    // Process the Project tag of a multi-project XML file
    bool ProcessProjectXML(TiXmlElement& xml);

    bool DoesBuildPhaseExist(const string& buildPhase);

    void AddBuildFileToTarget(PBXBuildFile* buildFile);

    DEFINE_ISA(PBXProject)
    DEFINE_GLOBAL_LIST(PBXProject)
    DEFINE_WRITE_GLOBAL_LIST(PBXProject)
};

#endif  // _PBXPROJECT_H
