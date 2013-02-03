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
Sandbox.ManipulationPullMaxForce = 1000;

// Reset the touch events.
Sandbox.TouchEvents = new ScriptObject()
{
    class = SandboxTouchGesture;
    EventCount = 0;
    EventActive[0] = false;
    EventActive[1] = false;
};

//-----------------------------------------------------------------------------

function SandboxTouchGesture::onTouchDownEvent( %this, %touchId, %worldPosition )
{
    // Sanity!
    if ( %this.EventActive[%touchId] == true )
    {
        error( "SandboxTouchGesture::onTouchDownEvent() - Touch Id already active." );
        return;        
    }
    
    // Store current touch.
    %this.Event[%touchId] = %worldPosition;
        
    // Set last touch event as curren touch.
    %this.LastEvent[%touchId] = %worldPosition;    
    
    // Flag event as active.
    %this.EventActive[%touchId] = true;
    
    // Increase event count.
    %this.EventCount++;
}

//-----------------------------------------------------------------------------

function SandboxTouchGesture::onTouchUpEvent( %this, %touchId, %worldPosition )
{
    // Sanity!
    if ( %this.EventActive[%touchId] == false )
    {
        error( "SandboxTouchGesture::onTouchUpEvent() - Touch Id not active." );
        return;        
    }
        
    // Reset last touch.
    %this.LastEvent[%touchId] = "";
    
    // Reset current touch.
    %this.Event[%touchId] = "";
    
    // Flag event as inactive.
    %this.EventActive[%touchId] = false;
    
    // Increase event count.
    %this.EventCount--;    
}

//-----------------------------------------------------------------------------

function SandboxTouchGesture::onTouchDraggedEvent( %this, %touchId, %worldPosition )
{
    // Sanity!
    if ( %this.EventActive[%touchId] == false )
    {
        error( "SandboxTouchGesture::onTouchDraggedEvent() - Touch Id not active." );
        return;        
    }
        
    // Set last touch as current touch.
    %this.LastEvent[%touchId] = %this.Event[%touchId];
    
    // Store current touch.
    %this.Event[%touchId] = %worldPosition;
}

//-----------------------------------------------------------------------------

function SandboxTouchGesture::onTouchAdjustEvent( %this, %touchId, %worldPosition )
{
    // Sanity!
    if ( %this.EventActive[%touchId] == false )
    {
        error( "SandboxTouchGesture::onTouchAdjustEvent() - Touch Id not active." );
        return;        
    }
        
    // Store current touch.
    %this.Event[%touchId] = %worldPosition;
}

//-----------------------------------------------------------------------------

function SandboxTouchGesture::fetchZoomGesture( %this )
{
    // Finish if we don't have two touch events.
    if ( %this.EventCount != 2 )
        return "";

    // Finish if we don't have touch Ids zero and one active.
    if ( !%this.EventActive[0] || !%this.EventActive[1] )
        return "";
        
    // Calculate the last and current separations.
    %lastLength = Vector2Length( Vector2Abs( %this.LastEvent[0], %this.LastEvent[1] ) );
    %currentLength = Vector2Length( Vector2Abs( %this.Event[0], %this.Event[1] ) );
    
    // Calculate the change in separation length.
    return %currentLength - %lastLength;
}

//-----------------------------------------------------------------------------

function SandboxTouchGesture::fetchDragGesture( %this, %touchId )
{
    // Finish if we don't have two touch events.
    if ( %this.EventCount != 1 )
        return "";

    // Finish if we don't have the touch Id active.
    if ( !%this.EventActive[%touchId] )
        return "";

    // Calculate drag distance.
    return Vector2Sub( %this.Event[%touchId], %this.LastEvent[%touchId] );
}

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

    // Set the current mode as text on the button.    
    if ( isObject(ManipulationModeButton) )
    {
        // Make the displayed mode more consistent.
        if ( %mode $= "off" )
            %mode = "Off";
        else if ( %mode $= "pan" )
            %mode = "Pan";
        else if ( %mode $= "pull" )
        %mode = "Pull";
        
        // Make the mode consistent when showed.
        ManipulationModeButton.Text = %mode;
    }
    
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

