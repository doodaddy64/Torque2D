//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import "platformOSX/osxInputManager.h"
#include "platform/event.h"

@interface OSXTorqueView : NSView
{
    @private
    NSOpenGLContext* _openGLContext;
    osxInputManager* inputManager;
    NSTrackingArea* _trackingArea;

    @public
    BOOL _contextInitialized;
}

@property BOOL contextInitialized;

- (BOOL) acceptsFirstResponder;
- (void) keyDown:(NSEvent *)event;
- (void) initialize;
- (void) createContextWithPixelFormat:(NSOpenGLPixelFormat *)pixelFormat;
- (void) clearContext;
- (void) updateContext;
- (void) flushBuffer;
- (void) setVerticalSync:(bool)sync;

- (void)processMouseButton:(NSEvent *)event button:(KeyCodes)button action:(U8)action;
- (void)processMouseDrag:(NSEvent *)event;
- (void)processKeyEvent:(NSEvent *)event action:(U8)action;
@end