//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createTruckToy( %scopeSet )
{       
    // Load scripts.
    exec( "./scripts/truck.cs" );

    // Create truck keys.    
    new ActionMap(truckToyMap);
    %scopeSet.add( truckToyMap );
    truckToyMap.bind(keyboard, "ctrl tilde", toggleConsole);
    truckToyMap.bind(keyboard, "left", truckReverse);
    truckToyMap.bind(keyboard, "right", truckForward);
    truckToyMap.push();
    
    // Initialize truck world.
    initializeTruckWorld();
}

//-----------------------------------------------------------------------------

function destroyTruckToy( %scopeSet )
{
    // Delete the input map.
    truckToyMap.pop();
    truckToyMap.delete();
    
    // Destroy the truck tou
    destroySandboxWindow();
}
