//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#import "platformOSX/osxInputManager.h"

#pragma mark ---- InputManager Handling ----

//--------------------------------------------------------------------------
osxInputManager::osxInputManager()
{
    mKeyboardEnabled = false;
    mMouseEnabled = false;
    mEnabled = false;
}

//--------------------------------------------------------------------------
bool osxInputManager::enable()
{
    mEnabled = true;
    return mEnabled;
}

//--------------------------------------------------------------------------
void osxInputManager::disable()
{
    mEnabled = false;
}

//--------------------------------------------------------------------------
void osxInputManager::process()
{
    // Nothing to process here, yet
}

#pragma mark ---- Keyboard Handling Functions ----

//--------------------------------------------------------------------------
void osxInputManager::enableKeyboard()
{
    if (!mEnabled)
        mEnabled = true;

    mKeyboardEnabled = true;
}

//--------------------------------------------------------------------------
void osxInputManager::disableKeyboard()
{
    mKeyboardEnabled = false;
}

//--------------------------------------------------------------------------
bool osxInputManager::isKeyboardEnabled()
{
    if (mEnabled)
        return mKeyboardEnabled;

    return false;
}

#pragma mark ---- Mouse Handling Functions ----

//--------------------------------------------------------------------------
void osxInputManager::enableMouse()
{
    if (!mEnabled)
        mEnabled = true;

    mMouseEnabled = true;
}

//--------------------------------------------------------------------------
void osxInputManager::disableMouse()
{
    mMouseEnabled = false;
}

//--------------------------------------------------------------------------
bool osxInputManager::isMouseEnabled()
{
    if (mEnabled)
        return mMouseEnabled;

    return false;
}

