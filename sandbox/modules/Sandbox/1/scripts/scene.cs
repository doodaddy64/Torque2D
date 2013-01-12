//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createSandboxWindow()
{
    // Sanity!
    if ( !isObject(SandboxWindow) )
    {
        // Create the scene window.
        new SceneWindow(SandboxWindow);

        // Set profile.        
        SandboxWindow.Profile = SandboxWindowProfile;
        
        // Push the window.
        Canvas.setContent( SandboxWindow );                     
    }

    // Set camera to a canonical state.
    %allBits = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31";
    SandboxWindow.stopCameraMove();
    SandboxWindow.dismount();
    SandboxWindow.setViewLimitOff();
    SandboxWindow.setRenderGroups( %allBits );
    SandboxWindow.setRenderLayers( %allBits );
    SandboxWindow.setObjectInputEventGroupFilter( %allBits );
    SandboxWindow.setObjectInputEventLayerFilter( %allBits );
    SandboxWindow.setLockMouse( false );
    SandboxWindow.setCurrentCameraPosition( 0, 0, 100, 75 );
    SandboxWindow.setCurrentCameraZoom( 1 );    
}

//-----------------------------------------------------------------------------

function destroySandboxWindow()
{
    // Finish if no window available.
    if ( !isObject(SandboxWindow) )
        return;
    
    // Delete the window.
    SandboxWindow.delete();
}

//-----------------------------------------------------------------------------

function createSandboxScene()
{
    // Destroy the scene if it already exists.
    if ( isObject(SandboxScene) )
        destroySandboxScene();
    
    // Create the scene.
    new Scene(SandboxScene);
    
    // Set the scene color.
    SandboxScene.BackgroundColor = "DimGray";
    SandboxScene.UseBackgroundColor = true;
    
    // Set the debugging options.
    SandboxScene.setDebugOn( 0 );
        
    // Sanity!
    if ( !isObject(SandboxWindow) )
    {
        error( "Sandbox: Created scene but no window available." );
        return;
    }
    
    // Set window to scene.
    SandboxWindow.setScene( SandboxScene );
}

//-----------------------------------------------------------------------------

function destroySandboxScene()
{
    // Finish if no scene available.
    if ( !isObject(SandboxScene) )
        return;

    // Delete the scene.
    SandboxScene.delete();         
}