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

#ifndef _ELEMENTREFERENCE_H
#define _ELEMENTREFERENCE_H

#include "platform/types.h"
#include <iostream>
#include <fstream>

using namespace std;

class ElementReference
{
protected:
    U32 m_Reference[3];

public:
    ElementReference()
    {
        Clear();
    }

    virtual ~ElementReference() {}

    void Clear()
    {
        m_Reference[0] = 0;
        m_Reference[1] = 0;
        m_Reference[2] = 0;
    }

    void Set(U32 a, U32 b, U32 c)
    {
        m_Reference[0] = a;
        m_Reference[1] = b;
        m_Reference[2] = c;
    }

    void Write(ofstream& file);
};

#endif  // _ELEMENTREFERENCE_H
