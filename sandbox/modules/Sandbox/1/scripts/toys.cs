//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$activeToy = "";

//-----------------------------------------------------------------------------

function scanForToys()
{
    // Find the toy modules.
    %toyModules = ModuleDatabase.findModuleTypes( "toy", false );

    // Do we have an existing set of sandbox toys?
    if ( !isObject(SandboxToys) )
    {
        // No, so create one.
        new SimSet(SandboxToys);
    }
    
    // Clear the sandbox toys.
    SandboxToys.clear();
    
    // Fetch toy module count.
    %toyModuleCount = getWordCount( %toyModules );
    
    // Add toys.
    for ( %i = 0; %i < %toyModuleCount; %i++ )
    {
        SandboxToys.add( getWord( %toyModules, %i ) );
    }
}

//-----------------------------------------------------------------------------

function loadToy( %moduleDefinition )
{   
    // Sanity!
    if ( !isObject( %moduleDefinition ) )
    {
        error( "Cannot load toy as the specified toy is not available." );
        return;
    }
            
    // Unload the active toy.
    unloadToy();
    
    // Create a sandbox scene.
    createSandboxScene();
    
    // Load the toy.
    if ( !ModuleDatabase.loadExplicit( %moduleDefinition.ModuleId ) )
    {
        error( "Failed to load the toy '" @ %moduleDefinition.ModuleId @ "'." );
        return;
    }
    
    // Set active toy.
    $activeToy = %moduleDefinition.ModuleId;
}

//-----------------------------------------------------------------------------

function unloadToy()
{
    // Finish if no active toy loaded.
    if ( !isObject($activeToy) )
        return;
        
    // Unload the toy.
    if ( !ModuleDatabase.addFieldFilter( $activeToy ) )
    {
        error( "Failed to unload the toy '" @ $activeToy.ModuleId @ "'." );
    }
    
    // Destroy the sandbox scene.
    destroySandboxWindow();
    
    // Reset active toy.
    $activeToy = "";    
}
