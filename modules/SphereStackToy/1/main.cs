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
    Sandbox.allowManipulation( pan );
    Sandbox.allowManipulation( pull );
    
    // Set the manipulation mode.
    Sandbox.useManipulation( pull );
    
    // Initialize the toys settings.
    SphereStackToy.maxBalls = 5;
    SphereStackToy.ballStacks = 1;
    SphereStackToy.currentBalls = 0;
    SphereStackToy.createBallScheduleId = "";
    SphereStackToy.GroundWidth = 150;
    
    // Add the custom controls.
    addNumericOption("Balls to stack", 1, 10, 1, "setMaxBalls", SphereStackToy.maxBalls, true);
    addNumericOption("Balls stacks", 1, 10, 1, "setBallStacks", SphereStackToy.ballStacks, true);

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

function SphereStackToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Zoom the camera in    
    SandboxWindow.setCurrentCameraSize( 50, 37.5 );
    
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
    
    // Create the background.
    %this.createBackground();
    
    // Create the ground.
    %this.createGround();
        
    // Schedule to create a ball.
    %this.createBallScheduleId = %this.schedule( 100, "createBall" );
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

//-----------------------------------------------------------------------------

function SphereStackToy::createBackground( %this )
{    
    // Create the scroller.
    %object = new Sprite();
    
    // Stop the background from being affected by gravity.
    %object.setBodyType( "static" );
       
    // Always try to configure a scene-object prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "0 0";

    // Set the size.        
    %object.Size = "100 75";
    
    // Set to the furthest background layer.
    %object.SceneLayer = 31;
    
    // Set the scroller to use an animation!
    %object.Image = "ToyAssets:skyBackground";
            
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}

//-----------------------------------------------------------------------------

function SphereStackToy::createGround( %this )
{
    // Create the ground
    %ground = new Scroller();
    %ground.setBodyType("static");
    %ground.Image = "ToyAssets:dirtGround";
    %ground.setPosition(0, -16);
    %ground.setSize(SphereStackToy.GroundWidth, 6);
    %ground.setRepeatX(SphereStackToy.GroundWidth / 60);   
    %ground.createEdgeCollisionShape(SphereStackToy.GroundWidth/-2, 3, SphereStackToy.GroundWidth/2, 3);
    SandboxScene.add(%ground);  
    
    // Create the grass.
    %grass = new Sprite();
    %grass.setBodyType("static");
    %grass.Image = "ToyAssets:grassForeground";
    %grass.setPosition(0, -12.5);
    %grass.setSize(SphereStackToy.GroundWidth, 2); 
    SandboxScene.add(%grass);       
}

//-----------------------------------------------------------------------------

function SphereStackToy::createBall(%this)
{
    // Reset the event schedule.
    %this.createBallScheduleId = "";

    // Set the ball size.
    %ballSize = 2;
    
    // Calculate ball-stack offset and stride.    
    %ballOffset = (SphereStackToy.ballStacks * %ballSize * 2) * -0.5;
    
    // Add a ball to each stack.
    for( %n = 0; %n < SphereStackToy.ballStacks; %n++ )
    {
        // Calculate the stack offset.
        %stackOffset = %ballOffset + (%n * %ballSize * 2);
        
        // Create the ball.
        %ball = new Sprite();
        %ball.Position = %stackOffset SPC (5 + 6 * %this.currentBalls);
        %ball.Size = %ballSize;
        %ball.Image = "ToyAssets:Football";        
        %ball.setDefaultRestitution( 0.5 );
        %ball.setLinearVelocity(0, -40);
        %ball.createCircleCollisionShape( %ballSize * 0.5 );
        
        // Add to the scene.
        SandboxScene.add( %ball );
    }

    // Increase ball count.    
    %this.currentBalls++;
    
    // Finish if exceeded the required number of balls.
    if ( %this.currentBalls == %this.maxBalls)
        return;

    // Schedule to create a ball.
    %this.createBallScheduleId = %this.schedule( 150, "createBall" );
}

//-----------------------------------------------------------------------------

function SphereStackToy::setMaxBalls(%this, %value)
{
    %this.maxBalls = %value;
}

//-----------------------------------------------------------------------------

function SphereStackToy::setBallStacks(%this, %value)
{
    %this.ballStacks = %value;
}
