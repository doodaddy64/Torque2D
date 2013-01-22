//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$toyAllCategoryIndex = 1;
$defaultToySelected = false;

//-----------------------------------------------------------------------------

function initializeToolbox()
{   
    // Populate the stock colors.
    %colorCount = getStockColorCount();
    for ( %i = 0; %i < %colorCount; %i++ )
    {
        // Fetch stock color name.
        %colorName = getStockColorName(%i);
        
        // Add to the list.        
        BackgroundColorSelectList.add( getStockColorName(%i), %i );
        
        // Select the color if it's the default one.
        if ( %colorName $= $pref::Sandbox::defaultBackgroundColor )
            BackgroundColorSelectList.setSelected( %i );
    }
    
    // Populate the toy categories.
    ToyCategorySelectList.add( "All", $toyAllCategoryIndex );
    ToyCategorySelectList.add( "Features (Physics)", $toyAllCategoryIndex+1 );
    ToyCategorySelectList.add( "Features (Rendering)", $toyAllCategoryIndex+2 );
    ToyCategorySelectList.add( "Stress Testing", $toyAllCategoryIndex+3 );
    ToyCategorySelectList.add( "Fun", $toyAllCategoryIndex+4 );
    ToyCategorySelectList.add( "Development", $toyAllCategoryIndex+5 );
    ToyCategorySelectList.add( "Miscellaneous", $toyAllCategoryIndex+6 );
    
    // Set the "All" category as the default.
    // NOTE:    This is important to use so that the user-configurable default toy
    //          can be selected at start-up.
    ToyCategorySelectList.setSelected( $toyAllCategoryIndex );
       
    // Is this on the desktop?
    if ( $platform $= "windows" || $platform $= "macos" )
    {
        // Yes, so make the controls screen controls visible.
        ResolutionSelectLabel.Active = true;
        ResolutionSelectList.Active = true;
        FullscreenOptionLabel.Active = true;
        FullscreenOptionCheckBox.Active = true;
        
        // Fetch the active resolution.
        %activeResolution = getRes();
        %activeWidth = getWord(%activeResolution, 0);
        %activeHeight = getWord(%activeResolution, 1);
        %activeBpp = getWord(%activeResolution, 2);
        
        // Fetch the resolutions.
        %resolutionList = getResolutionList( $pref::Video::displayDevice );
        %resolutionCount = getWordCount( %resolutionList ) / 3;
        %inputIndex = 0;
        %outputIndex = 0;
        for( %i = 0; %i < %resolutionCount; %i++ )
        {
            // Fetch the resolution entry.
            %width = getWord( %resolutionList, %inputIndex );
            %height = getWord( %resolutionList, %inputIndex+1 );
            %bpp = getWord( %resolutionList, %inputIndex+2 );
            %inputIndex += 3;
            
            // Skip the 16-bit ones.
            if ( %bpp == 16 )
                continue;
                
            // Store the resolution.
            $sandboxResolutions[%outputIndex] = %width SPC %height SPC %bpp;
            
            // Add to the list.
            ResolutionSelectList.add( %width @ "x" @ %height SPC "(" @ %bpp @ ")", %outputIndex );
            
            // Select the resolution if it's the default one.
            if ( %width == %activeWidth && %height == %activeHeight && %bpp == %activeBpp )
                ResolutionSelectList.setSelected( %outputIndex );
                
            %outputIndex++;
        }
    }
    else
    {
        Canvas.pushDialog(MainOverlay, 2);
    }
}

//-----------------------------------------------------------------------------

function toggleToolbox(%make)
{
    // Finish if being released.
    if ( !%make )
        return;
        
    // Finish if the console is awake.
    if ( ConsoleDialog.isAwake() )
        return;       
        
    // Is the toolbox awake?
    if ( ToolboxDialog.isAwake() )
    {
        // Yes, so deactivate it.
        if ( $enableDirectInput )
            activateKeyboard();
        Canvas.popDialog(ToolboxDialog);        
        return;
    }
    
    // Activate it.
    if ( $enableDirectInput )
        deactivateKeyboard();    
    Canvas.pushDialog(ToolboxDialog, 2);         
}

