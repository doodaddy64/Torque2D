//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "platformOSX/platformOSX.h"
#import "platformOSX/osxOpenGLDevice.h"
#import "platformOSX/platformGL.h"
#include "console/console.h"
#include "game/gameInterface.h"
#include "graphics/dgl.h"

//-----------------------------------------------------------------------------

osxOpenGLDevice::osxOpenGLDevice()
{
    mDeviceName = "OpenGL";
    mFullScreenOnly = false;

    // pick a monitor to run on
    enumMonitors();
    
    platState = [osxPlatState sharedPlatState];
    
    CGDirectDisplayID display = chooseMonitor();
    
    [platState setCgDisplay:display];
    
    enumDisplayModes(display);
}

//------------------------------------------------------------------------------
//  Fill Vector<Resolution> mResoultionList with list of supported modes
bool osxOpenGLDevice::enumDisplayModes(CGDirectDisplayID display)
{
    mResolutionList.clear();
    
    // get the display, and the list of all available modes.
    CFArrayRef modeArray = CGDisplayCopyAllDisplayModes(display, NULL);
    
    int len = CFArrayGetCount(modeArray);
    
    for(int i = 0; i < len; i++)
    {
        CGDisplayModeRef mode;
        CFStringRef pixelEncoding;
        
        mode = (CGDisplayModeRef)CFArrayGetValueAtIndex(modeArray, i);
        
        // get this mode.
        int width, height, bpp;
        
        // get width
        width = CGDisplayModeGetWidth(mode);
        
        // get height
        height = CGDisplayModeGetHeight(mode);
        
        // get bpp
        pixelEncoding = CGDisplayModeCopyPixelEncoding(mode);
        
        bpp = CFStringGetIntValue(pixelEncoding);
        
        // add to the list
        if (bpp != 8)
        {
            Resolution newRes(width, height, bpp);
            mResolutionList.push_back(newRes);
        }
    }
    
    return true;
}

//-----------------------------------------------------------------------------
// Unused for new OS X platform. The constructor handles initialization now
void osxOpenGLDevice::initDevice()
{
}

//-----------------------------------------------------------------------------
// This will fully clear the OpenGL context
bool osxOpenGLDevice::cleanUpContext()
{
    bool needResurrect = false;
    
    platState = [osxPlatState sharedPlatState];
    
    if ([[platState torqueView] contextInitialized])
    {
        if (!Video::smNeedResurrect)
        {
            Con::printf( "Killing the texture manager..." );
            Game->textureKill();
            needResurrect = true;
        }
        
        [[platState torqueView] clearContext];
    }
    
    // clear the Resolution state, so setScreenMode() will know not to early-out.
    smCurrentRes = Resolution(0, 0, 0);
    
    return needResurrect;
}

//-----------------------------------------------------------------------------
// 
bool osxOpenGLDevice::activate( U32 width, U32 height, U32 bpp, bool fullScreen )
{
    Con::printf( " OpenGLDevice activating..." );
        
    // gets opengl rendering capabilities of the screen pointed to by platState.hDisplay
    // sets up dgl with the capabilities info, & reports opengl status.
    getGLCapabilities();
    
    // Create the window or capture fullscreen
    if(!setScreenMode(width, height, bpp, fullScreen, true, false))
        return false;
    
    // set the displayDevice pref to "OpenGL"
    Con::setVariable( "$pref::Video::displayDevice", mDeviceName );
    
    // set vertical sync now because it doesnt need setting every time we setScreenMode()
    setVerticalSync( !Con::getBoolVariable( "$pref::Video::disableVerticalSync" ));

    return true;
}

//-----------------------------------------------------------------------------

void osxOpenGLDevice::shutdown()
{
    Con::printf( "Shutting down the OpenGL display device..." );
    cleanUpContext();
}

//-----------------------------------------------------------------------------

