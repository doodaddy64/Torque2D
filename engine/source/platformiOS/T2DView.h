//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import <GLKit/GLKit.h>

@interface T2DView : GLKView {
}

@property (strong, nonatomic) EAGLContext *context;
@property (nonatomic) CGFloat currentAngle;//for knowing our current oriantion
@property (nonatomic) BOOL isLayedOut;

- (void)swapBuffers;
- (void)rotateByAngle:(CGFloat)angle;//rotate BY a certain degree
- (void)rotateToAngle:(CGFloat)angle;//rotate TO a certain degree
- (void)centerOnPoint:(CGPoint)point;//set the center position


@end
