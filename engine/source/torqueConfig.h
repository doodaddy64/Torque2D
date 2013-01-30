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

#ifndef _TORQUECONFIG_H_
#define _TORQUECONFIG_H_

//-----------------------------------------------------------------------------

/// Version number is major * 1000 + minor * 100 + revision * 10.
#define TORQUE_GAME_ENGINE          1000

/// What's the name of your game? Used in a variety of places.
#define TORQUE_GAME_NAME            "Torque 2D MIT"

/// Human readable version string.
#define TORQUE_GAME_VERSION_STRING  "Open Source"

//-----------------------------------------------------------------------------

/// DSO version.
#define DSO_VERSION (U32(Con::DSOVersion))

//-----------------------------------------------------------------------------

/// Used to suppress unused compiler warnings.
#define TORQUE_UNUSED( arg )

//-----------------------------------------------------------------------------

/// Profiler macros used when the profiler is disabled.
#define PROFILE_START(name) TORQUE_UNUSED(#name)
#define PROFILE_END()
#define PROFILE_SCOPE(name) TORQUE_UNUSED(#name)

//-----------------------------------------------------------------------------

/// OSX version has to be multi-threaded.
/// Win32 & iOS have to be single-threaded.
#ifdef TORQUE_OS_OSX
#define TORQUE_MULTITHREAD
#endif

//-----------------------------------------------------------------------------

/// The following pre-processor definitions should be added to your compiler command-line
/// if you required their respective functionality:
///
/// 'TORQUE_DEBUG'
/// Should be associated with a compiler un-optimized build.  Typically produces more
/// diagnostic engine output such as warnings/errors to the console.
///
/// 'TORQUE_RELEASE'
/// Should be associated with a compiler optimized build.  Typically produces less
/// diagnostic engine output than the 'TORQUE_DEBUG' build.
///
/// 'TORQUE_SHIPPING'
/// Should be associated with a compiler optimized build that you intend to ship.
/// Typically produces only the output that is useful (and available) for the
/// end-user to report.
///
/// 'TORQUE_UNICODE'
/// When defined, enables unicode support.
///
/// 'TORQUE_ENABLE_PROFILER'
/// When defined, Torque will capture performance profiling information that sacrifices
/// a small performance overhead to gain significant diagnostics information.
///
/// 'TORQUE_DEBUG_NET'
/// When defined, Torque will enabled certain features that enabled diagnostics of
/// its networking sub-system.
///
/// 'TORQUE_GATHER_METRICS'
/// When defined, Torque will gather additional performance metrics.
///


#if defined(TORQUE_OS_IOS)

//PUAP -Mat unless you compile with a custom build step 'IPHONE_OPTIMIZE_OPTIONS' set to '-skip-PNGs', you're PNGs will be altered(optimized)
//So either deal with that yourself or define this so that we load it using apple iPhone functions to get the PNG data to Torque and everything will be peachy
//i.e. if you have 'IPHONE_OPTIMIZE_OPTIONS' set to '-skip-PNGs' in the user-defined build settings you don't need this, otherwise this will load PNGS properly for you
//#define USE_APPLE_OPTIMIZED_PNGS

#ifndef TORQUE_OS_IOS
	#define TORQUE_OS_IOS
#endif

// help out the shallow matrix stacks...
#define glPushMatrix iPhoneGLPushMatrix
#define glPopMatrix iPhoneGLPopMatrix
#define glMatrixMode iPhoneGLMatrixMode
/* EAGL and GL functions calling wrappers that log on error */
#define CALL_EAGL_FUNCTION(__FUNC__, ...) ({ EAGLError __error = __FUNC__( __VA_ARGS__ ); if(__error != kEAGLErrorSuccess) Con::printf("%s() called from %s returned error %i\n", #__FUNC__, __FUNCTION__, __error); (__error ? 0 : 1); })
#define CHECK_GL_ERROR() ({ int __error = glGetError(); if(__error) Con::printf("OpenGL error 0x%04X in %s\n", __error, __FUNCTION__); (__error ? 0 : 1); })
// define this to print out glErrors, un-define to get rid of it
#define TEST_FOR_OPENGL_ERRORS CHECK_GL_ERROR();
#endif

#endif

