//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createAquariumToy( %scopeSet )
{
    // Turn-off the sandbox auto-camera.
    //setSandboxAutoCamera( false );
    
    $FishCount = 10;
    
    buildAquarium();
    createAquariumEffects();
    
    for ( %i = 0; %i < $FishCount; %i++ )
    {
        spawnFish();
    }
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
}

function createAquariumEffects()
{
}

function spawnFish()
{
}

//-----------------------------------------------------------------------------

function destroyAquariumToy( %scopeSet )
{
}