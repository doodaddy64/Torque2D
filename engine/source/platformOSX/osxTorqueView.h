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
    NSOpenGLContext *_openGLContext;
    osxInputManager *inputManager;
    NSTrackingArea *_trackingArea;

@public
    BOOL _contextInitialized;
}

@property BOOL contextInitialized;

- (BOOL)acceptsFirstResponder;
- (void)initialize;
- (void)createContextWithPixelFormat:(NSOpenGLPixelFormat *)pixelFormat;
- (void)clearContext;
- (void)updateContext;
- (void)flushBuffer;
- (void)setVerticalSync:(bool)sync;

@end