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

#ifndef _PBXFILEREFERENCE_H
#define _PBXFILEREFERENCE_H

#include "xcodeTemplate/PBXBase.h"
#include <string>
#include <vector>

using namespace std;

class PBXFileReference : public PBXBase
{
protected:
    bool m_WriteFileEncoding;

public:
    S32 m_FileEncoding;

    string m_ExplicitFileType;

    string m_LastKnownFileType;

    string m_Path;

    PBXSourceTree m_SourceTree;

public:
    PBXFileReference();
    virtual ~PBXFileReference();

    void ResolveFilePath();
    void ResolveFileType();

    bool GetWriteFileEncoding() { return m_WriteFileEncoding; }
    void SetWriteFileEncoding(bool state) { m_WriteFileEncoding = state; }

    DEFINE_ISA(PBXFileReference)
    DEFINE_GLOBAL_LIST(PBXFileReference)
    DEFINE_WRITE_GLOBAL_LIST(PBXFileReference)
};

#endif  // _PBXFILEREFERENCE_H
