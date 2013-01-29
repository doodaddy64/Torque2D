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

#include "xcodeTemplate/PBXBuildFile.h"
#include "xcodeTemplate/PBXFileReference.h"

IMPLEMENT_GLOBAL_LIST(PBXBuildFile)

PBXBuildFile::PBXBuildFile()
{
    m_FileReference = NULL;

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXBuildFile::~PBXBuildFile()
{
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXBuildFile)
{
    stream << endl << "/* Begin PBXBuildFile section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXBuildFile* file = m_List[i];

        stream << "\t\t";
        file->GetUUID().Write(stream);
        if(!file->GetName().empty())
        {
            stream << " /* " << file->GetName() << " in " << file->GetBuildPhase() << " */";
        }
        stream << " = {";

        stream << "isa = " << file->GetISA() << ";";

        stream << " fileRef = ";
        file->m_FileReference->GetUUID().Write(stream);
        stream << " /* " << file->m_FileReference->GetName() << " */;";

        stream << " };" << endl;
    }

    stream << "/* End PBXBuildFile section */" << endl;

    return true;
}
