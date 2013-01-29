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

#include "xcodeTemplate/PBXVariantGroup.h"

IMPLEMENT_GLOBAL_LIST(PBXVariantGroup)

PBXVariantGroup::PBXVariantGroup()
{
    m_SourceTree = ST_RelativeToGroup;

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXVariantGroup::~PBXVariantGroup()
{
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXVariantGroup)
{
    stream << endl << "/* Begin PBXVariantGroup section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXVariantGroup* group = m_List[i];

        stream << "\t\t";
        group->GetUUID().Write(stream);
        if(!group->GetName().empty())
        {
            stream << " /* " << group->GetName() << " */";
        }
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << group->GetISA() << ";" << endl;

        if(group->m_ChildList.size() > 0)
        {
            stream << "\t\t\tchildren = (" << endl;

            for(U32 j=0; j<group->m_ChildList.size(); ++j)
            {
                stream << "\t\t\t\t";
                group->m_ChildList[j]->GetUUID().Write(stream);
                if(!group->m_ChildList[j]->GetName().empty())
                {
                    stream << " /* " << group->m_ChildList[j]->GetName() << " */";
                }
                stream << "," << endl;
            }

            stream << "\t\t\t);" << endl;
        }

        if(!group->m_Name.empty())
        {
            stream << "\t\t\tname = " << group->m_Name << ";" << endl;
        }

        stream << "\t\t\tsourceTree = " << PBXBase::SourceTreeName(group->m_SourceTree) << ";" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End PBXVariantGroup section */" << endl;

    return true;
}
