//-----------------------------------------------------------------------------

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
    SandboxScene.addToScene( %background );
    
    // Far rocks
    %farRocks = new Sprite();
    %farRocks.setBodyType( "static" );
    %farRocks.setImage( "AquariumToy:rocksfar" );
    %farRocks.setSize( 100, 75 );
    %farRocks.setCollisionSuppress();
    %farRocks.setAwake( false );
    %farRocks.setActive( false );
    %farRocks.setSceneLayer(4);
    SandboxScene.addToScene( %farRocks );
    
    // Near rocks
    %nearRocks = new Sprite();
    %nearRocks.setBodyType( "static" );
    %nearRocks.setImage( "AquariumToy:rocksnear" );
    %nearRocks.setSize( 100, 75 );
    %nearRocks.setCollisionSuppress();
    %nearRocks.setAwake( false );
    %nearRocks.setActive( false );
    %farRocks.setSceneLayer(3);
    SandboxScene.addToScene( %nearRocks );
    
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
    SandboxScene.addToScene( %leftTrigger );
    
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
    SandboxScene.addToScene( %rightTrigger );
}

function AquariumBoundary::handleCollision(%this, %object, %collisionDetails)
{
    if (%object.class $= "FishClass")
        %object.recycle(%this.side);
}

//-----------------------------------------------------------------------------

function createAquariumEffects()
{
}

//-----------------------------------------------------------------------------

function spawnFish()
{
    for ( %i = 0; %i < $FishCount; %i++ )
    {
        %fish = new Sprite()
        {
            Animation = "AquariumToy:angelfishAnim";
            class = "FishClass";
            size = "15 15";
            SceneLayer = "2";
            SceneGroup = "14";
            minSpeed = "15";
            maxSpeed = "15";
            CollisionCallback = true;
        };
        
        %fish.createPolygonBoxCollisionShape( 15, 15);
        %fish.setCollisionGroups( 15 );
        %fish.setDefaultDensity( 1 );
        %fish.setDefaultFriction( 1.0 );
        SandboxScene.addToScene( %fish );
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
   
   // Set the object's velocity based on .speed variable
   %this.setLinearVelocityX(%this.speed);
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
   
    // Set up a string comparison switch based on the %limit
    switch$ (%side)
    {
        // Fish hit "left" boundary
        // Make it face right and go in that direction
        // Set a random position in the Y axis
        case "left":
        %this.setLinearVelocityX(%this.speed);
        %this.setLinearVelocityY(getRandom(-3, 3));
        %this.setFlipX(false);
        %this.setPositionY(getRandom(-15, 15));
        %this.setSceneLayer(%layer);

        // Fish hit "right" boundary
        // Make it face left and go in that direction
        // Set a random position in the Y axis  
        case "right":
        %this.setLinearVelocityX(-%this.speed);
        %this.setLinearVelocityY(getRandom(-3, 3));
        %this.setFlipX(true);
        %this.setPositionY(getRandom(-15, 15));
        %this.setSceneLayer(%layer);
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