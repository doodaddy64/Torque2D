//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$maxBalls = 200;
$currentBalls = 0;
$createTumblerBallSchedule = "";

//-----------------------------------------------------------------------------

function createTumblerToy( %scopeSet )
{
    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption( false );
    
    // Set the scene gravity.
    SandboxScene.setGravity( 0, -39.8 );
    
    // Create the tumbler.
    %tumbler = new Sprite();
    SandboxScene.addToScene( %tumbler );
    %tumbler.createPolygonBoxCollisionShape( 1, 50, 25, 0, 0 );
    %tumbler.createPolygonBoxCollisionShape( 1, 50, -25, 0, 0 );
    %tumbler.createPolygonBoxCollisionShape( 50, 1, 0, 25, 0 );
    %tumbler.createPolygonBoxCollisionShape( 50, 1, 0, -25, 0 );    
    %tumberJoint = SandboxScene.createRevoluteJoint( %tumbler, 0, "0 0" );
    SandboxScene.setRevoluteJointMotor( %tumberJoint, true, 15, 1000000 );
    
    // Schedule to create a ball.
    $createTumblerBallSchedule = schedule( 100, 0, createTumblerBall );
}

//-----------------------------------------------------------------------------

function createTumblerBall()
{
    // Reset the event schedule.
    $createTumblerBallSchedule = "";

    // Fetch the stock color count.
    %stockColorCount = getStockColorCount();
    
    // Create some balls.
    for( %n = 0; %n < 5; %n++ )
    {      
        // Create the ball.
        %ball = new Sprite();
        %ball.Position = getRandom(-10,10) SPC "0";
        %ball.Size = "1";
        %ball.Image = "TumblerToy:FootballImage";        
        %ball.BlendColor = getStockColorName(getRandom(0,%stockColorCount-1));
        %ball.setDefaultRestitution( 0.6 );
        %collisionId = %ball.createCircleCollisionShape( 0.5 );        
        SandboxScene.addToScene( %ball );

        // Increase ball count.
        $currentBalls++;
        
        // Finish if exceeded the required number of balls.
        if ( $currentBalls >= $maxBalls )
            return;
    }

    // Schedule to create a ball.
    $createTumblerBallSchedule = schedule( 100, 0, createTumblerBall );
}

//-----------------------------------------------------------------------------

function destroyTumblerToy( %scopeSet )
{
    // Cancel any pending events.
    if ( isEventPending($createTumblerBallSchedule) )
    {
        cancel($createTumblerBallSchedule);
        $createTumblerBallSchedule = "";
    }
}