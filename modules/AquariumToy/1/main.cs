//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createAquariumToy( %scopeSet )
{
    exec("./scripts/aquarium.cs");
    SandboxScene.setGravity(0, 0);
    
    // Turn-off the sandbox auto-camera.
    setSandboxAutoCamera( false );
    
    $FishCount = 1;
    
    buildAquarium();
    createAquariumEffects();
    spawnFish();
}

//-----------------------------------------------------------------------------

function destroyAquariumToy( %scopeSet )
{
}