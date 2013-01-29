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

#include "application/Application.h"
#include "xcodeTemplate/FileTypeDatabase.h"
#include "xcodeTemplate/XcodeProject.h"
#include "platform/stringFunctions.h"

Application* Application::m_Singleton = NULL;

Application::Application()
{
    if(!m_Singleton)
        m_Singleton = this;

    m_ProjectType = PT_Unknown;
}

Application::~Application()
{
}

void Application::Begin(S32 argc, const char **argv)
{
    printf("Starting application...\n");

    // There needs to be at least three arguments.  The first indicates
    // the type of project we will produce.  Then we're told what
    // the project file to use is.
    if(argc < 6)
    {
        printf("Application::Begin(): Wrong number of arguments.  Need to include the following switches:\n");
        printf("-xcode\n");
        printf("-project {project_file_path_and_name}\n");
        printf("-output {Xcode_project_path_and_name}\n");
        return;
    }

    // The first argument should always be the project type
    string projectType = argv[1];
    if(Platform::StringCompareNoCase(projectType, string("-xcode")))
    {
        m_ProjectType = PT_Xcode;
    }
    else
    {
        printf("Application::Begin(): Unknown project type.  The first argument needs to be one of the following:\n");
        printf("\t-xcode\n");
        return;
    }

    // Work the rest of the arguments
    for(S32 i=2; i<argc; )
    {
        string arg = argv[i];

        if(Platform::StringCompareNoCase(arg, string("-project")))
        {
            // Check that we have another argument
            ++i;
            if(i < argc)
            {
                m_ProjectFileName = argv[i];
            }
        }

        if(Platform::StringCompareNoCase(arg, string("-output")))
        {
            // Check that we have another argument
            ++i;
            if(i < argc)
            {
                m_XcodeProjectFileName = argv[i];
            }
        }

        ++i;
    }

    // Did we get a project file?
    if(m_ProjectFileName.empty())
    {
        printf("Application::Begin(): Did not receive a project file in a '-project' argument.\n");
        return;
    }

    // Did we get a project file?
    if(m_XcodeProjectFileName.empty())
    {
        printf("Application::Begin(): Did not receive an Xcode project file in a '-output' argument.\n");
        return;
    }

    // Work with an Xcode project
    if(m_ProjectType == PT_Xcode)
    {
        // Create the file type database
        new FileTypeDatabase();

        bool result = FileTypeDatabase::singleton()->BuildDatabase();
        if(!result)
        {
            // Base database.  We must quit.
            printf("Application::Begin(): Bad file type database.  Must quit.\n");
            return;
        }

        // Write out the project file
        XcodeProject* project = new XcodeProject();
        project->CreateProjectFile(m_ProjectFileName, m_XcodeProjectFileName);
        delete project;
    }
}