//-----------------------------------------------------------------------------

function ToyCategorySelectList::onSelect(%this)
{
    // Fetch the index.
    %index = %this.getSelected();

    // Clear the toy GUI list.    
    ToySelectList.clear();
    
    // Unload the active toy.
    unloadToy();   

    // Fetch the toy count.
    %toyCount = SandboxToys.getCount();

    // Populate toys in the selected category.
    for ( %toyIndex = 0; %toyIndex < %toyCount; %toyIndex++ )
    {
        // Fetch the toy module.
        %moduleDefinition = SandboxToys.getObject( %toyIndex );

        // Skip the toy module if the "all" category is not selected and if the toy is not in the selected category.
        if ( %index != $toyAllCategoryIndex && %moduleDefinition.ToyCategoryIndex != %index )
            continue;

        // Add the toy GUI list.
        ToySelectList.add( %moduleDefinition.moduleId, %moduleDefinition.getId() );
        
        // Select the toy if it's the default and we've not selected a toy yet.
        if ( !$defaultToySelected && %moduleDefinition.moduleId $= $pref::Sandbox::defaultToyId )
            ToySelectList.setSelected( %moduleDefinition.getId() );
    }
    
    // Flag as the default toy selected.
    $defaultToySelected = true;
   
    // Was a toy selected?
    if ( ToySelectList.getSelected() == 0 )
    {
        // No, so are there any toys to select?
        if ( ToySelectList.size() > 0 )
        {
            // Yes, so select the first one.
            ToySelectList.setFirstSelected();
        }
        else
        {
            // No, so recreate the sandbox scene just in-case any previous toy has left remenants.
            createSandboxScene();
        }        
    }
    
    // Fetch whether a toy is selected or not.
    %toySelected = ToySelectList.getSelected() != 0;
    
    // (De)activate the toy selection and reloading appropriately.
    ToySelectLabel.Active = %toySelected;
    ToySelectList.Active = %toySelected;
    ReloadToyButton.Active = %toySelected;
}

//-----------------------------------------------------------------------------

function ToySelectList::onSelect(%this)
{
    // Fetch the index.
    %index = %this.getSelected();
    
    // Finish if already selected.
    if ( %index == $activeToy )
        return;
    
    // Load the selected toy.
    loadToy( %index );
}

//-----------------------------------------------------------------------------

function BackgroundColorSelectList::onSelect(%this)
{
    // Finish if the sandbox scene is not available.
    if ( !isObject(SandboxScene) )
        return;
            
    // Fetch the index.
    $activeSceneColor = %this.getSelected();
            
    // Set the scene color.
    SandboxScene.BackgroundColor = getStockColorName($activeSceneColor);
    SandboxScene.UseBackgroundColor = true;
}

//-----------------------------------------------------------------------------

function ResolutionSelectList::onSelect(%this)
{
    // Finish if the sandbox scene is not available.
    if ( !isObject(SandboxScene) )
        return;
            
    // Fetch the index.
    %index = %this.getSelected();

    // Fetch resolution.
    %resolution = $sandboxResolutions[%index];
    
    // Set the screen mode.    
    setScreenMode( GetWord( %resolution , 0 ), GetWord( %resolution, 1 ), GetWord( %resolution, 2 ), $pref::Video::fullScreen );
}

//-----------------------------------------------------------------------------

function ReloadToyButton::onClick(%this)
{
    // Finish if no toy is loaded.
    if ( !isObject($activeToy) )
        return;
        
    // Reload the toy.
    loadToy( $activeToy );    
}

//-----------------------------------------------------------------------------

