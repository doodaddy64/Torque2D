//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

// Sandbox touch-drag modes are:
// - Off
// - Camera
// - Pull
Sandbox.DragMode = "off";

// Reset the sandbox pull object.
Sandbox.PullObject = "";
Sandbox.PullJointId = "";

Sandbox.PullMaxForce = 1000;

//-----------------------------------------------------------------------------

GlobalActionMap.bind( keyboard, "space", setNextDragMode );

//-----------------------------------------------------------------------------

function resetSandboxDragModes()
{   
    // These control which drag modes are available or not.
    Sandbox.DragModeAvailable["off"] = true;
    Sandbox.DragModeAvailable["camera"] = false;
    Sandbox.DragModeAvailable["pull"] = false;
    
    // Set the sandbox drag mode default.
    setSandboxDragMode( "off" ); 
}

//-----------------------------------------------------------------------------

function setSandboxDragMode( %mode )
{
    // Is the drag mode available?
    if ( %mode !$= "off" && !Sandbox.DragModeAvailable[%mode] )
    {
        // No, so warn.
        error( "Cannot set sandbox drag mode to " @ %mode @ " as it is currently disabled." );
        return;
    }
    
    Sandbox.DragMode = %mode;
    
    // Reset pulled object and joint.
    Sandbox.PullObject = "";    
    if ( Sandbox.PullJointId !$= "" && SandboxScene.isJoint(Sandbox.PullJointId) )
    {
        SandboxScene.deleteJoint( Sandbox.PullJointId );
        Sandbox.PullJointId = "";
    }        
}

//-----------------------------------------------------------------------------

function setNextDragMode( %make )
{
    // Finish if being released.
    if ( !%make )
        return;

    // "off" to "camera" transition.
    if ( Sandbox.DragMode $= "off" )
    {
        if ( Sandbox.DragModeAvailable["camera"] )
        {
            setSandboxDragMode("camera");
            return;
        }
        
        Sandbox.DragMode = "camera";
    }      
    
    // "camera" to "pull" transition.
    if ( Sandbox.DragMode $= "camera" )
    {
        if ( Sandbox.DragModeAvailable["pull"] )
        {
            setSandboxDragMode("pull");
            return;
        }
            
        Sandbox.DragMode = "pull";
    }

    // "pull" to "off" transition.
    if ( Sandbox.DragMode $= "pull" )
    {
        setSandboxDragMode("off");
    }          
}

//-----------------------------------------------------------------------------

function setSandboxDragModeAvailable( %mode, %status )
{
    Sandbox.DragModeAvailable[%mode] = %status;    
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.DragMode $= "off" )
        return;
    
    // Set touch event.
    Sandbox.TouchEvent[%touchID] = %worldPos;
       
    // Handle "pull" mode.
    if ( Sandbox.DragMode $= "pull" )
    {
        // Pick an object.
        %picked = SandboxScene.pickPoint( %worldPos );
        
        // Finish if nothing picked.
        if ( %picked $= "" )
            return;
            
        // Fetch the first object only.
        Sandbox.PullObject = getWord( %picked, 0 );
        
        // Create the target joint.
        Sandbox.PullJointId = SandboxScene.createTargetJoint( Sandbox.PullObject, %worldPos, Sandbox.PullMaxForce );
    }    
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchUp(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.DragMode $= "off" )
        return;

    // Reset touch event.
    Sandbox.TouchEvent[%touchID] = "";
    
    // Handle "pull" mode.
    if ( Sandbox.DragMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject(Sandbox.PullObject) )
            return;
        
        // Remove the pull joint.
        SandboxScene.deleteJoint( Sandbox.PullJointId );
        Sandbox.PullJointId = "";
    }      
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchMoved(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.DragMode $= "off" )
        return;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDragged(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.DragMode $= "off" )
        return;

    // Handle "camera" mode.
    if ( Sandbox.DragMode $= "camera" )
    {
        // Fetch touch event.
        %lastWorldPos = Sandbox.TouchEvent[%touchID];

        // Calculate touch delta.
        %offsetX = getWord(%worldPos, 0) - getWord(%lastWorldPos, 0);
        %offsetY = getWord(%worldPos, 1) - getWord(%lastWorldPos, 1);
        %touchDelta = %offsetX SPC %offsetY;
       
        // Update touch event (offset by our adjustment).
        %offsetWorldPosX = getWord(%worldPos, 0) - getWord(%touchDelta, 0);
        %offsetWorldPosY = getWord(%worldPos, 1) - getWord(%touchDelta, 1);
        Sandbox.TouchEvent[%touchID] = %offsetWorldPosX SPC %offsetWorldPosY;    
                
        // Update the camera.
        %cameraPosition = SandboxWindow.getCurrentCameraPosition();    
        SandboxWindow.setCurrentCameraPosition( getWord(%cameraPosition,0) - getWord(%touchDelta,0), getWord(%cameraPosition,1) - getWord(%touchDelta,1) );
        
        return;
    }
    
    // Handle "pull" mode.
    if ( Sandbox.DragMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject(Sandbox.PullObject) )
            return;
              
        // Set a new target for the target joint.
        SandboxScene.setTargetJointTarget( Sandbox.PullJointId, %worldPos );
    }
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelUp(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "camera".
    if ( !Sandbox.DragMode $= "camera" )
        return;
        
    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() + $pref::Sandbox::cameraZoomRate );
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "camera".
    if ( !Sandbox.DragMode $= "camera" )
        return;

    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() - $pref::Sandbox::cameraZoomRate );            
}
