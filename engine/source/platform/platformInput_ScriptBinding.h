//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
#import "console/console.h"
#import "platform/platformInput.h"

//------------------------------------------------------------------------------
ConsoleFunction( activateKeyboard, bool, 1, 1, "() Use the activateKeyboard function to enable directInput polling of the keyboard.\n"
																"@return Returns a true if polling was successfully enabled.\n"
																"@sa deactivateKeyboard")
{
	return Input::activateKeyboard();
}

//------------------------------------------------------------------------------
ConsoleFunction( deactivateKeyboard, void, 1, 1, "() Use the deactivateKeyboard function to disable directInput polling of the keyboard.\n"
																"@return No return value.\n"
																"@sa activateKeyboard")
{
	Input::deactivateKeyboard();
}

//------------------------------------------------------------------------------
ConsoleFunction( enableMouse, bool, 1, 1, "() Use the enableMouse function to enable mouse input.\n"
																"@return Returns true if a mouse is present and it was enabled, false otherwise.\n"
																"@sa disableMouse")
{
	Input::enableMouse();
}

//------------------------------------------------------------------------------
ConsoleFunction( disableMouse, void, 1, 1, "() Use the disableMouse function to disable mouse input.\n"
																"@return No return value.\n"
																"@sa enableMouse")
{
	Input::disableMouse();
}

//------------------------------------------------------------------------------
ConsoleFunction( enableJoystick, bool, 1, 1, "() Use the enableJoystick function to enable joystick input if it is present.\n"
																"@return Will return true if the joystick is present and was successfully enabled, false otherwise.\n"
																"@sa disableJoystick, getJoystickAxes, isJoystickDetected")
{
	return Input::enableJoystick();
}

//------------------------------------------------------------------------------
ConsoleFunction( disableJoystick, void, 1, 1, "() Use the disableJoystick function to disable joystick input.\n"
																"@return No return value.\n"
																"@sa enableJoystick, getJoystickAxes, isJoystickEnabled")
{
	Input::disableJoystick();
}

//------------------------------------------------------------------------------
ConsoleFunction( echoInputState, void, 1, 1, "() Use the echoInputState function to dump the input state of the mouse, keyboard, and joystick to the console.\n"
																"@return No return value.\n"
																"@sa activateDirectInput, deactivateDirectInput, activateKeyboard, deactivateKeyboard, disableJoystick, enableJoystick, enableMouse, disableMouse")
{
	Input::echoInputState();
}
