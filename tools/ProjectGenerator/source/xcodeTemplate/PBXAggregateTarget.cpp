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

#include "xcodeTemplate/PBXAggregateTarget.h"
#include "xcodeTemplate/XCConfigurationList.h"
#include "xcodeTemplate/PBXTargetDependency.h"
#include "xcodeTemplate/PBXProject.h"
#include "xcodeTemplate/XCBuildConfiguration.h"
#include "xcodeTemplate/PBXTargetDependency.h"
#include "tinyXML/tinyxml.h"

IMPLEMENT_GLOBAL_LIST(PBXAggregateTarget)

PBXAggregateTarget::PBXAggregateTarget()
{
    m_ParentProject = NULL;
    m_BuildConfigurationList = NULL;

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXAggregateTarget::~PBXAggregateTarget()
{
}

bool PBXAggregateTarget::ProcessXMLFile(const string& filePath, PBXProject* parent)
{
    m_ParentProject = parent;

    // Read the given file
    TiXmlDocument xml;
    bool result = xml.LoadFile(filePath);
    if(!result)
    {
        printf("PBXAggregateTarget::ProcessXMLFile(): Cannot load target XML file '%s'.\n", filePath.c_str());
        return false;
    }

    // Process the target entry
    TiXmlElement* root = xml.FirstChildElement("TargetAll");
    if(!root)
    {
        printf("PBXAggregateTarget::ProcessXMLFile(): Target XML file '%s' is missing root TargetAll element\n", filePath.c_str());
        return false;
    }

    // Set some automatic values as we're the All target
    SetName("All");
    m_ProductName = "All";

    // Process build configurations
    TiXmlElement* buildConfigs = root->FirstChildElement("BuildConfigs");
    if(!buildConfigs)
    {
        printf("PBXAggregateTarget::ProcessXMLFile(): Target XML file '%s' is missing build configurations\n", filePath.c_str());
        return false;
    }

    m_BuildConfigurationList = new XCConfigurationList();
    m_BuildConfigurationList->SetOwner(this);
    m_BuildConfigurationList->m_DefaultConfigurationIsVisible = buildConfigs->Attribute("defaultConfigurationIsVisible");
    m_BuildConfigurationList->m_DefaultConfigurationName = buildConfigs->Attribute("default");
    m_BuildConfigurationList->ProcessBuildConfigXML(*buildConfigs);

    // Add the dependencies to other project targets
    for(U32 i=0; i<PBXTargetDependency::m_List.size(); ++i)
    {
        PBXTargetDependency* td = PBXTargetDependency::m_List[i];
        m_Dependencies.push_back(td);
    }

    // Add ourselves as a target to the project
    m_ParentProject->m_Targets.push_back(this);

    return true;
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXAggregateTarget)
{
    stream << endl << "/* Begin PBXAggregateTarget section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXAggregateTarget* target = m_List[i];

        stream << "\t\t";
        target->GetUUID().Write(stream);
        stream << " /* " << target->GetName() << " */";
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << target->GetISA() << ";" << endl;

        stream << "\t\t\tbuildConfigurationList = ";
        target->m_BuildConfigurationList->GetUUID().Write(stream);
        stream << " /* Build configuration list for PBXAggregateTarget \"" << target->GetName() << "\" */;" << endl;

        stream << "\t\t\tbuildPhases = (" << endl;
        // Build phases are not supported at this time
        stream << "\t\t\t);" << endl;

        stream << "\t\t\tdependencies = (" << endl;
        for(U32 j=0; j<target->m_Dependencies.size(); ++j)
        {
            stream << "\t\t\t\t";
            target->m_Dependencies[j]->GetUUID().Write(stream);
            stream << " /* PBXTargetDependency */," << endl;
        }
        stream << "\t\t\t);" << endl;

        stream << "\t\t\tname = " << target->GetName() << ";" << endl;
        stream << "\t\t\tproductName = " << target->m_ProductName << ";" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End PBXAggregateTarget section */" << endl;

    return true;
}
