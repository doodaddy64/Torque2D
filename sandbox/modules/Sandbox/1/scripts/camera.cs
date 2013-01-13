//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$sandboxAutoCamera = true;
$sandboxZoomSpeed = 0.02;

//-----------------------------------------------------------------------------

function setSandboxAutoCamera( %state )
{
    $sandboxAutoCamera = %state;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Finish if the sandbox auto-camera is off.
    if ( !$sandboxAutoCamera )
        return;
    
    // Set touch event.
    $touchEvent[%touchID] = %worldPos;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchUp(%this, %touchID, %worldPos)
{
    // Finish if the sandbox auto-camera is off.
    if ( !$sandboxAutoCamera )
        return;

    // Reset touch event.
    $touchEvent[%touchID] = "";
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchMoved(%this, %touchID, %worldPos)
{
    // Finish if the sandbox auto-camera is off.
    if ( !$sandboxAutoCamera )
        return;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDragged(%this, %touchID, %worldPos)
{
    // Finish if the sandbox auto-camera is off.
    if ( !$sandboxAutoCamera )
        return;

    // Fetch touch event.
    %lastWorldPos = $touchEvent[%touchID];

    // Calculate touch delta.
    %offsetX = getWord(%worldPos, 0) - getWord(%lastWorldPos, 0);
    %offsetY = getWord(%worldPos, 1) - getWord(%lastWorldPos, 1);
    %touchDelta = %offsetX SPC %offsetY;
   
    // Update touch event (offset by our adjustment).
    %offsetWorldPosX = getWord(%worldPos, 0) - getWord(%touchDelta, 0);
    %offsetWorldPosY = getWord(%worldPos, 1) - getWord(%touchDelta, 1);
    $touchEvent[%touchID] = %offsetWorldPosX SPC %offsetWorldPosY;    
            
    // Update the camera.
    %cameraPosition = SandboxWindow.getCurrentCameraPosition();    
    SandboxWindow.setCurrentCameraPosition( getWord(%cameraPosition,0) - getWord(%touchDelta,0), getWord(%cameraPosition,1) - getWord(%touchDelta,1) );
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelUp(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the sandbox auto-camera is off.
    if ( !$sandboxAutoCamera )
        return;
        
    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() + $sandboxZoomSpeed );
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the sandbox auto-camera is off.
    if ( !$sandboxAutoCamera )
        return;

    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() - $sandboxZoomSpeed );            
}