function updateToolboxOptions()
{
    // Finish if the sandbox scene is not available.
    if ( !isObject(SandboxScene) )
        return;
        
    // Set the scene color.
    SandboxScene.BackgroundColor = getStockColorName($activeSceneColor);
    SandboxScene.UseBackgroundColor = true;        
       
    // Set option.
    if ( $pref::Sandbox::metricsOption )
        SandboxScene.setDebugOn( "metrics" );
    else
        SandboxScene.setDebugOff( "metrics" );

    // Set option.
    if ( $pref::Sandbox::jointsOption )
        SandboxScene.setDebugOn( "joints" );
    else
        SandboxScene.setDebugOff( "joints" );

    // Set option.
    if ( $pref::Sandbox::wireframeOption )
        SandboxScene.setDebugOn( "wireframe" );
    else
        SandboxScene.setDebugOff( "wireframe" );
        
    // Set option.
    if ( $pref::Sandbox::aabbOption )
        SandboxScene.setDebugOn( "aabb" );
    else
        SandboxScene.setDebugOff( "aabb" );

    // Set option.
    if ( $pref::Sandbox::oobbOption )
        SandboxScene.setDebugOn( "oobb" );
    else
        SandboxScene.setDebugOff( "oobb" );
        
    // Set option.
    if ( $pref::Sandbox::sleepOption )
        SandboxScene.setDebugOn( "sleep" );
    else
        SandboxScene.setDebugOff( "sleep" );                

    // Set option.
    if ( $pref::Sandbox::collisionOption )
        SandboxScene.setDebugOn( "collision" );
    else
        SandboxScene.setDebugOff( "collision" );
        
    // Set option.
    if ( $pref::Sandbox::positionOption )
        SandboxScene.setDebugOn( "position" );
    else
        SandboxScene.setDebugOff( "position" );
        
    // Set option.
    if ( $pref::Sandbox::sortOption )
        SandboxScene.setDebugOn( "sort" );
    else
        SandboxScene.setDebugOff( "sort" );
                  
    // Set the options check-boxe.
    MetricsOptionCheckBox.setStateOn( $pref::Sandbox::metricsOption );
    JointsOptionCheckBox.setStateOn( $pref::Sandbox::jointsOption );
    WireframeOptionCheckBox.setStateOn( $pref::Sandbox::wireframeOption );
    AABBOptionCheckBox.setStateOn( $pref::Sandbox::aabbOption );
    OOBBOptionCheckBox.setStateOn( $pref::Sandbox::oobbOption );
    SleepOptionCheckBox.setStateOn( $pref::Sandbox::sleepOption );
    CollisionOptionCheckBox.setStateOn( $pref::Sandbox::collisionOption );
    PositionOptionCheckBox.setStateOn( $pref::Sandbox::positionOption );
    SortOptionCheckBox.setStateOn( $pref::Sandbox::sortOption ); 
    
    // Is this on the desktop?
    if ( $platform $= "windows" || $platform $= "macos" )
    {
        // Set the fullscreen check-box.
        FullscreenOptionCheckBox.setStateOn( $pref::Video::fullScreen );
        
        // Set the full-screen mode appropriately.
        if ( isFullScreen() != $pref::Video::fullScreen )
            toggleFullScreen();            
    }
    else
    {
        // No, so make the screen controls visible.
        ResolutionSelectLabel.Active = false;
        ResolutionSelectList.Active = false;
        FullscreenOptionLabel.Active = false;
        FullscreenOptionCheckBox.Active = false;
    }    
}

//-----------------------------------------------------------------------------

function setFullscreenOption( %flag )
{
    $pref::Video::fullScreen = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setMetricsOption( %flag )
{
    $pref::Sandbox::metricsOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setMetricsOption( %flag )
{
    $pref::Sandbox::metricsOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setJointsOption( %flag )
{
    $pref::Sandbox::jointsOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setWireframeOption( %flag )
{
    $pref::Sandbox::wireframeOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setAABBOption( %flag )
{
    $pref::Sandbox::aabbOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setOOBBOption( %flag )
{
    $pref::Sandbox::oobbOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setSleepOption( %flag )
{
    $pref::Sandbox::sleepOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setCollisionOption( %flag )
{
    $pref::Sandbox::collisionOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setPositionOption( %flag )
{
    $pref::Sandbox::positionOption = %flag;
    updateToolboxOptions();
}

//-----------------------------------------------------------------------------

function setSortOption( %flag )
{
    $pref::Sandbox::sortOption = %flag;
    updateToolboxOptions();
}
