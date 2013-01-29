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

#include "xcodeTemplate/PBXHeadersBuildPhase.h"
#include "xcodeTemplate/PBXBuildFile.h"

IMPLEMENT_GLOBAL_LIST(PBXHeadersBuildPhase)

PBXHeadersBuildPhase::PBXHeadersBuildPhase()
{
    SetName("Headers");

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXHeadersBuildPhase::~PBXHeadersBuildPhase()
{
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXHeadersBuildPhase)
{
    stream << endl << "/* Begin PBXHeadersBuildPhase section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXHeadersBuildPhase* phase = m_List[i];

        stream << "\t\t";
        phase->GetUUID().Write(stream);
        if(!phase->GetName().empty())
        {
            stream << " /* " << phase->GetName() << " */";
        }
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << phase->GetISA() << ";" << endl;
        stream << "\t\t\tbuildActionMask = " << phase->m_BuildActionMask << ";" << endl;

        stream << "\t\t\tfiles = (" << endl;
        for(U32 j=0; j<phase->m_Files.size(); ++j)
        {
            stream << "\t\t\t\t";
            phase->m_Files[j]->GetUUID().Write(stream);
            if(!phase->m_Files[j]->GetName().empty())
            {
                stream << " /* " << phase->m_Files[j]->GetName() << " in " << phase->GetName() << " */";
            }
            stream << "," << endl;
        }
        stream << "\t\t\t);" << endl;

        stream << "\t\t\trunOnlyForDeploymentPostprocessing = " << phase->m_RunOnlyForDeploymentPostprocessing << ";" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End PBXHeadersBuildPhase section */" << endl;

    return true;
}
