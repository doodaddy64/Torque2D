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

#include "xcodeTemplate/PBXTargetDependency.h"
#include "xcodeTemplate/PBXNativeTarget.h"
#include "xcodeTemplate/PBXContainerItemProxy.h"

IMPLEMENT_GLOBAL_LIST(PBXTargetDependency)

PBXTargetDependency::PBXTargetDependency()
{
    m_Target = NULL;
    m_TargetProxy = NULL;

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXTargetDependency::~PBXTargetDependency()
{
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXTargetDependency)
{
    stream << endl << "/* Begin PBXTargetDependency section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXTargetDependency* target = m_List[i];

        stream << "\t\t";
        target->GetUUID().Write(stream);
        stream << " /* " << target->GetISA() << " */";
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << target->GetISA() << ";" << endl;

        stream << "\t\t\ttarget = ";
        target->m_Target->GetUUID().Write(stream);
        stream << " /* " << target->m_Target->GetName() << " */;" << endl;
        
        stream << "\t\t\ttargetProxy = ";
        target->m_TargetProxy->GetUUID().Write(stream);
        stream << " /* PBXContainerItemProxy */;" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End PBXTargetDependency section */" << endl;

    return true;
}
