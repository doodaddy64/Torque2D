//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import <GLKit/GLKit.h>


@interface T2DViewController : GLKViewController {
	/* The pixel dimensions of the backbuffer */
	GLint backingWidth;
	GLint backingHeight;
	
    /* OpenGL names for the renderbuffer and framebuffers used to render to this view */
	GLuint viewRenderbuffer, viewFramebuffer;
	
	/* OpenGL name for the depth buffer that is attached to viewFramebuffer, if it exists (0 if it does not exist) */
	GLuint depthRenderbuffer;
    
    GLuint _vertexBuffer;
    GLuint _indexBuffer;
    GLuint _vertexArray;
    
    bool isLayedOut;
    
@public
    NSURLConnection *connection;
    NSMutableData *connectionData;
    bool mOrientationLandscapeRightSupported;
    bool mOrientationLandscapeLeftSupported;
    bool mOrientationPortraitSupported;
    bool mOrientationPortraitUpsideDownSupported;
}

@property (strong, nonatomic) EAGLContext *context;

@property (nonatomic, strong) NSURLConnection   *connection;
@property (nonatomic, strong) NSMutableData     *connectionData;

void supportLandscape( bool );
void supportPortrait( bool );
@end
