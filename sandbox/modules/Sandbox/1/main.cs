//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createSandbox( %scopeSet )
{    
    // Load system scripts
    exec( "./scripts/defaultPrefs.cs" );
    exec( "./scripts/canvas.cs" );
    exec( "./scripts/openal.cs" );  
    exec( "./scripts/console.cs" );   

    // Initialize the canvas.
    initializeCanvas("Sandbox");
    
    // Set the canvas color.
    Canvas.UseBackgroundColor = false;
    
    // Initialize audio.
    initializeOpenAL();    
    
    // Load Sandbox scripts.
    exec( "./scripts/scene.cs" );
    exec( "./scripts/toys.cs" );        
        
    // Load GUI profiles.
    exec("./gui/guiProfiles.cs");
    
    // Load and configure console.
    %scopeSet.add( TamlRead("./gui/ConsoleDialog.gui.taml") );
    GlobalActionMap.bind( keyboard, "ctrl tilde", toggleConsole );
    GlobalActionMap.bindcmd( keyboard, "alt k", "cls();",  "" );
    GlobalActionMap.bindcmd( keyboard, "#", toggleToolbox, "" );

    // Create the sandbox window.
    CreateSandboxWindow();
    
    // Scan for toys.
    scanForToys();
    
    loadToy( SandboxToys.getObject(0) );
}

//-----------------------------------------------------------------------------

function destroySandbox( %scopeSet )
{
    // Destroy the sandbox window.
    destroySandboxWindow();
}
