//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createAquariumToy( %scopeSet )
{
    exec("./scripts/aquarium.cs");
    SandboxScene.setGravity(0, 0);
    
    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", false );
    setSandboxDragModeAvailable( "zoom", false );
    
    $FishCount = 10;
    
    buildAquarium();
    createAquariumEffects();
    spawnFish();
}

//-----------------------------------------------------------------------------

function destroyAquariumToy( %scopeSet )
{
}