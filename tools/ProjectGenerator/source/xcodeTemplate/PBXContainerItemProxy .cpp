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

#include "xcodeTemplate/PBXContainerItemProxy.h"
#include "xcodeTemplate/PBXNativeTarget.h"
#include "xcodeTemplate/PBXProject.h"

IMPLEMENT_GLOBAL_LIST(PBXContainerItemProxy)

PBXContainerItemProxy::PBXContainerItemProxy()
{
    m_ContainerPortal = NULL;
    m_RemoteGlobalIDString = NULL;

    m_ProxyType = "1";

    // Add ourselves to the global list
    ADD_TO_GLOBAL_LIST()
}

PBXContainerItemProxy::~PBXContainerItemProxy()
{
}

IMPLEMENT_WRITE_GLOBAL_LIST(PBXContainerItemProxy)
{
    stream << endl << "/* Begin PBXContainerItemProxy section */" << endl;
    U32 size = m_List.size();
    for(U32 i=0; i<size; ++i)
    {
        PBXContainerItemProxy* proxy = m_List[i];

        stream << "\t\t";
        proxy->GetUUID().Write(stream);
        stream << " /* " << proxy->GetISA() << " */";
        stream << " = {" << endl;

        stream << "\t\t\tisa = " << proxy->GetISA() << ";" << endl;

        stream << "\t\t\tcontainerPortal = ";
        proxy->m_ContainerPortal->GetUUID().Write(stream);
        stream << " /* Project object */;" << endl;

        stream << "\t\t\tproxyType = " << proxy->m_ProxyType << ";" << endl;

        stream << "\t\t\tremoteGlobalIDString = ";
        proxy->m_RemoteGlobalIDString->GetUUID().Write(stream);
        stream << ";" << endl;

        stream << "\t\t\tremoteInfo = " << proxy->m_RemoteInfo << ";" << endl;

        stream << "\t\t};" << endl;
    }

    stream << "/* End PBXContainerItemProxy section */" << endl;

    return true;
}
