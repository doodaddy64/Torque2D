//-----------------------------------------------------------------------------

function getRandomFishInfo(%index)
{
    switch(%index)
    {
        case 0:
        %fishInfo = "AquariumToy:angelfish1Anim" @ " 15 15";

        case 1:
        %fishInfo = "AquariumToy:angelfish2Anim" @ " 15 15";
        
        case 2:
        %fishInfo = "AquariumToy:butterflyfishAnim" @ " 15 15";
        
        case 3:
        %fishInfo = "AquariumToy:pufferfishAnim" @ " 15 15";
        
        case 4:
        %fishInfo = "AquariumToy:rockfishAnim" @ " 15 7.5";
        
        case 5:
        %fishInfo = "AquariumToy:seahorseAnim" @ " 7.5 15";
        
        case 6:
        %fishInfo = "AquariumToy:triggerfish1Anim" @ " 15 15";
    }

    return %fishInfo;
}

function buildAquarium()
{
    // Background
    %background = new Sprite();
    %background.setBodyType( "static" );
    %background.setImage( "AquariumToy:background" );
    %background.setSize( 100, 75 );
    %background.setCollisionSuppress();
    %background.setAwake( false );
    %background.setActive( false );
    %background.setSceneLayer(5);
    SandboxScene.add( %background );
    
    // Far rocks
    %farRocks = new Sprite();
    %farRocks.setBodyType( "static" );
    %farRocks.setImage( "AquariumToy:rocksfar" );
    %farRocks.setSize( 100, 75 );
    %farRocks.setCollisionSuppress();
    %farRocks.setAwake( false );
    %farRocks.setActive( false );
    %farRocks.setSceneLayer(4);
    SandboxScene.add( %farRocks );
    
    // Near rocks
    %nearRocks = new Sprite();
    %nearRocks.setBodyType( "static" );
    %nearRocks.setImage( "AquariumToy:rocksnear" );
    %nearRocks.setSize( 100, 75 );
    %nearRocks.setCollisionSuppress();
    %nearRocks.setAwake( false );
    %nearRocks.setActive( false );
    %farRocks.setSceneLayer(3);
    SandboxScene.add( %nearRocks );
    
    // Left trigger
    %leftTrigger = new SceneObject() { class = "AquariumBoundary"; };
    
    %leftTrigger.side = "left";
    %leftTrigger.setSize( 5, 400 );
    %leftTrigger.setPosition( -85, 0);
    %leftTrigger.setSceneLayer( 1 );
    %leftTrigger.setSceneGroup( 15 );
    %leftTrigger.setCollisionGroups( 14 );
    %leftTrigger.createPolygonBoxCollisionShape( 5, 400);
    %leftTrigger.setDefaultDensity( 1 );
    %leftTrigger.setDefaultFriction( 1.0 );        
    %leftTrigger.setAwake( true );
    %leftTrigger.setActive( true );
    %leftTrigger.setCollisionCallback(true);
    %leftTrigger.setBodyType( "static" );
    %leftTrigger.setCollisionShapeIsSensor(0, true);
    SandboxScene.add( %leftTrigger );
    
    // Right trigger
    %rightTrigger = new SceneObject() { class = "AquariumBoundary"; };
    
    %rightTrigger.setSize( 5, 400 );
    %rightTrigger.side = "right";
    %rightTrigger.setPosition( 85, 0);
    %rightTrigger.setSceneLayer( 1 );
    %rightTrigger.setSceneGroup( 15 );
    %rightTrigger.setCollisionGroups( 14 );
    %rightTrigger.createPolygonBoxCollisionShape( 5, 400);
    %rightTrigger.setDefaultDensity( 1 );
    %rightTrigger.setDefaultFriction( 1.0 );    
    %rightTrigger.setAwake( true );
    %rightTrigger.setActive( true );
    %rightTrigger.setCollisionCallback(true);
    %rightTrigger.setBodyType( "static" );
    %rightTrigger.setCollisionShapeIsSensor(0, true);
    SandboxScene.add( %rightTrigger );
}

function AquariumBoundary::handleCollision(%this, %object, %collisionDetails)
{
    if (%object.class $= "FishClass")
        %object.recycle(%this.side);
}

//-----------------------------------------------------------------------------

function createAquariumEffects()
{
    %obj = new Scroller();
    %obj.setBodyType( "static" );
    %obj.setImage( "AquariumToy:wave" );
    %obj.setPosition( 0, 0 );
    %obj.setScrollX(1);
    %obj.setSize( 200, 75 );
    %obj.setRepeatX( 1 );   
    %obj.setSceneLayer( 0 );
    %obj.setSceneGroup( 0 );
    %obj.setCollisionSuppress();
    %obj.setAwake( false );
    %obj.setActive( false );
    SandboxScene.add( %obj );
}

//-----------------------------------------------------------------------------

function spawnFish()
{
    for ( %i = 0; %i < $FishCount; %i++ )
    {
        %position = getRandom(-55, 55) SPC getRandom(-20, 20);
        %index = getRandom(0, 6);
        
        %fishInfo = getRandomFishInfo(%index);
        
        %fish = new Sprite()
        {
            Animation = getWord(%fishInfo, 0);
            class = "FishClass";
            position = %position;
            size = getWords(%fishInfo, 1, 2);
            SceneLayer = "2";
            SceneGroup = "14";
            minSpeed = "5";
            maxSpeed = "15";
            CollisionCallback = true;
        };
        
        %fish.createPolygonBoxCollisionShape( 15, 15);
        %fish.setCollisionGroups( 15 );
        %fish.setDefaultDensity( 1 );
        %fish.setDefaultFriction( 1.0 );
        SandboxScene.add( %fish );
    }
}

// This function will be called when a level loads
// on any sprite with this class
//
// %this - Represents the object calling the function. 
// %scenegraph - Represents the scene this object exists in
function FishClass::onAdd(%this)
{
    // Set a random speed for the fish
    %this.setSpeed();
   
    if (getRandom(0, 10) > 5)
    {
        %this.setLinearVelocityX(%this.speed);
        %this.setFlipX(false);
    }
    else
    {
        %this.setLinearVelocityX(-%this.speed);
        %this.setFlipX(true);
    }
}

// This function will be called when the object
// hits its world limits boundaries
//
// %this - The object calling the function
// %mode - The mode setting for the world limit
// %limit - Which world limit object reached, "right", "left", "top", "bottom"
function FishClass::recycle(%this, %side)
{
    // Fish has turned around, so set a new random speed
    %this.setSpeed();
    %layer = getRandom(0, 5);
    %this.setLinearVelocityY(getRandom(-3, 3));
    %this.setPositionY(getRandom(-15, 15));
    %this.setSceneLayer(%layer);

    if (%side $= "left")
    {
        %this.setLinearVelocityX(%this.speed);
        %this.setFlipX(false);
    }
    else if (%side $= "right")
    {
        %this.setLinearVelocityX(-%this.speed);
        %this.setFlipX(true);
    }
}

// Set the fish's speed to a random value
// Range of speed values is defined by 
// minSpeed and maxSpeed Dynamic Fields
//
// %this - The object calling the function
function FishClass::setSpeed(%this)
{
   // Speed is a dynamic variable created when this function is first called
   // Every other time after the first call will simply modify the variable
   // .minSpeed and .maxSpeed are declared in the Dynamic Fields rollout of the editor
   %this.speed = getRandom(%this.minSpeed, %this.maxSpeed);
}