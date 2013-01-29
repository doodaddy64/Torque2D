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

#include "xcodeTemplate/XCBuildConfiguration.h"
#include "xcodeTemplate/PBXFileReference.h"
#include "platform/stringFunctions.h"

IMPLEMENT_GLOBAL_LIST(XCBuildConfiguration)

XCBuildConfiguration::XCBuildConfiguration()
{
    // Default name
    m_Name = "Release";

    m_BaseConfigurationReference = NULL;

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

XCBuildConfiguration::~XCBuildConfiguration()
{
}

void XCBuildConfiguration::ResolveConfigFileReference()
{
    if(!m_BaseConfigurationReferencePath.empty())
    {
        // Attempt to find the file reference by path
        for(U32 i=0; i<PBXFileReference::m_List.size(); ++i)
        {
            const string& path = PBXFileReference::m_List[i]->m_Path;
            if(Platform::StringCompare(path, m_BaseConfigurationReferencePath))
            {
                // Found our match
                m_BaseConfigurationReference = PBXFileReference::m_List[i];
                break;
            }
        }
    }
}

bool XCBuildConfiguration::ProcessConfigXML(TiXmlElement& xml)
{
    TiXmlElement* buildSettings = xml.FirstChildElement("BuildSettings");
    if(buildSettings)
    {
        TiXmlElement* setting = buildSettings->FirstChildElement();
        const string settingTypeLabel = "settingType";
        while(setting)
        {
            // Retrieve the data
            string settingName = setting->Value();

            // Check if this is a special type that has the setting name in the 'name'
            // attribute.  If so, then also surround the name with quotes.
            if(Platform::StringCompareNoCase(settingName, "QuotedBuildSetting"))
            {
                string name = setting->Attribute("name");
                settingName = "\"" + name + "\"";
            }

            const string* settingType = setting->Attribute(settingTypeLabel);
            XCBuildConfiguration::BuildSettingType type = XCBuildConfiguration::BST_Single;
            if(settingType && !settingType->empty() && Platform::StringCompareNoCase(*settingType, "List"))
            {
                type = XCBuildConfiguration::BST_List;
            }

            // Build the setting
            m_BuildSettingName.push_back(settingName);
            m_BuildSettingType.push_back(type);

            // Process the value(s)
            if(type == XCBuildConfiguration::BST_List)
            {
                // Process multiple values
                vector<string> values;
                TiXmlElement* value = setting->FirstChildElement("Item");
                while(value)
                {
                    string text = value->GetText();
                    values.push_back(text);
                    value = value->NextSiblingElement("Item");
                }

                m_BuildSettingValues.push_back(values);
            }
            else
            {
                // Process a single value
                vector<string> values;
                string value = setting->GetText();
                values.push_back(value);

                m_BuildSettingValues.push_back(values);
            }

            setting = setting->NextSiblingElement();
        }
    }

    return true;
}

IMPLEMENT_WRITE_GLOBAL_LIST(XCBuildConfiguration)
{
    stream << endl << "/* Begin XCBuildConfiguration section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        XCBuildConfiguration* config = m_List[i];

        stream << "\t\t";
        config->GetUUID().Write(stream);
        if(!config->GetName().empty())
        {
            stream << " /* " << config->GetName() << " */";
        }
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << config->GetISA() << ";" << endl;

        if(config->m_BaseConfigurationReference)
        {
            stream << "\t\t\tbaseConfigurationReference = ";
            config->m_BaseConfigurationReference->GetUUID().Write(stream);
            stream << " /* " << config->m_BaseConfigurationReference->GetName() << " */;" << endl;
        }

        stream << "\t\t\tbuildSettings = {" << endl;
        for(U32 j=0; j<config->m_BuildSettingName.size(); ++j)
        {
            stream << "\t\t\t\t" << config->m_BuildSettingName[j] << " = ";

            if(config->m_BuildSettingType[j] == XCBuildConfiguration::BST_List)
            {
                stream << "(" << endl;

                for(U32 k=0; k<config->m_BuildSettingValues[j].size(); ++k)
                {
                    stream << "\t\t\t\t\t" << config->m_BuildSettingValues[j][k] << "," << endl;
                }

                stream << "\t\t\t\t);" << endl;
            }
            else
            {
                stream << config->m_BuildSettingValues[j][0] << ";" << endl;
            }
        }
        stream << "\t\t\t};" << endl;

        if(!config->m_Name.empty())
        {
            stream << "\t\t\tname = " << config->m_Name << ";" << endl;
        }

        stream << "\t\t};" << endl;
    }

    stream << "/* End XCBuildConfiguration section */" << endl;

    return true;
}