function SandboxWindow::onTouchDown(%this, %touchID, %worldPosition)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;
        
    // Set touch event.
    Sandbox.TouchEvents.onTouchDownEvent( %touchID, %worldPosition );
           
    // Handle "pull" mode.
    if ( Sandbox.ManipulationMode $= "pull" )
    {
        // Reset the pull
        Sandbox.ManipulationPullObject[%touchID] = "";
        Sandbox.ManipulationPullJointId[%touchID] = "";
        
        // Pick an object.
        %picked = SandboxScene.pickPoint( %worldPosition );
        
        // Finish if nothing picked.
        if ( %picked $= "" )
            return;
        
        // Fetch the pick count.
        %pickCount = getWordCount( %picked );
        
        for( %n = 0; %n < %pickCount; %n++ )
        {
            // Fetch the picked object.
            %pickedObject = getWord( %picked, %n );
            
            // Skip if the object is static.
            if ( %pickedObject.getBodyType() $= "static" )
                continue;
                
            // Skipp if the object is ignoring being picked.
            if ( %pickedObject.ignorePick )
                continue;
                
            // Set the pull object.
            Sandbox.ManipulationPullObject[%touchID] = %pickedObject;
            Sandbox.ManipulationPullJointId[%touchID] = SandboxScene.createTargetJoint( %pickedObject, %worldPosition, Sandbox.ManipulationPullMaxForce );            
            return;
        }
        
        return;
    }    
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchUp(%this, %touchID, %worldPosition)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;
        
    // Set touch event.
    Sandbox.TouchEvents.onTouchUpEvent( %touchID, %worldPosition );

    // Handle "pull" mode.
    if ( Sandbox.ManipulationMode $= "pull" )
    {       
        // Finish if nothing is being pulled.
        if ( !isObject(Sandbox.ManipulationPullObject[%touchID]) )
            return;
        
        // Reset the pull object.
        Sandbox.ManipulationPullObject[%touchID] = "";
        
        // Remove the pull joint.
        SandboxScene.deleteJoint( Sandbox.ManipulationPullJointId[%touchID] );
        Sandbox.ManipulationPullJointId[%touchID] = "";        
        return;
    }      
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchMoved(%this, %touchID, %worldPosition)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchDragged(%this, %touchID, %worldPosition)
{
    // Finish if the drag mode is off.
    if ( Sandbox.ManipulationMode $= "off" )
        return;

    // Set touch event.
    Sandbox.TouchEvents.onTouchDraggedEvent( %touchID, %worldPosition );
    
    // Handle "pan" mode.
    if ( Sandbox.ManipulationMode $= "pan" )
    {
        // Fetch the touch event count.
        %eventCount = Sandbox.TouchEvents.EventCount;
        
        // Do we have a single touch event?
        if ( %eventCount == 1 )
        {
            // Yes, so fetch the drag gesture.
            %dragDelta = Sandbox.TouchEvents.fetchDragGesture( %touchId );
            
            // Finish if no drag available.
            if ( %dragDelta $= "" )
                return;
                        
            // Adjust the current touch event.
            // This is an offset by our camera adjustment as the touch event will be incorrect.
            Sandbox.TouchEvents.onTouchAdjustEvent( %touchID, Vector2Sub( %worldPosition, %dragDelta ) );
                                   
            // Update the camera position.
            SandboxWindow.setCameraPosition( Vector2Sub( SandboxWindow.getCameraPosition(), %dragDelta ) );
            
            return;
        }
        
        // Do we have two event counts?
        if ( %eventCount == 2 )
        {
            // Yes, so fetch the zoom gesture.
            %zoomLength = Sandbox.TouchEvents.fetchZoomGesture();
            
            // Finish if no zoom available.
            if ( %zoomLength $= "" )
                return;
                
            // Change the zoom.
            SandboxWindow.setCameraZoom( SandboxWindow.getCameraZoom() + ( %zoomLength * $pref::Sandbox::cameraZoomRate ) );
        }
    }
    
    // Handle "pull" mode.
    if ( Sandbox.ManipulationMode $= "pull" )
    {
        // Finish if nothing is being pulled.
        if ( !isObject(Sandbox.ManipulationPullObject[%touchID]) )
            return;
              
        // Set a new target for the target joint.
        SandboxScene.setTargetJointTarget( Sandbox.ManipulationPullJointId[%touchID], %worldPosition );
        
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
    SandboxWindow.setCameraZoom( SandboxWindow.getCameraZoom() + $pref::Sandbox::cameraZoomRate );
}

//-----------------------------------------------------------------------------

function SandboxWindow::onMouseWheelDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
    // Finish if the drag mode is not "pan".
    if ( !Sandbox.ManipulationMode $= "pan" )
        return;

    // Increase the zoom.
    SandboxWindow.setCameraZoom( SandboxWindow.getCameraZoom() - $pref::Sandbox::cameraZoomRate );            
}
