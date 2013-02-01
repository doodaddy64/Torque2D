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

function SphereStackToy::create( %this )
{
    // Set the sandbox drag mode availability.
    Sandbox.allowManipulation( "pan" );
    Sandbox.allowManipulation( "pull" );
    
    // Set the manipulation mode.
    Sandbox.useManipulation( "pull" );
    
    // Initialize the toys settings.
    SphereStackToy.maxBalls = 5;
    SphereStackToy.currentBalls = 0;
    SphereStackToy.createBallScheduleId = "";
    SphereStackToy.GroundWidth = 150;
    
    // Add the custom controls.
    addNumericOption("Number of balls", 1, 10, 1, "setMaxBalls", SphereStackToy.maxBalls, true);

    // Reset the toy initially.
    SphereStackToy.reset();
}

//-----------------------------------------------------------------------------

function SphereStackToy::destroy( %this )
{   
    // Cancel any pending events.
    SphereStackToy::cancelPendingEvents();
}

//-----------------------------------------------------------------------------

function SphereStackToy::setMaxBalls(%this, %value)
{
    %this.maxBalls = %value;
}

//-----------------------------------------------------------------------------

function SphereStackToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Zoom the camera in    
    SandboxWindow.setCurrentCameraZoom( 2 );
    
    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption( false );
    
    // Set the scene gravity.
    SandboxScene.setGravity( 0, -20 );
    
    // Set the drag mode as "pull".
    Sandbox.useManipulation( "pull" );
    
    // Reset the ball count.    
    %this.currentBalls = 0;
    
    // Cancel any pending events.
    SphereStackToy::cancelPendingEvents();
    
    %ground = new Scroller();
    %ground.setBodyType( "static" );
    %ground.Image = "ToyAssets:woodGround";
    %ground.setSize(SphereStackToy.GroundWidth, 2);
    %ground.setRepeatX( SphereStackToy.GroundWidth / 12 );   
    %ground.setPosition(0, -10);
    %ground.createEdgeCollisionShape( SphereStackToy.GroundWidth/-2, 1, SphereStackToy.GroundWidth/2, 1 );
    SandboxScene.add( %ground );
        
    // Schedule to create a ball.
    %this.createBallScheduleId = %this.schedule( 100, "createBall" );
}

//-----------------------------------------------------------------------------

function SphereStackToy::createBall(%this)
{
    // Reset the event schedule.
    %this.createBallScheduleId = "";
    
    // Create the ball.
    %ball = new Sprite();
    %ball.Position = "0" SPC (5 + 6 * %this.currentBalls);
    %ball.Size = "2";
    %ball.Image = "ToyAssets:Football";        
    %ball.setDefaultRestitution( 0.5 );
    %ball.setUseInputEvents(true);
    %collisionId = %ball.createCircleCollisionShape( 1 );
    SandboxScene.add( %ball );
    %ball.setLinearVelocity(0, -40);
    
    %this.currentBalls++;
    
    // Finish if exceeded the required number of balls.
    if ( %this.currentBalls >= %this.maxBalls)
        return;

    // Schedule to create a ball.
    %this.createBallScheduleId = %this.schedule( 150, "createBall" );
}

//-----------------------------------------------------------------------------

function SphereStackToy::cancelPendingEvents(%this)
{
    // Finish if there are not pending events.
    if ( !isEventPending(%this.createBallScheduleId) )
        return;
        
    // Cancel it.
    cancel(%this.createBallScheduleId);
    %this.createBallScheduleId = "";
}
