//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createSandbox( %scopeSet )
{    
    // Load the preferences.
    loadSandboxPreferences();
    
    // Load system scripts
    exec( "./scripts/constants.cs");
    exec( "./scripts/canvas.cs" );
    exec( "./scripts/openal.cs" );
    exec( "./scripts/console.cs" );
    exec( "./scripts/toolbox.cs" );
    exec( "./scripts/camera.cs" );

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
    GlobalActionMap.bind( keyboard, "minus", toggleToolbox );

    // Load and configure the main overlay.
    %scopeSet.add( TamlRead("./gui/MainOverlay.gui.taml") );
    
    // Scan for toys.
    scanForToys();

    // Initialize the toolbox.    
    initializeToolbox();  
}

//-----------------------------------------------------------------------------

function destroySandbox( %scopeSet )
{
    // Save sandbox preferences.
    saveSandboxPreferences();    
    
    // Unload the active toy.
    unloadToy();
    
    // Destroy the sandbox window.
    destroySandboxWindow();
}

//-----------------------------------------------------------------------------

function loadSandboxPreferences()
{
    // Load the default preferences.
    exec( "./scripts/defaultPreferences.cs" );
    
    // Load the last session preferences if available.
    if ( isFile("preferences.cs") )
        exec( "preferences.cs" );   
}

//-----------------------------------------------------------------------------

function saveSandboxPreferences()
{
    // Export only the sandbox preferences.
    export("$pref::Sandbox::*", "preferences.cs", false, false);    
}
