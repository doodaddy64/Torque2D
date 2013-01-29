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

#ifndef _PLATFORMUNICODE_H
#define _PLATFORMUNICODE_H

#include "platform/types.h"

namespace Platform
{
    const U32 convertUTF8toUTF16(const UTF8 *unistring, UTF16 *outbuffer, U32 len);
    const U32 convertUTF16toUTF8( const UTF16 *unistring, UTF8  *outbuffer, U32 len);

    const UTF32  oneUTF8toUTF32( const UTF8 *codepoint,  U32 *unitsWalked = NULL);
    const U32    oneUTF32toUTF8( const UTF32 codepoint, UTF8 *threeByteCodeunitBuf);

    const UTF16  oneUTF32toUTF16(const UTF32 codepoint);
    const UTF32  oneUTF16toUTF32(const UTF16 *codepoint, U32 *unitsWalked = NULL);
}

#endif  // _PLATFORMUNICODE_H
