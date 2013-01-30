//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
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
    exec( "./scripts/customToolboxGui.cs" );
    exec( "./scripts/manipulation.cs" );

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

    // Load and configure the main overlay.
    %scopeSet.add( TamlRead("./gui/MainOverlay.gui.taml") );
    
    // Scan for toys.
    scanForToys();

    // Initialize the toolbox.    
    initializeToolbox();
    
    // Initialize the "cannot render" proxy.
    new RenderProxy(CannotRenderProxy)
    {
        Image = "ToyAssets:CannotRender";
    };
    %scopeSet.add( CannotRenderProxy );
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
