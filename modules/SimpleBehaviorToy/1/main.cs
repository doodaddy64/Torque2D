//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createSimpleBehaviorToy( %scopeSet )
{
    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", false );
    
    // Set the drag mode as "pull".
    setSandboxDragMode( "pull" );
}

//-----------------------------------------------------------------------------

function destroySimpleBehaviorToy( %scopeSet )
{
}