NSOpenGLPixelFormat* osxOpenGLDevice::generateValidPixelFormat(bool fullscreen, U32 bpp, U32 samples)
{
    AssertWarn(bpp==16 || bpp==32 || bpp==0, "An unusual bit depth was requested in findValidPixelFormat(). clamping to 16|32");
    
    if (bpp)
        bpp = bpp > 16 ? 32 : 16;
    
    AssertWarn(samples <= 6, "An unusual multisample depth was requested in findValidPixelFormat(). clamping to 0...6");
    
    samples = samples > 6 ? 6 : samples;
    
    int i = 0;
    NSOpenGLPixelFormatAttribute attr[64];
    
    attr[i++] = NSOpenGLPFADoubleBuffer;
    attr[i++] = NSOpenGLPFANoRecovery;
    attr[i++] = NSOpenGLPFAAccelerated;
    
    if (fullscreen)
        attr[i++] = NSOpenGLPFAFullScreen;
    
    if(bpp != 0)
    {
        // native pixel formats are argb 1555 & argb 8888.
        U32 colorbits = 0;
        U32 alphabits = 0;
        
        if(bpp == 16)
        {
            colorbits = 5;             // ARGB 1555
            alphabits = 1;
        }
        else if(bpp == 32)
            colorbits = alphabits = 8; // ARGB 8888
        
        attr[i++] = NSOpenGLPFADepthSize;
        attr[i++] = (NSOpenGLPixelFormatAttribute)bpp;
        attr[i++] = NSOpenGLPFAColorSize;
        attr[i++] = (NSOpenGLPixelFormatAttribute)colorbits;
        attr[i++] = NSOpenGLPFAAlphaSize;
        attr[i++] = (NSOpenGLPixelFormatAttribute)alphabits;
    }
    
    if (samples != 0)
    {
        attr[i++] = NSOpenGLPFAMultisample;
        attr[i++] = (NSOpenGLPixelFormatAttribute)1;
        attr[i++] = NSOpenGLPFASamples;
        attr[i++] = (NSOpenGLPixelFormatAttribute)samples;
    }
    
    attr[i++] = 0;
 
    NSOpenGLPixelFormat* format = [[[NSOpenGLPixelFormat alloc] initWithAttributes:attr] autorelease];
    
    return format;
}

//-----------------------------------------------------------------------------

bool osxOpenGLDevice::setScreenMode( U32 width, U32 height, U32 bpp, bool fullScreen, bool forceIt, bool repaint )
{
    // Print to the console that we are setting the screen mode
    Con::printf(" set screen mode %i x %i x %i, %s, %s, %s",width, height, bpp,
                fullScreen  ? "fullscreen" : "windowed",
                forceIt     ? "force it" : "dont force it",
                repaint     ? "repaint"  : "dont repaint");
 
    bool needResurrect = cleanUpContext();
    
    // Get the global OSX platform state
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    // Validation, early outs
    // Sanity check. Some scripts are liable to pass in bad values.
    if (!bpp)
        bpp = [platState desktopBitsPixel];
    
    Resolution newRes = Resolution(width, height, bpp);
    
    // If no values changing and we're not forcing a change, kick out. prevents thrashing.
    if (!forceIt && smIsFullScreen == fullScreen && smCurrentRes == newRes)
        return true;
    
    // Create a pixel format to be used with the context
    NSOpenGLPixelFormat* pixelFormat = generateValidPixelFormat(fullScreen, bpp, 0);
    
    if (!pixelFormat)
    {
        Con::printf("osxOpenGLDevice::setScreenMode error: No OpenGL pixel format");
        return false;
    }
    
    if (fullScreen)
    {
        NSRect mainDisplayRect = [[NSScreen mainScreen] frame];
        
        newRes.w = mainDisplayRect.size.width;
        newRes.h = mainDisplayRect.size.height;
        
        [[platState window] setFrame:mainDisplayRect display:YES];
        
        [[platState window] setLevel:NSMainMenuWindowLevel+1];

        [[platState torqueView] setFrame:mainDisplayRect];
    }
    else
    {
        [platState setWindowSize:newRes.w height:newRes.h];
    }
    
    [[platState torqueView] createContextWithPixelFormat:pixelFormat];
    
    // clear out garbage from the gl window.
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT );
    
    // set opengl options & other options ---------------------------------------
    // ensure data is packed tightly in memory. this defaults to 4.
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    // TODO: set gl arb multisample enable & hint
    //dglSetFSAASamples(gFSAASamples);
        
    // update smIsFullScreen and pref
    smIsFullScreen = fullScreen;
    
    Con::setBoolVariable( "$pref::Video::fullScreen", smIsFullScreen );
    
    // save resolution
    smCurrentRes = newRes;
    
    // save resolution to prefs
    char buf[32];
    if (fullScreen)
    {
        dSprintf( buf, sizeof(buf), "%d %d %d", newRes.w, newRes.h, newRes.bpp);
        Con::setVariable("$pref::Video::resolution", buf);
    }
    else
    {
        dSprintf( buf, sizeof(buf), "%d %d", newRes.w, newRes.h);
        Con::setVariable("$pref::Video::windowedRes", buf);
    }
    
    if (needResurrect)
    {
        // Reload the textures gl names
        Con::printf( "Resurrecting the texture manager..." );
        Game->textureResurrect();
    }
    
    if( repaint )
        Con::evaluate( "resetCanvas();" );
    
    return true;
}

