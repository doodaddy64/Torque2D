//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#ifndef _OSXOPENGLDEVICE_H_
#define _OSXOPENGLDEVICE_H_

#include "platform/platformVideo.h"

class osxOpenGLDevice : public DisplayDevice
{
private:
    /// Gamma value
    F32 mGamma;
            
    /// Handles to all the devices ( monitors ) attached to the system.
    Vector<CGDirectDisplayID> mMonitorList;
    
    CGDirectDisplayID allDevs[32];
    U32 nAllDevs;
    
    /// Fills mResolutionList with a list of valid resolutions for a particular screen
    bool enumDisplayModes(CGDirectDisplayID hDevice);
    
    /// Fills mMonitorList with all available monitors
    bool enumMonitors();
    
    /// Chooses a monitor based on $pref::the results of enumMontors()
    CGDirectDisplayID chooseMonitor();
    
    osxPlatState * platState;

public:
    osxOpenGLDevice();
    
    static DisplayDevice* create();
    
    /// The following are inherited from DisplayDevice
    void initDevice();
    
    bool cleanUpContext();
    
    bool activate( U32 width, U32 height, U32 bpp, bool fullScreen );
    
    void shutdown();
    
    NSOpenGLPixelFormat* generateValidPixelFormat(bool fullscreen, U32 bpp, U32 samples);
    
    bool setScreenMode( U32 width, U32 height, U32 bpp, bool fullScreen, bool forceIt = false, bool repaint = true );
    
    void swapBuffers();
    
    const char* getDriverInfo();
    
    bool getGammaCorrection(F32 &g);
    
    bool setGammaCorrection(F32 g);
    
    bool setVerticalSync( bool sync );
    
};

#endif // _OSXOPENGLDEVICE_H_
