//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "platformOSX/platformOSX.h"
#include "platform/platformSemaphore.h"
#include "platform/platformInput.h"
#include "platform/platformVideo.h"
#include "game/gameInterface.h"

#pragma mark ---- OSXPlatState Implementation ----

@interface osxPlatState (PrivateMethods)
- (void)mainTorqueLoop:(NSTimer *)obj;
@end

@implementation osxPlatState

@synthesize window = _window;
@synthesize torqueView = _torqueView;
@synthesize cgDisplay = _cgDisplay;
@synthesize applicationID = _applicationID;
@synthesize alertSemaphore = _alertSemaphore;
@synthesize fullScreen = _fullscreen;
@synthesize argc = _argc;
@synthesize argv = _argv;
@synthesize platformRandom = _platformRandom;
@synthesize desktopBitsPixel = _desktopBitsPixel;
@synthesize desktopWidth = _desktopWidth;
@synthesize desktopHeight = _desktopHeight;
@synthesize currentSimTime = _currentSimTime;
@synthesize lastTimeTick = _lastTimeTick;
@synthesize sleepTicks = _sleepTicks;
@synthesize mainCSDirectory = _mainCSDirectory;
@synthesize backgrounded = _backgrounded;
@synthesize minimized = _minimized;
@synthesize mouseLocked = _mouseLocked;
@synthesize windowTitle = _windowTitle;
@synthesize quit = _quit;
@synthesize osxTimer = _osxTimer;

static osxPlatState * tempSharedPlatState = nil;

//-----------------------------------------------------------------------------

- (id)init
{
    self = [super init];
    
    if (self)
    {
        // Default window behaviors
        _backgrounded = false;
        _minimized = false;
        _mouseLocked = true;
        _fullscreen = false;
        _quit = false;
        
        // Default window resolution
        _desktopBitsPixel = 32;
        _desktopWidth = 1024;
        _desktopHeight = 768;
        _windowSize.x = 1024;
        _windowSize.y = 768;
        
        _windowTitle = [[NSString alloc] initWithString:@"Torque 2D OS X"];
        
        // Default window
        _window = nil;
        
        // Default system variables
        _currentSimTime = 0;
        _sleepTicks = 0;
        _lastTimeTick = 0;
        _argc = 0;
        _alertSemaphore = Semaphore::createSemaphore(0);
        _platformRandom = new MRandomLCG();
    }
    
    return self;
}

//-----------------------------------------------------------------------------

- (void)dealloc
{
    if (_window)
        [_window release];
    
    if (_windowTitle)
        [_windowTitle release];
    
    if (_mainCSDirectory)
        [_mainCSDirectory release];
    
    if (_platformRandom)
        delete _platformRandom;
    
    [super dealloc];
}

-(void) updateWindowTitle:(const char*)title
{
    _windowTitle = [NSString stringWithFormat:@"%s", title];
    [_window setTitle:_windowTitle];
}

//-----------------------------------------------------------------------------

- (void)setWindowSize:(int)width height:(int)height
{
    NSRect frame = NSMakeRect([_window frame].origin.x, [_window frame].origin.y, width, height);

    frame = [NSWindow frameRectForContentRect:frame styleMask:NSTitledWindowMask];

    [_window setFrame:frame display:YES];
    
    _windowSize.x = width;
    _windowSize.y = height;
    
    [_torqueView updateContext];
}

//-----------------------------------------------------------------------------

-(Point2I&) getWindowSize
{
    return _windowSize;
}

//-----------------------------------------------------------------------------

- (U32)windowWidth
{
    return (U32)_windowSize.x;
}

//-----------------------------------------------------------------------------

- (U32)windowHeight
{
   return (U32)_windowSize.y;
}

#pragma mark ---- Singleton Functions ----

//-----------------------------------------------------------------------------

+ (id)sharedPlatState
{
    @synchronized(self)
    {
        if (tempSharedPlatState == nil)
            tempSharedPlatState = (osxPlatState *) [[super allocWithZone:NULL] init];
    }
    
    return tempSharedPlatState;
}

//-----------------------------------------------------------------------------

+ (id)allocWithZone:(NSZone *)zone
{
    return [[self sharedPlatState] retain];
}

//-----------------------------------------------------------------------------

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

