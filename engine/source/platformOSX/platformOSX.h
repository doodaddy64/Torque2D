//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import <AppKit/AppKit.h>
#import "platformOSX/osxTorqueView.h"
#include "platform/platform.h"
#include "platform/types.h"
#include "math/mPoint.h"
#include "math/mRandom.h"

@interface osxPlatState : NSObject
{
    // Main application Window
    NSWindow* _window;
    
    // Main view for Torque 2D engine display
    OSXTorqueView* _torqueView;
    
    // Core graphics display ID the app will start with
    CGDirectDisplayID _cgDisplay;

    // Process ID for this application instance
    id _applicationID;
    
    // Version of operating system
    U32 osVersion;
    
    // Number of arguments passed into this application
    U32 _argc;
    
    // Arguments passed into this application
    const char** _argv;
    
    Point2I _windowSize;
    
    // Bit depth of the screen (desktop)
    U32 _desktopBitsPixel;
    
    // Horizontal resolution of user's desktop
    U32 _desktopWidth;
    
    // Vertical resolution of user's desktop
    U32 _desktopHeight;
    
    // Time sync from the desktop
    U32 _currentSimTime;
    
    U32 _lastTimeTick;
    
    U32 _sleepTicks;
    
    // Location of the folder containing the main.cs
    NSString* _mainCSDirectory;
    
    // Title for the main window
    NSString* _windowTitle;
    
    // Threaded alert object
    void* _alertSemaphore;
    
    // Random generator
    RandomLCG* _platformRandom;
    
    // Used to report is mouse is locked to the main window or not
    BOOL _mouseLocked;
    
    // Used to report if the window has been pushed to the background or not
    BOOL _backgrounded;
    
    // Use to report if the window has been minimized or not
    BOOL _minimized;
    
    // Used to report windowed vs fullscreen status
    BOOL _fullscreen;
    
    // Reports the quit state for the applications
    BOOL _quit;
    
    // Timer
    NSTimer* _osxTimer;
}

@property (strong) NSWindow* window;
@property (strong) OSXTorqueView* torqueView;
@property CGDirectDisplayID cgDisplay;
@property (strong) id applicationID;
@property void* alertSemaphore;
@property RandomLCG* platformRandom;
@property BOOL fullScreen;
@property U32 argc;
@property const char** argv;
@property U32 desktopBitsPixel;
@property U32 desktopWidth;
@property U32 desktopHeight;
@property U32 currentSimTime;
@property U32 lastTimeTick;
@property U32 sleepTicks;
@property (nonatomic,retain) NSString* mainCSDirectory;
@property (nonatomic,retain) NSString* windowTitle;
@property BOOL mouseLocked;
@property BOOL backgrounded;
@property BOOL minimized;
@property BOOL quit;
@property (strong)NSTimer* osxTimer;

+ (id)sharedPlatState;

- (BOOL)initializeTorque2D;
- (void)runTorque2D;
- (void)shutDownTorque2D;

- (void)updateWindowTitle:(const char*)title;
- (void)setWindowSize:(int)width height:(int)height;
- (Point2I&)getWindowSize;
- (U32)windowWidth;
- (U32)windowHeight;

@end