//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _TORQUECONFIG_H_
#define _TORQUECONFIG_H_

/// Version number is major * 1000 + minor * 100 + revision * 10.
#define TORQUE_GAME_ENGINE          1000

/// What's the name of your game? Used in a variety of places.
// -Mat added iPhone tag
#define TORQUE_GAME_NAME            "Torque 2D MIT"

/// Human readable version string.
#define TORQUE_GAME_VERSION_STRING  "Open Source"

/// Used to suppress unused compiler warnings.
#define TORQUE_UNUSED( arg )


#if defined(TORQUE_OS_IOS)

//PUAP -Mat unless you compile with a custom build step 'IPHONE_OPTIMIZE_OPTIONS' set to '-skip-PNGs', you're PNGs will be altered(optimized)
//So either deal with that yourself or define this so that we load it using apple iPhone functions to get the PNG data to Torque and everything will be peachy
//i.e. if you have 'IPHONE_OPTIMIZE_OPTIONS' set to '-skip-PNGs' in the user-defined build settings you don't need this, otherwise this will load PNGS properly for you
//#define USE_APPLE_OPTIMIZED_PNGS

#ifndef TORQUE_OS_IOS
	#define TORQUE_OS_IOS
#endif

//Defined in project settings
//#define TORQUE_DISABLE_MEMORY_MANAGER

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


/// Define me if you want to enable multithreading support.

//-Mat MAC version has to be multi-threaded, who knew
//-Mat leave PC and iPhone single threaded for now
#ifdef TORQUE_OS_OSX
#define TORQUE_MULTITHREAD
#endif

/// Define me if you want to enable the profiler.
///    See also the TORQUE_SHIPPING block below
//#define TORQUE_ENABLE_PROFILER

#define PROFILE_START(name) TORQUE_UNUSED(#name)
#define PROFILE_END()
#define PROFILE_SCOPE(name) TORQUE_UNUSED(#name)

/// Define me to enable unicode support.
#define TORQUE_UNICODE

/// Define me to enable debug mode; enables a great number of additional
/// sanity checks, as well as making AssertFatal and AssertWarn do something.
/// This is usually defined by the build target.
//#define TORQUE_DEBUG

/// Define me if this is a shipping build; if defined I will instruct Torque
/// to batten down some hatches and generally be more "final game" oriented.
/// Notably this disables a liberal resource manager file searching, and
/// console help strings.
//#define TORQUE_SHIPPING

/// Define me to enable a variety of network debugging aids.
//#define TORQUE_DEBUG_NET

/// Modify me to enable metric gathering code in the renderers.
///
/// 0 does nothing; higher numbers enable higher levels of metric gathering.
//#define TORQUE_GATHER_METRICS 0

/// Define me if you want to enable debug guards in the memory manager.
///
/// Debug guards are known values placed before and after every block of
/// allocated memory. They are checked periodically by Memory::validate(),
/// and if they are modified (indicating an access to memory the app doesn't
/// "own"), an error is flagged (ie, you'll see a crash in the memory
/// manager's validate code). Using this and a debugger, you can track down
/// memory corruption issues quickly.
//#define TORQUE_DEBUG_GUARD

/// Define me if you want to enable debug guards on the FrameAllocator.
/// 
/// This is similar to the above memory manager guards, but applies only to the
/// fast FrameAllocator temporary pool memory allocations. The guards are only
/// checked when the FrameAllocator frees memory (when it's water mark changes).
/// This is most useful for detecting buffer overruns when using FrameTemp<> .
/// A buffer overrun in the FrameAllocator is unlikely to cause a crash, but may
/// still result in unexpected behavior, if other FrameTemp's are stomped.
//#define FRAMEALLOCATOR_DEBUG_GUARD

/// This #define is used by the FrameAllocator to align starting addresses to
/// be byte aligned to this value. This is important on the 360 and possibly
/// on other platforms as well. Use this #define anywhere alignment is needed.
///
/// NOTE: Do not change this value per-platform unless you have a very good
/// reason for doing so. It has the potential to cause inconsistencies in 
/// memory which is allocated and expected to be contiguous.
///
///@ TODO: Make sure that everywhere this should be used, it is being used.
#define TORQUE_BYTE_ALIGNMENT 4


// Finally, we define some dependent #defines. This enables some subsidiary
// functionality to get automatically turned on in certain configurations.

#ifdef TORQUE_DEBUG
//#  define TORQUE_GATHER_METRICS 0
// enable the profiler by default, if we're not doing a release build
//#  define TORQUE_ENABLE_PROFILER
#endif

#ifdef TORQUE_RELEASE
  // If it's not DEBUG, it's a RELEASE build, put appropriate things here.
#  undef TORQUE_ENABLE_PROFILER
#endif

#ifdef TORQUE_LIB
   #ifndef TORQUE_NO_OGGVORBIS
   #define TORQUE_NO_OGGVORBIS
   #endif
#endif

#define DSO_VERSION (U32(Con::DSOVersion))

#endif

