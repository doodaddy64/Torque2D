//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function initializeSandbox( %scopeSet )
{    
    // Load system scripts
    exec( "./scripts/canvas.cs" );
    exec( "./scripts/openal.cs" );  
    exec( "./scripts/console.cs" );
    exec( "./scripts/defaultPrefs.cs" );

    // Initialize the canvas.
    initializeCanvas("Sandbox");
    
    // Initialize audio.
    initializeOpenAL();    
    
    // Load Sandbox scripts.
    exec("./scripts/utility.cs");
    exec("./scripts/toys.cs");
        
    // Load GUIs
    exec("./gui/guiProfiles.cs");
    %scopeSet.add( TamlRead("./gui/ConsoleDialog.gui.taml") );
}

//-----------------------------------------------------------------------------

function destroySandbox( %scopeSet )
{
}
