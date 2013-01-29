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

#ifndef _APPLICATION_H
#define _APPLICATION_H

#include "platform/types.h"
#include <string>

using namespace std;

class Application
{
public:
    enum ProjectTypes {
        PT_Unknown,
        PT_Xcode,
        PT_VS2010,
    };

protected:
    static Application* m_Singleton;

    ProjectTypes m_ProjectType;

    string m_ProjectFileName;

    string m_XcodeProjectFileName;

public:
    Application();
    virtual ~Application();

    void Begin(S32 argc, const char **argv);

    string& GetProjectFileName() { return m_ProjectFileName; }

    static Application* singleton() { return m_Singleton; }
};

#endif  // _APPLICATION_H
