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

// Sandbox manipulation modes are:
// - Off
// - Pan
// - Pull
Sandbox.ManipulationMode = "off";

// Reset the sandbox pull object.
Sandbox.ManipulationPullObject = "";
Sandbox.ManipulationPullJointId = "";
Sandbox.ManipulationPullMaxForce = 1000;

//-----------------------------------------------------------------------------

GlobalActionMap.bind( keyboard, "space", cycleManipulation );

//-----------------------------------------------------------------------------

function Sandbox::resetManipulationModes( %this )
{   
    // These control which drag modes are available or not.
    Sandbox.ManipulationModeState["off"] = true;
    Sandbox.ManipulationModeState["pan"] = false;
    Sandbox.ManipulationModeState["pull"] = false;
    
    // Set the sandbox drag mode default.
    Sandbox.useManipulation( "off" ); 
}

//-----------------------------------------------------------------------------

function Sandbox::allowManipulation( %this, %mode )
{
    // Cannot turn-off the "off" manipulation.
    if ( %mode $= "off" )
        return;
        
    Sandbox.ManipulationModeState[%mode] = true;    
}

//-----------------------------------------------------------------------------

function Sandbox::useManipulation( %this, %mode )
{
    // Is the drag mode available?
    if ( %mode !$= "off" && !Sandbox.ManipulationModeState[%mode] )
    {
        // No, so warn.
        error( "Cannot set sandbox drag mode to " @ %mode @ " as it is currently disabled." );
        return;
    }
    
    // Set the manipulation mode.
    Sandbox.ManipulationMode = %mode;
    
    // Reset pulled object and joint.
    Sandbox.ManipulationPullObject = "";    
    if ( Sandbox.ManipulationPullJointId !$= "" && SandboxScene.isJoint(Sandbox.ManipulationPullJointId) )
    {
        SandboxScene.deleteJoint( Sandbox.ManipulationPullJointId );
        Sandbox.ManipulationPullJointId = "";
    }        
}

//-----------------------------------------------------------------------------

function cycleManipulation( %make )
{
    // Finish if being released.
    if ( !%make )
        return;

    // "off" to "pan" transition.
    if ( Sandbox.ManipulationMode $= "off" )
    {
        if ( Sandbox.ManipulationModeState["pan"] )
        {
            Sandbox.useManipulation("pan");
            return;
        }
        
        Sandbox.ManipulationMode = "pan";
    }      
    
    // "pan" to "pull" transition.
    if ( Sandbox.ManipulationMode $= "pan" )
    {
        if ( Sandbox.ManipulationModeState["pull"] )
        {
            Sandbox.useManipulation("pull");
            return;
        }
            
        Sandbox.ManipulationMode = "pull";
    }

    // "pull" to "off" transition.
    if ( Sandbox.ManipulationMode $= "pull" )
    {
        Sandbox.useManipulation("off");
    }          
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;
    
    // Set touch event.
    Sandbox.TouchEvent[%touchID] = %worldPos;
       
    // Handle "pull" mode.
    if ( Sandbox.ManipulationMode $= "pull" )
    {
        // Pick an object.
        %picked = SandboxScene.pickPoint( %worldPos );
        
        // Finish if nothing picked.
        if ( %picked $= "" )
            return;
            
        // Fetch the first object only.
        Sandbox.ManipulationPullObject = getWord( %picked, 0 );
        
        // Create the target joint.
        Sandbox.ManipulationPullJointId = SandboxScene.createTargetJoint( Sandbox.ManipulationPullObject, %worldPos, Sandbox.ManipulationPullMaxForce );
    }    
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchUp(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;

    // Reset touch event.
    Sandbox.TouchEvent[%touchID] = "";
    
    // Handle "pull" mode.
    if ( Sandbox.ManipulationMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject(Sandbox.ManipulationPullObject) )
            return;
        
        // Remove the pull joint.
        SandboxScene.deleteJoint( Sandbox.ManipulationPullJointId );
        Sandbox.ManipulationPullJointId = "";
    }      
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchMoved(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDragged(%this, %touchID, %worldPos)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;

    // Handle "pan" mode.
    if ( Sandbox.ManipulationMode $= "pan" )
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
    if ( Sandbox.ManipulationMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject(Sandbox.ManipulationPullObject) )
            return;
              
        // Set a new target for the target joint.
        SandboxScene.setTargetJointTarget( Sandbox.ManipulationPullJointId, %worldPos );
        
        return;
    }
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelUp(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "pan".
    if ( !Sandbox.ManipulationMode $= "pan" )
        return;
        
    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() + $pref::Sandbox::cameraZoomRate );
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "pan".
    if ( !Sandbox.ManipulationMode $= "pan" )
        return;

    // Increase the zoom.
    SandboxWindow.setCurrentCameraZoom( SandboxWindow.getCurrentCameraZoom() - $pref::Sandbox::cameraZoomRate );            
}
