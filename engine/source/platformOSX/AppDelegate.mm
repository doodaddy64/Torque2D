//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import "AppDelegate.h"
#import "platformOSX/platformOSX.h"

@implementation AppDelegate

//-----------------------------------------------------------------------------

- (void)dealloc
{
    [super dealloc];
}

//-----------------------------------------------------------------------------

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    // Parse command line arguments
    const int kMaxCmdlineArgs = 32; // arbitrary
    const char* newArgv[kMaxCmdlineArgs];
    
    NSArray* arguments = [[NSProcessInfo processInfo] arguments];
    
    const char* cwd = Platform::getMainDotCsDir();
    Platform::setCurrentDirectory(cwd);
    
    osxPlatState * platState = [osxPlatState sharedPlatState];
    
    platState.argc = [arguments count];
    
    for (NSUInteger i = 0; i < platState.argc; i++)
    {
        const char* pArg = [[arguments objectAtIndex:i] UTF8String];
        newArgv[i] = pArg;
    }
    
    platState.argv = newArgv;
    
    // With the command line arguments stored, let's run Torque
    [platState runTorque2D];
}

//-----------------------------------------------------------------------------

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
    return YES;
}

//-----------------------------------------------------------------------------

- (void)applicationWillTerminate:(NSNotification *)aNotification
{
    osxPlatState * platState = [osxPlatState sharedPlatState];
    [platState shutDownTorque2D];
}

@end
