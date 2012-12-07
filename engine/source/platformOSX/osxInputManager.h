//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#ifndef _OSXINPUT_H_
#define _OSXINPUT_H_

#import "platform/platformInput.h"

class osxInputManager : public InputManager
{
private:

    bool mKeyboardEnabled;
    bool mMouseEnabled;
    
public:

    osxInputManager();

    // InputManager handling
    bool enable();
    void disable();
    void process();

    // Keyboard handling
    void enableKeyboard();
    void disableKeyboard();
    bool isKeyboardEnabled();

    // Mouse handling
    void enableMouse();
    void disableMouse();
    bool isMouseEnabled();
};

#endif