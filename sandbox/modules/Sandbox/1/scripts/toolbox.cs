//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$metricsOption = false;
$jointsOption = false;
$aabbOption = false;
$oobbOption = false;
$sleepOption = false;
$collisionOption = false;
$positionOption = false;
$sortOption = false;

$defaultSceneColor = "DimGray";

//-----------------------------------------------------------------------------

function initializeToolbox()
{   
    // Populate the stock colors.
    %colorCount = getStockColorCount();
    //%colorCount = 12;
    for ( %i = 0; %i < %colorCount; %i++ )
    {
        // Fetch stock color name.
        %colorName = getStockColorName(%i);
        
        // Add to the list.        
        BackgroundColorSelectList.add( getStockColorName(%i), %i );
        
        // Select the color if it's the default one.
        if ( %colorName $= $defaultSceneColor )
            BackgroundColorSelectList.setSelected( %i );
    }
    
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
    
    // Set the fullscreen flag.
    $sandboxFullscreen = $pref::Video::fullScreen;
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
    ToySelectList.setFirstResponder();
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
    setScreenMode( GetWord( %resolution , 0 ), GetWord( %resolution, 1 ), GetWord( %resolution, 2 ), $sandboxFullscreen );
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
    
    // Set the full-screen mode appropriately.
    if ( isFullScreen() != $sandboxFullscreen )
        toggleFullScreen();
    
    // Set option.
    if ( $metricsOption )
        SandboxScene.setDebugOn( 0 );
    else
        SandboxScene.setDebugOff( 0 );

    // Set option.
    if ( $jointsOption )
        SandboxScene.setDebugOn( 1 );
    else
        SandboxScene.setDebugOff( 1 );        

    // Set option.
    if ( $aabbOption )
        SandboxScene.setDebugOn( 2 );
    else
        SandboxScene.setDebugOff( 2 );

    // Set option.
    if ( $oobbOption )
        SandboxScene.setDebugOn( 3 );
    else
        SandboxScene.setDebugOff( 3 );
        
    // Set option.
    if ( $sleepOption )
        SandboxScene.setDebugOn( 4 );
    else
        SandboxScene.setDebugOff( 4 );                

    // Set option.
    if ( $collisionOption )
        SandboxScene.setDebugOn( 5 );
    else
        SandboxScene.setDebugOff( 5 );
        
    // Set option.
    if ( $positionOption )
        SandboxScene.setDebugOn( 6 );
    else
        SandboxScene.setDebugOff( 6 );
        
    // Set option.
    if ( $sortOption )
        SandboxScene.setDebugOn( 7 );
    else
        SandboxScene.setDebugOff( 7 );                                       
}
