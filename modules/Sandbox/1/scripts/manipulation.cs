//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Sandbox touch-drag modes are:
// - Off
// - Camera
// - Pull
$sandboxDragMode = "off";

// Reset the sandbox pull object.
$sandboxPullObject = "";
$sandboxPullJointId = "";

//-----------------------------------------------------------------------------

GlobalActionMap.bind( keyboard, "space", setNextDragMode );

//-----------------------------------------------------------------------------

function resetSandboxDragModes()
{   
    // These control which drag modes are available or not.
    $sandboxDragModeAvailable["off"] = true;
    $sandboxDragModeAvailable["camera"] = true;
    $sandboxDragModeAvailable["pull"] = true;
    
    // Set the sandbox drag mode default.
    setSandboxDragMode( "camera" ); 
}

//-----------------------------------------------------------------------------

function setSandboxDragMode( %mode )
{
    // Is the drag mode available?
    if ( !$sandboxDragModeAvailable[%mode] )
    {
        // No, so warn.
        error( "Cannot set sandbox drag mode to " @ %mode @ " as it is currently disabled." );
        return;
    }
    
    $sandboxDragMode = %mode;
    
    // Reset pulled object and joint.
    $sandboxPullObject = "";    
    if ( $sandboxPullJointId !$= "" && SandboxScene.isJoint($sandboxPullJointId) )
    {
        SandboxScene.deleteJoint( $sandboxPullJointId );
        $sandboxPullJointId = "";
    }        
}

//-----------------------------------------------------------------------------

function setNextDragMode( %make )
{
    // Finish if being released.
    if ( !%make )
        return;
    
    // Finish if the drag mode is off.
    if ( $sandboxDragMode $= "off" )
        return;

    // "camera" to "pull" transition.
    if ( $sandboxDragMode $= "camera" )
    {
        if ( $sandboxDragModeAvailable["pull"] )
            setSandboxDragMode("pull");
            
        return;
    }

    // "pull" to "camera" transition.
    if ( $sandboxDragMode $= "pull" )
    {
        if ( $sandboxDragModeAvailable["camera"] )
            setSandboxDragMode("camera");
            
        return;
    }    
}

//-----------------------------------------------------------------------------

function setSandboxDragModeAvailable( %mode, %status )
{
    $sandboxDragModeAvailable[%mode] = %status;    
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( $sandboxDragMode $= "off" )
        return;
    
    // Set touch event.
    $touchEvent[%touchID] = %worldPos;
       
    // Handle "pull" mode.
    if ( $sandboxDragMode $= "pull" )
    {
        // Pick an object.
        %picked = SandboxScene.pickPoint( %worldPos );
        
        // Finish if nothing picked.
        if ( %picked $= "" )
            return;
            
        // Fetch the first object only.
        $sandboxPullObject = getWord( %picked, 0 );
        
        // Create the target joint.
        $sandboxPullJointId = SandboxScene.createTargetJoint( $sandboxPullObject, %worldPos, 1000 );
    }    
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchUp(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( $sandboxDragMode $= "off" )
        return;

    // Reset touch event.
    $touchEvent[%touchID] = "";
    
    // Handle "pull" mode.
    if ( $sandboxDragMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject($sandboxPullObject) )
            return;
        
        // Remove the pull joint.
        SandboxScene.deleteJoint( $sandboxPullJointId );
        $sandboxPullJointId = "";
    }      
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchMoved(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( $sandboxDragMode $= "off" )
        return;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDragged(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( $sandboxDragMode $= "off" )
        return;

    // Handle "camera" mode.
    if ( $sandboxDragMode $= "camera" )
    {
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
        
        return;
    }
    
    // Handle "pull" mode.
    if ( $sandboxDragMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject($sandboxPullObject) )
            return;
              
        // Set a new target for the target joint.
        SandboxScene.setTargetJointTarget( $sandboxPullJointId, %worldPos );
    }
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelUp(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "camera".
    if ( !$sandboxDragMode $= "camera" )
        return;
        
    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() + $pref::Sandbox::cameraZoomRate );
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "camera".
    if ( !$sandboxDragMode $= "camera" )
        return;

    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() - $pref::Sandbox::cameraZoomRate );            
}