//-----------------------------------------------------------------------------

void osxOpenGLDevice::swapBuffers()
{
    if ([[platState torqueView] contextInitialized])
        [[platState torqueView] flushBuffer];
    
#if defined(TORQUE_DEBUG)
    if (gOutlineEnabled)
        glClear(GL_COLOR_BUFFER_BIT);
#endif
    
}

//-----------------------------------------------------------------------------

const char* osxOpenGLDevice::getDriverInfo()
{
    // Prepare some driver info for the console:
    const char* vendorString   = (const char*) glGetString( GL_VENDOR );
    const char* rendererString = (const char*) glGetString( GL_RENDERER );
    const char* versionString  = (const char*) glGetString( GL_VERSION );
    const char* extensionsString = (const char*) glGetString( GL_EXTENSIONS );
    
    U32 bufferLen = ( vendorString ? dStrlen( vendorString ) : 0 )
    + ( rendererString ? dStrlen( rendererString ) : 0 )
    + ( versionString  ? dStrlen( versionString ) : 0 )
    + ( extensionsString ? dStrlen( extensionsString ) : 0 )
    + 4;
    
    char* returnString = Con::getReturnBuffer( bufferLen );
    dSprintf( returnString, bufferLen, "%s\t%s\t%s\t%s",
             ( vendorString ? vendorString : "" ),
             ( rendererString ? rendererString : "" ),
             ( versionString ? versionString : "" ),
             ( extensionsString ? extensionsString : "" ) );
    
    return( returnString );
}

//-----------------------------------------------------------------------------
#pragma message ("osxOpenGLDevice::getGammaCorrection not yet implemented")
bool osxOpenGLDevice::getGammaCorrection(F32 &g)
{
    
}

//-----------------------------------------------------------------------------
#pragma message ("osxOpenGLDevice::setGammaCorrection not yet implemented")
bool osxOpenGLDevice::setGammaCorrection(F32 g)
{
    
}

//-----------------------------------------------------------------------------

bool osxOpenGLDevice::setVerticalSync( bool sync )
{
    if ([[platState torqueView] contextInitialized])
        [[platState torqueView] setVerticalSync:sync];
}

//------------------------------------------------------------------------------
//  Fill mMonitorList with list of supported modes
//   Guaranteed to include at least the main device.
//------------------------------------------------------------------------------
bool osxOpenGLDevice::enumMonitors()
{
    mMonitorList.clear();
    nAllDevs = 0;
    
    CGDirectDisplayID _displayIDs[32];
    uint32_t _displayCount;
    
    CGGetActiveDisplayList (32, _displayIDs, &_displayCount);
    
    for (int ii = 0 ; ii < _displayCount ; ii++)
    {
        mMonitorList.push_back(_displayIDs[ii]);
        allDevs[nAllDevs++] = _displayIDs[ii];
    }
    
    return true;
}

//------------------------------------------------------------------------------
// Chooses a monitor based on $pref, on the results of tors(), & on the
// current window's screen.
//------------------------------------------------------------------------------
CGDirectDisplayID osxOpenGLDevice::chooseMonitor()
{
    // TODO: choose monitor based on which one contains most of the window.
    // NOTE: do not call cleanup before calling choose, or we won't have a window to consider.
    AssertFatal(!mMonitorList.empty(), "Cannot choose a monitor if the list is empty!");
    
    U32 monNum = Con::getIntVariable("$pref::Video::monitorNum", 0);
    
    if (monNum >= mMonitorList.size())
    {
        Con::errorf("invalid monitor number %i", monNum);
        monNum = 0;
        Con::setIntVariable("$pref::Video::monitorNum", 0);
    }
    
    Con::printf("using display 0x%x", mMonitorList[monNum]);
    
    return mMonitorList[monNum];
}