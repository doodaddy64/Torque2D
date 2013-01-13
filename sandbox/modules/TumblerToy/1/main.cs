//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$maxBalls = 500;
$currentBalls = 0;

//-----------------------------------------------------------------------------

function createTumblerToy( %scopeSet )
{
    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption( false );
    
    // Set the scene gravity.
    SandboxScene.setGravity( 0, -9.8 );
    
    // Create the tumbler.
    %tumbler = new Sprite();
    SandboxScene.addToScene( %tumbler );
    %tumbler.createPolygonBoxCollisionShape( 1, 40, 20, 0, 0 );
    %tumbler.createPolygonBoxCollisionShape( 1, 40, -20, 0, 0 );
    %tumbler.createPolygonBoxCollisionShape( 40, 1, 0, 20, 0 );
    %tumbler.createPolygonBoxCollisionShape( 40, 1, 0, -20, 0 );    
    %tumberJoint = SandboxScene.createRevoluteJoint( %tumbler, 0, "0 0" );
    SandboxScene.setRevoluteJointMotor( %tumberJoint, true, 15, 1000000 );
    
    // Schedule to create a ball.
    schedule( 100, 0, createTumblerBall );
}

//-----------------------------------------------------------------------------

function createTumblerBall()
{ 
    for( %n = 0; %n < 5; %n++ )
    {      
        // Create the ball.
        %ball = new Sprite();
        %ball.Position = getRandom(-10,10) SPC "0";
        %ball.Size = "1";
        %ball.ImageMap = "TumblerToy:FootballImage";
        %ball.createCircleCollisionShape( 0.5 );
        SandboxScene.addToScene( %ball );

        // Increase ball count.
        $currentBalls++;
        
        // Finish if exceeded the required number of balls.
        if ( $currentBalls >= $maxBalls )
            return;
    }

    // Schedule to create a ball.
    schedule( 100, 0, createTumblerBall );
}

//-----------------------------------------------------------------------------

function destroyTumblerToy( %scopeSet )
{
}
