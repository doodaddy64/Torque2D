//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _iOSOGLVIDEO_H_
#define _iOSOGLVIDEO_H_

#import "UIKit/UIKit.h"
#import "UIKit/UIAccelerometer.h"

#include "platform/platformVideo.h"

class OpenGLDevice : public DisplayDevice
{
private:
    
    /// Gamma value
    F32 mGamma;
    
    /// Cleans up the opengl context, and destroys the rendering window
    bool cleanupContextAndWindow();
    
public:
    OpenGLDevice();
    static DisplayDevice* create();
    
    /// The following are inherited from DisplayDevice
    void initDevice();
    bool activate( U32 width, U32 height, U32 bpp, bool fullScreen );
    
    void shutdown();
    
    bool setScreenMode( U32 width, U32 height, U32 bpp, bool fullScreen, bool forceIt = false, bool repaint = true );
    void swapBuffers();
    
    const char* getDriverInfo();
    bool getGammaCorrection(F32 &g);
    bool setGammaCorrection(F32 g);
    bool setVerticalSync( bool on );
};

#endif // _iOSOGLVIDEO_H_
