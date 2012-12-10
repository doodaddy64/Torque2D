//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "platformOSX/platformOSX.h"

#include "platformOSX/osxOpenGLDevice.h"


//------------------------------------------------------------------------------
// Get the video settings from the prefs.
static void osxGetInitialResolution(U32 &width, U32 &height, U32 &bpp, bool &fullScreen)
{
    const char* resString;
    char *tempBuf;
    
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    // cache the desktop size of the selected screen in platState
    Video::getDesktopResolution();
    
    // load pref variables, properly choose windowed / fullscreen
#ifndef TORQUE_TOOLS
    fullScreen = Con::getBoolVariable("$pref::Video::fullScreen");
#else
    fullScreen = Con::getBoolVariable("$pref::T2D::fullScreen");
#endif
    
    if (fullScreen)
        resString = Con::getVariable("$pref::Video::resolution");
    else
        resString = Con::getVariable("$pref::Video::windowedRes");
    
    // dStrtok is destructive, work on a copy...
    tempBuf = new char[dStrlen(resString) + 1];
    dStrcpy(tempBuf, resString);
    
    // set window size
    //DAW: Added min size checks for windowSize
    width = (U32)dAtoi(dStrtok(tempBuf, " x\0"));
    
    if (width <= 0)
        width = [platState windowWidth];
    
    height = (U32)dAtoi(dStrtok( NULL, " x\0"));
    
    if (height <= 0)
        height = [platState windowHeight];
    
    // bit depth
    if (fullScreen)
    {
        dAtoi(dStrtok(NULL, "\0"));
        
        if ( bpp <= 0 )
            bpp = 16;
    }
    else
        bpp = platState.desktopBitsPixel == 24 ? 32 : platState.desktopBitsPixel;
    
    delete [] tempBuf;
}

//-----------------------------------------------------------------------------
// This is the critical platform function for initializing graphics. This
// will create a NSWindow, a custom OSXTorqueView, a DisplayDevice, and
// kick off the Video:: work
void Platform::initWindow(const Point2I &initialSize, const char *name)
{
    bool fullScreen;
    U32 width, height, bpp;
    
    osxGetInitialResolution(width, height, bpp, fullScreen);
    
    // Create the NSWindow
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    [platState setWindowSize:initialSize.x height:initialSize.y];
    
    NSRect frame = NSMakeRect(0, 0, [platState windowWidth], [platState windowHeight]);
    
    NSWindow *tempWindow = [[[NSWindow alloc] initWithContentRect:frame
                                              styleMask:NSTitledWindowMask | NSClosableWindowMask | NSResizableWindowMask
                                              backing:NSBackingStoreBuffered
                                              defer:NO] autorelease];
    
    [tempWindow setBackgroundColor:[NSColor blueColor]];

    // The full frame for a window must consider the title bar height as well
    // Thus, our NSWindow must be larger than the passed width and height
    frame = [NSWindow frameRectForContentRect:frame styleMask:NSTitledWindowMask];
    [tempWindow setFrame:frame display:YES];

    [platState setWindow:tempWindow];
    
    [platState updateWindowTitle:name];
    
    // Set up TorqueView and add it here:
    OSXTorqueView* torqueView = [[OSXTorqueView alloc] initWithFrame:frame];
    [torqueView initialize];
    
    [platState setTorqueView:torqueView];
    [[platState window] setContentView:[platState torqueView]];

    [[NSNotificationCenter defaultCenter] addObserver:[platState torqueView] selector:@selector(windowFinishedLiveResize:) name:NSWindowDidEndLiveResizeNotification object:[platState window]];
    
    // Create the DisplayDevice and install it. In this case, our osxOpenGLDevice
    osxOpenGLDevice* device = new osxOpenGLDevice();
    Video::installDevice(device);
    
    // Show the window and all its contents
    [[platState window] makeKeyAndOrderFront:NSApp];
    [[platState window] center];
    
    bool deviceWasSet = Video::setDevice(device->mDeviceName, width, height, bpp, fullScreen);
    
    if (!deviceWasSet)
        AssertFatal(false, "Platform::initWindow could not find a compatible display device!");
}

//-----------------------------------------------------------------------------
// Changes the text in the NSWindow title
void Platform::setWindowTitle( const char* title )
{
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    [platState updateWindowTitle:title];
}

//-----------------------------------------------------------------------------
// Calls osxPlatState::setWindowSize
void Platform::setWindowSize( U32 newWidth, U32 newHeight )
{
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    [platState setWindowSize:newWidth height:newHeight];
}

//-----------------------------------------------------------------------------
// Returns osxPlatState::getWindowSize
const Point2I& Platform::getWindowSize()
{
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    return [platState getWindowSize];
}

//-----------------------------------------------------------------------------
// Simulates the user clicking the minimize button by momentarily highlighting
// the button, then minimizing the window.
void Platform::minimizeWindow()
{
    osxPlatState* platState = [osxPlatState sharedPlatState];

    [[platState window] performMiniaturize:nil];
}

//-----------------------------------------------------------------------------
// De-minimizes the window.
void Platform::restoreWindow()
{
    osxPlatState* platState = [osxPlatState sharedPlatState];

    [[platState window] deminiaturize:nil];
}

//-----------------------------------------------------------------------------
// Sets whether the mouse is locked to the appWindow. Thist changes how the
// mouse input is processed on movement, but does not affect any actual window
// or view properties.
void Platform::setMouseLock(bool locked)
{
    osxPlatState* platState = [osxPlatState sharedPlatState];

    [platState setMouseLocked:YES];
}

//-----------------------------------------------------------------------------
// Launch the default OS browser. This has nothing to do with the QT browser
bool Platform::openWebBrowser( const char* webAddress )
{
    if(Video::isFullScreen())
        Video::toggleFullScreen();

    NSString* convertedAddress = [NSString stringWithUTF8String:webAddress];

    bool result = [[NSWorkspace sharedWorkspace] openURL:[NSURL URLWithString:convertedAddress]];

    if (!result)
        Con::errorf("Platform::openWebBrowser could not open web address:%s", webAddress);

    return result;
}