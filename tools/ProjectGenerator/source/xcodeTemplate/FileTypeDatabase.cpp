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

#include "xcodeTemplate/FileTypeDatabase.h"
#include "platform/stringFunctions.h"
#include "tinyXML/tinyxml.h"
#include "platform/types.h"

FileTypeDatabase* FileTypeDatabase::m_Singleton = NULL;

FileTypeDatabase::FileTypeDatabase()
{
    if(!m_Singleton)
        m_Singleton = this;
}

FileTypeDatabase::~FileTypeDatabase()
{
    m_Singleton = NULL;
}

bool FileTypeDatabase::BuildDatabase()
{
    TiXmlDocument xml;
    bool result = xml.LoadFile("XcodeFileTypeDefinitions.xml");
    if(!result)
    {
        printf("FileTypeDatabase::BuildDatabase(): Cannot load file type definition file\n");
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    if(!root)
    {
        printf("FileTypeDatabase::BuildDatabase(): File definition file is missing root element\n");
        return false;
    }

    TiXmlElement* e = root->FirstChildElement("FileType");
    while(e)
    {
        // Get the standard attributes
        const char* extension = e->Attribute("extension");
        const char* type = e->Attribute("type");
        const char* buildPhase = e->Attribute("buildPhase");

        if(extension && type && buildPhase)
        {
            // Build the type attribute
            FileTypeAttributesMap attributes;
            attributes.insert(FileTypeAttributesPair("type", type));

            // Add the buildPhase attribute
            attributes.insert(FileTypeAttributesPair("buildPhase", buildPhase));

            // Some file types may have the explicit attribute
            int value = 0;
            const char* exp = e->Attribute("explicit", &value);
            if(exp && value == 1)
            {
                // If the explicit value is 1 then add the attribute as true
                attributes.insert(FileTypeAttributesPair("explicit", "true"));
            }
            else
            {
                // Either this attribute doesn't exist, or it is set to something
                // other than 1.
                attributes.insert(FileTypeAttributesPair("explicit", "false"));
            }

            m_Database.insert(FileTypePair(extension, attributes));
        }

        e = e->NextSiblingElement("FileType");
    }

    return true;
}

bool FileTypeDatabase::HasFileType(const string& extension)
{
    if(m_Database.count(extension) == 0)
        return false;

    return true;
}

string FileTypeDatabase::GetFileType(const string& extension)
{
    if(m_Database.count(extension) == 0)
    {
        printf("FileTypeDatabase::GetFileType(): Cannot find file type '%s'\n", extension.c_str());
        return m_EmptyString;
    }

    // Get then attributes for the extension
    FileTypeMap::iterator itr = m_Database.find(extension);
    FileTypeAttributesMap attributes = itr->second;

    // Find the type attribute
    FileTypeAttributesMap::iterator aitr = attributes.find("type");
    return aitr->second;
}

string FileTypeDatabase::GetFileBuildPhase(const string& extension)
{
    if(m_Database.count(extension) == 0)
    {
        printf("FileTypeDatabase::GetFileBuildPhase(): Cannot find file type '%s'\n", extension.c_str());
        return m_EmptyString;
    }

    // Get then attributes for the extension
    FileTypeMap::iterator itr = m_Database.find(extension);
    FileTypeAttributesMap attributes = itr->second;

    // Find the type attribute
    FileTypeAttributesMap::iterator aitr = attributes.find("buildPhase");
    return aitr->second;
}

bool FileTypeDatabase::FileTypeIsExplicit(const string& extension)
{
    if(m_Database.count(extension) == 0)
    {
        printf("FileTypeDatabase::FileTypeIsExplicit(): Cannot find file type '%s'\n", extension.c_str());
        return false;
    }

    // Get then attributes for the extension
    FileTypeMap::iterator itr = m_Database.find(extension);
    FileTypeAttributesMap attributes = itr->second;

    // Find the type attribute
    FileTypeAttributesMap::iterator aitr = attributes.find("explicit");
    return Platform::StringCompareNoCase(aitr->second, "true");
}