//-----------------------------------------------------------------------------

- (id)retain
{
    return self;
}

//-----------------------------------------------------------------------------

- (unsigned)retainCount
{
    // Denotes an object that cannot be released
    return UINT_MAX;
}

//-----------------------------------------------------------------------------

- (oneway void)release
{
    // never release
}

//-----------------------------------------------------------------------------

- (id)autorelease
{
    return self;
}

#pragma mark ---- Torque 2D Functions ----

//-----------------------------------------------------------------------------
// Initializes the Game and any additional inits you need to perform
- (BOOL) initializeTorque2D
{
    return Game->mainInitialize(_argc, _argv);
}

//-----------------------------------------------------------------------------
// Runs the main Game instance and any other looping calls you need to perform
- (void) mainTorqueLoop:(NSTimer *)obj
{
    if(Game->isRunning())
    {
        Game->mainLoop();
    }
    else
    {
        [NSApp terminate:self];
    }
}

//-----------------------------------------------------------------------------
// Entry function for running Torque 2D
- (void) runTorque2D
{
    BOOL initialResult = [self initializeTorque2D];
    
    if (initialResult)
    {
        // Setting the interval to 1ms to handle Box2D and rendering        
        _osxTimer = [NSTimer scheduledTimerWithTimeInterval:0.001 target:self selector:@selector(mainTorqueLoop:) userInfo:self repeats:YES];
    }
    else
    {
        [NSApp terminate:self];
    }
}

//-----------------------------------------------------------------------------
// Shut down the main Game instancea nd perform any additional cleanup
- (void) shutDownTorque2D
{
    // Shutdown the game
    Game->mainShutdown();
    
    // Perform any platform cleanup
}

@end

#pragma mark ---- Platform Namespace Functions ----

//-----------------------------------------------------------------------------
// Used for initializing the OS X platform code
void Platform::init()
{
    // Set the global script variable $Platform to "macos"
    Con::setVariable("$Platform", "macos");
    
    // Initialize standard libraries (namespaces)
    Video::init();
    Input::init();
    
    // Initialize OS X specific libraries and services
    
    Con::printSeparator();
}

//-----------------------------------------------------------------------------
// OS X processing
void Platform::process()
{
}

//-----------------------------------------------------------------------------
// Shuts down the OS X platform layer code
void Platform::shutdown()
{
    Input::destroy();
    Video::destroy();
}

//-----------------------------------------------------------------------------
// Completely closes and restarts the simulation
void Platform::restartInstance()
{
    // Returns the NSBundle that corresponds to the directory where the current app executable is located.
    NSBundle* mainAppBundle = [NSBundle mainBundle];
    
    // Returns the file URL of the receiver's executable file.
    // Not currently used, but left here for reference
    //NSURL* execURL = [mainAppBundle executableURL];
    
    // Returns the full pathname of the receiver's executable file.
    NSString* execString = [mainAppBundle executablePath];
    
    // Create a mutable string we can build into an executable command
    NSMutableString* mut = [[[NSMutableString alloc] init] autorelease];
    
    // Base string is the executable path
    [mut appendString:execString];
    
    // append ampersand so that we can launch without blocking.
    // encase in quotes so that spaces in the path are accepted.
    [mut insertString:@"\"" atIndex:0];
    [mut appendString:@"\" & "];
    [mut appendString:@"\\0"];
    
    // Convert to a C string
    const char* execCString = [mut UTF8String];
    
    // Echo the command before we run it
    Con::printf("---- %s -----", execCString);
    
    // Run the restart command and hope for the best
    system(execCString);
}

//-----------------------------------------------------------------------------
// Starts the quit process for the main Game instance
void Platform::postQuitMessage(const U32 in_quitVal)
{
    Event quitEvent;
    quitEvent.type = QuitEventType;
    
    Game->postEvent(quitEvent);
}

//-----------------------------------------------------------------------------
// Kicks off the termination process for the NSApp
void Platform::forceShutdown(S32 returnValue)
{
    // Note: AppCode states terminate is deprecated, which is not true
    [NSApp performSelector:@selector(terminate:) withObject:nil afterDelay:0.0];
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::debugBreak not yet implemented")
void Platform::debugBreak()
{
    raise(SIGTRAP);
}
