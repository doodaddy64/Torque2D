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
    exec( "./scripts/toolbox.cs" );

    // Initialize the canvas.
    initializeCanvas("Sandbox");
    
    // Set the canvas color.
    Canvas.BackgroundColor = "CornflowerBlue";
    Canvas.UseBackgroundColor = false;
    
    // Initialize audio.
    initializeOpenAL();    
    
    // Load Sandbox scripts.
    exec( "./scripts/scene.cs" );
    exec( "./scripts/toys.cs" );        
        
    // Load GUI profiles.
    exec("./gui/guiProfiles.cs");

    // Create the sandbox window.
    CreateSandboxWindow();
    
    // Load and configure the console.
    %scopeSet.add( TamlRead("./gui/ConsoleDialog.gui.taml") );
    GlobalActionMap.bind( keyboard, "ctrl tilde", toggleConsole );
    
    // Load and configure the toolbox.
    %scopeSet.add( TamlRead("./gui/ToolboxDialog.gui.taml") );
    GlobalActionMap.bind( keyboard, "#", toggleToolbox );

    // Scan for toys.
    scanForToys();

    // Initialize the toolbox.    
    initializeToolbox();    
}

//-----------------------------------------------------------------------------

function destroySandbox( %scopeSet )
{
    // Unload the active toy.
    unloadToy();
    
    // Destroy the sandbox window.
    destroySandboxWindow();
}
