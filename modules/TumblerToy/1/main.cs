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

function createTumblerToy( %scopeSet )
{
    if ( !isObject(TumblerToy) )
    {
        new ScriptObject(TumblerToy)
        {
            createTumblerBallSchedule = "";
            maxBalls = 100;
            currentBalls = 0;
            repeat = true;
        };
        
        %scopeSet.add(TumblerToy);
    }
    
    %checkbox = createCheckBoxControl("Create lots of balls?", "10 10", "20 20", TumblerToy, true, "repeat", TumblerToy.repeat);
    %scopeSet.add(%checkbox);
    
    %textEdit = createNumberEditControl("Number of balls", "10 40", "20 25", TumblerToy, true, "maxBalls", TumblerToy.maxBalls);
    %scopeSet.add(%textEdit);
    
    ToyCustomControls.add(%checkbox);
    ToyCustomControls.add(%textEdit);
    
    TumblerToy.reset();
}

function TumblerToy::reset(%this)
{
    SandboxScene.clear();
    
    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption( false );
    
    // Set the scene gravity.
    SandboxScene.setGravity( 0, -39.8 );
    
    // Set the drag mode as "pull".
    setSandboxDragMode( "pull" );

    // Create the tumbler.
    %tumbler = new Sprite();
    SandboxScene.add( %tumbler );
    %tumbler.createPolygonBoxCollisionShape( 1, 50, 25, 0, 0 );
    %tumbler.createPolygonBoxCollisionShape( 1, 50, -25, 0, 0 );
    %tumbler.createPolygonBoxCollisionShape( 50, 1, 0, 25, 0 );
    %tumbler.createPolygonBoxCollisionShape( 50, 1, 0, -25, 0 );    
    %tumberJoint = SandboxScene.createRevoluteJoint( %tumbler, 0, "0 0" );
    SandboxScene.setRevoluteJointMotor( %tumberJoint, true, 15, 1000000 );
    
    %this.currentBalls = 0;
    
    // Schedule to create a ball.
    %this.createTumblerBallSchedule = %this.schedule( 100, "createTumblerBall" );
}

function TumblerToy::setRepeat(%this, %value)
{
    %this.repeat = %value;
}

function TumblerToy::setMaxBalls(%this, %value)
{
    %this.maxBalls = %value;
}

function TumblerToy::createTumblerBall(%this)
{
    // Reset the event schedule.
    %this.createTumblerBallSchedule = "";

    // Fetch the stock color count.
    %stockColorCount = getStockColorCount();
    
    // Create some balls.
    for ( %n = 0; %n < 5; %n++ )
    {      
        // Create the ball.
        %ball = new Sprite();
        %ball.Position = getRandom(-10,10) SPC "0";
        %ball.Size = "2";
        %ball.Image = "ToyAssets:Football";        
        %ball.setDefaultRestitution( 0.6 );
        %collisionId = %ball.createCircleCollisionShape( 1 );
        SandboxScene.add( %ball );

        // Increase ball count.
        %this.currentBalls++;
        
        // Finish if exceeded the required number of balls.
        if ( %this.currentBalls >= %this.maxBalls)
            return;
    }

    // Schedule to create a ball.
    if (%this.repeat)
        %this.createTumblerBallSchedule = %this.schedule( 100, "createTumblerBall" );
    
    SandboxScene.setBatchingEnabled( true );
}

//-----------------------------------------------------------------------------

function destroyTumblerToy( %scopeSet )
{
    // Cancel any pending events.
    if ( isEventPending(TumblerToy.createTumblerBallSchedule) )
    {
        cancel(TumblerToy.createTumblerBallSchedule);
        TumblerToy.createTumblerBallSchedule = "";
    }
}
