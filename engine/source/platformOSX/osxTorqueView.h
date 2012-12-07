//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

#import <Cocoa/Cocoa.h>
#import "platformOSX/osxInputManager.h"

@interface OSXTorqueView : NSView
{
    @private
    NSOpenGLContext* _openGLContext;
    NSPoint _lastDragLocation;
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
@end