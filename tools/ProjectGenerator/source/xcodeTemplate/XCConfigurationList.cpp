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

#include "xcodeTemplate/XCConfigurationList.h"
#include "xcodeTemplate/XCBuildConfiguration.h"

IMPLEMENT_GLOBAL_LIST(XCConfigurationList)

XCConfigurationList::XCConfigurationList()
{
    m_Owner = NULL;

    m_DefaultConfigurationIsVisible = "0";

    m_DefaultConfigurationName = "Release";

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

XCConfigurationList::~XCConfigurationList()
{
}

bool XCConfigurationList::ProcessBuildConfigXML(TiXmlElement& buildConfigs)
{
    TiXmlElement* buildConfig = buildConfigs.FirstChildElement("BuildConfig");
    while(buildConfig)
    {
        XCBuildConfiguration* config = new XCBuildConfiguration();
        config->SetName(buildConfig->Attribute("name"));
        config->SetBaseConfigurationReferencePath(m_BaseConfigurationReferencePath);

        // Process the build settings
        config->ProcessConfigXML(*buildConfig);

        m_BuildConfigurations.push_back(config);

        buildConfig = buildConfig->NextSiblingElement("BuildConfig");
    }

    return true;
}

IMPLEMENT_WRITE_GLOBAL_LIST(XCConfigurationList)
{
    stream << endl << "/* Begin XCConfigurationList section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        XCConfigurationList* config = m_List[i];

        stream << "\t\t";
        config->GetUUID().Write(stream);
        stream << " /* Build configuration list for " << config->GetOwner()->GetISA() << " \"" << config->GetOwner()->GetName() << "\" */";
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << config->GetISA() << ";" << endl;

        if(config->m_BuildConfigurations.size() > 0)
        {
            stream << "\t\t\tbuildConfigurations = (" << endl;

            for(U32 j=0; j<config->m_BuildConfigurations.size(); ++j)
            {
                stream << "\t\t\t\t";
                config->m_BuildConfigurations[j]->GetUUID().Write(stream);
                if(!config->m_BuildConfigurations[j]->GetName().empty())
                {
                    stream << " /* " << config->m_BuildConfigurations[j]->GetName() << " */";
                }
                stream << "," << endl;
            }

            stream << "\t\t\t);" << endl;
        }

        stream << "\t\t\tdefaultConfigurationIsVisible = " << config->m_DefaultConfigurationIsVisible << ";" << endl;
        stream << "\t\t\tdefaultConfigurationName = " << config->m_DefaultConfigurationName << ";" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End XCConfigurationList section */" << endl;

    return true;
}
