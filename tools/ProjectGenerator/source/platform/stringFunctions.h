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

#ifndef _STRINGFUNCTIONS_H
#define _STRINGFUNCTIONS_H

#include <string>

using namespace std;

namespace Platform
{
    bool StringCompare(const string& left, const string& right);

    bool StringCompareNoCase(const string& left, const string& right);
    
    int Stricmp(const char *str1, const char *str2);
};

inline bool Platform::StringCompare(const string& left, const string& right)
{
    return (strcmp(left.c_str(), right.c_str()) == 0);
}

inline bool Platform::StringCompareNoCase(const string& left, const string& right)
{
    return (Platform::Stricmp(left.c_str(), right.c_str()) == 0);
}

#endif  // _STRINGFUNCTIONS_H
