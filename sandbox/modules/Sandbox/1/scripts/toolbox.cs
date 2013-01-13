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
        
        // Add to list.        
        BackgroundColorSelectList.add( getStockColorName(%i), %i );
        
        // Select the color if it's the default one.
        if ( %colorName $= $defaultSceneColor )
            BackgroundColorSelectList.setSelected( %i );
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

function updateToolboxOptions()
{
    // Finish if the sandbox scene is not available.
    if ( !isObject(SandboxScene) )
        return;
        
    // Set the scene color.
    SandboxScene.BackgroundColor = getStockColorName($activeSceneColor);
    SandboxScene.UseBackgroundColor = true;        
    
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
