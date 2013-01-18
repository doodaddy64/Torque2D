//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------



//----------------------------------------------------------------
// Globals
//----------------------------------------------------------------
$LevelBuilderTool::CurrentWorld = "";
$LevelBuilderTool::CurrentLevel = "";

$CollisionShapesDebugId = 5;
$LevelBuilderTool::LastLauncherCollisionObjectPosition = "";

$LevelBuilderTool::GravityConstant = -9.81;
$LevelBuilderTool::GravityMultiplierHighest = 6;
$LevelBuilderTool::GravityMultiplierHigher = 4;
$LevelBuilderTool::GravityMultiplierHigh = 2;
$LevelBuilderTool::GravityMultiplierLow = 0.6;
$LevelBuilderTool::GravityMultiplierLower = 0.4;
$LevelBuilderTool::GravityMultiplierLowest = 0.2;

$LevelBuilderTool::LevelSize = "";

$LevelBuilderTool::ParallaxSpeedFastest = 20; 
$LevelBuilderTool::ParallaxSpeedFast = 10;
$LevelBuilderTool::ParallaxSpeedMedium = 5;
$LevelBuilderTool::ParallaxSpeedSlow = 2.5;
$LevelBuilderTool::ParallaxSpeedSlowest = 1.25;
$LevelBuilderTool::ParallaxSpeedNone = 0;

$LevelBuilderTool::ForegroundParallaxSpeed = "";

$LevelBuilderTool::RotationScheduleId = -1;
$LevelBuilderTool::SmoothRotating = false;
$LevelBuilderTool::SmoothRotationFrequency = 50;
$LevelBuilderTool::QuickRotateDegrees = 45;


new ScriptMsgListener(LevelBuilderToolPresenter)
{
    HintsAndTips = "";
    
    LeftView = "";
    RightView = "";
    
    LeftViewInitialized = false;
    RightViewInitialized = false;
    LevelBuilderSceneEditor = "";
    LevelBuilderSelector = "";
    
    ToolEquipped = "";
    
    RotateLeftButton = "";
    RotateRightButton = "";
    
    DuplicationStamp = "";
    
    CollisionViewEnabled = false;
    PhysicsPreviewEnabled = false;
    
    Scene = "";
    
    SelectedObjectPreviewScene = "";
    
    ForegroundAssetSelect = "";
    Background1AssetSelect = "";
    Background2AssetSelect = "";
    SkyAssetSelect = "";
};


function LevelBuilderToolPresenter::initialize(%this, %leftView, %rightView)
{
    %this.LeftView = %leftView;
    
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::Wake", "onLeftViewWake");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::Sleep", "onLeftViewSleep");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::ControlDropped", "onLeftViewControlDropped");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::DuplicationStampMove", "onLeftViewDuplicationStampMove");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::DuplicationStampDown", "onLeftViewDuplicationStampDown");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::WorldSelect", "onLeftViewWorldSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::LevelSelect", "onLeftViewLevelSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::CollisionToggle", "onLeftViewCollisionToggle");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::ZoomSelect", "onLeftViewZoomSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %leftView @ "::PhysicsPreviewToggle", "onLeftViewPhysicsPreviewToggle");
    
    %this.RightView = %rightView;    
    
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::Wake", "onRightViewWake");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::SelectionToolSelected", "onRightViewSelectionToolSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::RotateSelected", "onRightViewRotateSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::DuplicateSelected", "onRightViewDuplicateSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::BringToFrontSelected", "onRightViewBringToFrontSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::SendToBackSelected", "onRightViewSendToBackSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::DeleteSelected", "onRightViewDeleteSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::SelectedObjectEditSelected", "onRightViewSelectedObjectEditSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::GravityUpSelected", "onRightViewGravityUpSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::GravityNoneSelected", "onRightViewGravityNoneSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::GravityDownSelected", "onRightViewGravityDownSelected");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::GravityStrengthSelect", "onRightViewGravityStrengthSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::LevelSizeSelect", "onRightViewLevelSizeSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::BackgroundFormatTileSelect", "onRightViewBackgroundFormatTileSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::BackgroundFormatStretchSelect", "onRightViewBackgroundFormatStretchSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::ForegroundParallaxSpeedSelect", "onRightViewForegroundParallaxSpeedSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::Background1ParallaxSpeedSelect", "onRightViewBackground1ParallaxSpeedSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::Background2ParallaxSpeedSelect", "onRightViewBackground2ParallaxSpeedSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::SkyParallaxSpeedSelect", "onRightViewSkyParallaxSpeedSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::ForegroundAssetSelect", "onRightViewForegroundAssetSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::Background1AssetSelect", "onRightViewBackground1AssetSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::Background2AssetSelect", "onRightViewBackground2AssetSelect");
    PhysicsLauncherToolsEventManager.subscribe(%this, %rightView @ "::SkyAssetSelect", "onRightViewSkyAssetSelect");
    
    %this.Scene = "MainScene";
    
    %this.ForegroundAssetSelect = new ScriptObject(ForegroundAssetSelect);
    %this.Background1AssetSelect = new ScriptObject(Background1AssetSelect);
    %this.Background2AssetSelect = new ScriptObject(Background2AssetSelect);
    %this.SkyAssetSelect = new ScriptObject(SkyAssetSelect);
}

function LevelBuilderToolPresenter::onLeftViewWake(%this)
{
    if (!isObject(%this.HintsAndTips))
        %this.HintsAndTips = createHelpMarqueeObject("LevelBuilderToolTips", 10000, "{PhysicsLauncherTools}");

    %this.HintsAndTips.openHelpSet("LevelBuilderHelp");
    %this.HintsAndTips.start();
    
    if (!%this.LeftViewInitialized)
        %this.initializeLeftView();
        
    %this.refreshWorldList();

    %this.onRightViewSelectionToolSelected();
}

/// <summary>
/// Initializes the Left View of the Level Builder Tool
/// </summary>
function LevelBuilderToolPresenter::initializeLeftView(%this)
{
    %layerMask = 0;
    %layerMask = addBitToMask(%layerMask, $PhysicsLauncherTools::WorldObjectAndProjectileSceneLayer);
    %layerMask = addBitToMask(%layerMask, $PhysicsLauncherTools::LauncherCollisionObjectSceneLayer);
    %this.LeftView.setSceneViewLayerMask(%layerMask);
    
    %this.LevelBuilderSceneEditor = new LevelBuilderSceneEdit(LevelBuilderSceneEditor);
    %this.LevelBuilderSceneEditor.setCameraVisibility(false);
    %this.LevelBuilderSceneEditor.setGuidesVisibility(false);
    %this.LevelBuilderSceneEditor.setGridVisibility(false);
    %this.LevelBuilderSceneEditor.setSnapToGrid(false);
    
    %this.initializeZoomOptions();
    
    %this.LeftView.setSceneViewSceneEdit(%this.LevelBuilderSceneEditor);
    
    %this.LevelBuilderSelector = new LevelBuilderSelectionTool(LevelBuilderSelector);
    %this.LevelBuilderSelector.setHoverOutlineColor(255, 127, 0, 255);
    %this.LevelBuilderSelector.setHoverOutlineWidth(3);
    %this.LevelBuilderSelector.setAllowSizing(false);
    %this.LevelBuilderSelector.setAllowMultipleSelection(false);
    %this.LevelBuilderSceneEditor.addTool(%this.LevelBuilderSelector, true);

    %this.LeftViewInitialized = true;
}

function LevelBuilderToolPresenter::onRightViewWake(%this)
{
    if (!%this.RightViewInitialized)
        %this.initializeRightView();
        
    %this.tick = new GuiSpriteCtrl()
    {
        Position = "706 222";
        Extent = "47 57";
        Image="{EditorAssets}:indicationArrowImage";
    };
    
    EditorShellGui.add(%this.tick);
        
    %this.RightView.selectSelectionTool();
    %this.setSelectedObjectManipulationActive(false);
    
    %this.RightView.clearObjectList();

    for (%i = 0; %i < WorldObjectSet.getCount(); %i++)
    {
        %container = new GuiControl()
        {
            Extent = "70 70";
            MinExtent = "2 2";
            HorizSizing = "relative";
            VertSizing = "relative";
            Profile = "GuiLargePanelContainer";
        };
        
        %sceneObjectControl = new GuiSceneObjectCtrl()
        {
            position = "2 2";
            extent = "66 66";
            MinExtent = "2 2";
            HorizSizing = "relative";
            VertSizing = "relative";
            class = "ObjectPreview";
            Profile = "GuiTransparentProfile";
        };
        
        %object = WorldObjectSet.getObject(%i);

        %sceneObjectControl.setup(%object);
        
        %prefabPreviewMouseEventCtrl = new GuiMouseEventCtrl()
        {
            position = "0 0";
            extent = "66 66";
            MinExtent = "2 2";
            HorizSizing = "relative";
            VertSizing = "relative";
            class = "PrefabPreviewMouseEventCtrl";
            Profile = "GuiTransparentProfile";
        };
        
        %sceneObjectControl.add(%prefabPreviewMouseEventCtrl);
        
        %container.add(%sceneObjectControl);
        
        %this.RightView.addToObjectList(%container);
    }
    
    // Force-refresh scroller to react to Dynamic Array resizing
    %this.RightView.refreshObjectScroll();    
    
    // Refresh Gravity Tab
    %this.refreshGravityTab();
    
    // Refresh Backgrounds Tab
    %this.refreshBackgroundsTab();
}

function LevelBuilderToolPresenter::refreshGravityTab(%this)
{
    %gravity = getWord(%this.Scene.getGravity(), 1);
    
    if (%gravity > 0)
        %this.RightView.selectGravityUp();
    else if (%gravity < 0)
        %this.RightView.selectGravityDown();
    else
        %this.RightView.selectGravityNone();
        
    %this.refreshGravityStrength();
}

function LevelBuilderToolPresenter::refreshGravityStrength(%this)
{
    %gravityMultiplier = getWord(%this.Scene.getGravity(), 1) / $LevelBuilderTool::GravityConstant;
    
    %strengthText = "";
    
    switch (%gravityMultiplier)
    {
        case $LevelBuilderTool::GravityMultiplierHighest :
            %strengthText = "Highest";
        
        case $LevelBuilderTool::GravityMultiplierHigher :
            %strengthText = "Higher";
            
        case $LevelBuilderTool::GravityMultiplierHigh :
            %strengthText = "High";
            
        case $LevelBuilderTool::GravityMultiplierLow :
            %strengthText = "Low";
            
        case $LevelBuilderTool::GravityMultiplierLower :
            %strengthText = "Lower";
            
        case $LevelBuilderTool::GravityMultiplierLowest :
            %strengthText = "Lowest";
            
        default:
            %strengthText = "Normal";
    }
    
    %this.RightView.selectGravityStrength(%strengthText);
}

function LevelBuilderToolPresenter::refreshBackgroundsTab(%this)
{
    %this.refreshLevelSize();
    
    if (Sky.callOnBehaviors(getTileable))
        %this.RightView.selectBackgroundFormatTile();
    else
        %this.RightView.selectBackgroundFormatStretch();
        
    %this.applyBackgroundFormat();
    
    %this.RightView.setForegroundAssetDisplay(AssetDatabase.getAssetName(Foreground.getImage()));
    %this.RightView.selectForegroundParallaxSpeed(%this.convertParallaxSpeedToText(Foreground.callOnBehaviors(getHorizontalScrollSpeed)));
    
    %this.RightView.setBackground1AssetDisplay(AssetDatabase.getAssetName(Background1.getImage()));
    %this.RightView.selectBackground1ParallaxSpeed(%this.convertParallaxSpeedToText(Background1.callOnBehaviors(getHorizontalScrollSpeed)));
    
    %this.RightView.setBackground2AssetDisplay(AssetDatabase.getAssetName(Background2.getImage()));
    %this.RightView.selectBackground2ParallaxSpeed(%this.convertParallaxSpeedToText(Background2.callOnBehaviors(getHorizontalScrollSpeed)));
    
    %this.RightView.setSkyAssetDisplay(AssetDatabase.getAssetName(Sky.getImage()));
    %this.RightView.selectSkyParallaxSpeed(%this.convertParallaxSpeedToText(Sky.callOnBehaviors(getHorizontalScrollSpeed)));
}

function LevelBuilderToolPresenter::convertParallaxSpeedToText(%this, %parallaxSpeed)
{
    switch (%parallaxSpeed)
    {
        case $LevelBuilderTool::ParallaxSpeedFastest :
            return "Fastest";
            
        case $LevelBuilderTool::ParallaxSpeedFast :
            return "Fast";
            
        case $LevelBuilderTool::ParallaxSpeedMedium :
            return "Medium";
            
        case $LevelBuilderTool::ParallaxSpeedSlow :
            return "Slow";
            
        case $LevelBuilderTool::ParallaxSpeedSlowest :
            return "Slowest";
            
        case $LevelBuilderTool::ParallaxSpeedNone :
            return "None";
        
        default:
    }
}

/// <summary>
/// Applies the Background Format to the Scrollers
/// </summary>
function LevelBuilderToolPresenter::applyBackgroundFormat(%this)
{
    %backgroundsFormatTileable = %this.RightView.getBackgroundFormatIsTile();
    
    Sky.callOnBehaviors(setTileable, %backgroundsFormatTileable);
    Background2.callOnBehaviors(setTileable, %backgroundsFormatTileable);
    Background1.callOnBehaviors(setTileable, %backgroundsFormatTileable);
    Foreground.callOnBehaviors(setTileable, %backgroundsFormatTileable);
    
    %levelSize = Sky.getSize();
    
    if (!%backgroundsFormatTileable)
    {
        %foregroundRepeatX = 1 / (((%levelSize.x - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * Foreground.callOnBehaviors(getHorizontalScrollSpeed));
        Foreground.setRepeatX(%foregroundRepeatX);
        
        %background1RepeatX = 1 / (((%levelSize.x - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * Background1.callOnBehaviors(getHorizontalScrollSpeed));
        Background1.setRepeatX(%background1RepeatX);
        
        %background2RepeatX = 1 / (((%levelSize.x - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * Background2.callOnBehaviors(getHorizontalScrollSpeed));
        Background2.setRepeatX(%background2RepeatX);
        
        %skyRepeatX = 1 / (((%levelSize.x - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * Sky.callOnBehaviors(getHorizontalScrollSpeed));
        Sky.setRepeatX(%skyRepeatX);
    }
    else
    {
        Foreground.setRepeatX(1);
        Background1.setRepeatX(1);
        Background2.setRepeatX(1);
        Sky.setRepeatX(1);
    }
}

function LevelBuilderToolPresenter::refreshLevelSize(%this)
{
    %levelSizeInPixels = VectorScale(%this.Scene.levelSize, $PhysicsLauncherTools::PixelsPerMeter);
    
    %this.RightView.selectLevelSize(mRound(getWord(%levelSizeInPixels, 0)) @ "x" @ mRound(getWord(%levelSizeInPixels, 1)));
}

function LevelBuilderToolPresenter::initializeRightView(%this)
{
    %this.SelectedObjectPreviewScene = new Scene();

    %this.initializeGravityStrengthList();
    %this.initializeLevelSizeList();
    %this.initializeParallaxSpeedList("Foreground");
    %this.initializeParallaxSpeedList("Background1");
    %this.initializeParallaxSpeedList("Background2");
    %this.initializeParallaxSpeedList("Sky");    
    
    %this.RightView.selectSceneTab("Objects");
    
    %this.RightViewInitialized = true;
}

function LevelBuilderToolPresenter::initializeGravityStrengthList(%this)
{
    %this.RightView.addToGravityStrengthList("Highest");
    %this.RightView.addToGravityStrengthList("Higher");
    %this.RightView.addToGravityStrengthList("High");
    %this.RightView.addToGravityStrengthList("Normal");
    %this.RightView.addToGravityStrengthList("Low");
    %this.RightView.addToGravityStrengthList("Lower");
    %this.RightView.addToGravityStrengthList("Lowest");
}

function LevelBuilderToolPresenter::initializeLevelSizeList(%this)
{
    %this.RightView.addToLevelSizeList("1024x1024");
    %this.RightView.addToLevelSizeList("2048x1024");
    %this.RightView.addToLevelSizeList("2048x2048");
    %this.RightView.addToLevelSizeList("1024x2048");
}

function LevelBuilderToolPresenter::initializeParallaxSpeedList(%this, %layer)
{
    %this.RightView.call(addTo @ %layer @ ParallaxSpeedList, "Fastest");
    %this.RightView.call(addTo @ %layer @ ParallaxSpeedList, "Fast");
    %this.RightView.call(addTo @ %layer @ ParallaxSpeedList, "Medium");
    %this.RightView.call(addTo @ %layer @ ParallaxSpeedList, "Slow");
    %this.RightView.call(addTo @ %layer @ ParallaxSpeedList, "Slowest");
    %this.RightView.call(addTo @ %layer @ ParallaxSpeedList, "None");
}

function LevelBuilderToolPresenter::onRightViewRotateSelected(%this)
{
    if (%this.ToolEquipped $= "DuplicationTool")
    {
        %this.DuplicationStamp.delete();    
        %this.LeftView.setAcceptDuplicationStamp(false);
    }
    else
        %this.LevelBuilderSceneEditor.clearActiveTool();
    
    %acquiredObject = %this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0);
    %acquiredObjectTopLeft = %acquiredObject.getPosition();
    %acquiredObjectTopLeft.x -= %acquiredObject.getSizeX() / 2;
    %acquiredObjectTopLeft.y += %acquiredObject.getSizeY() / 2;
    %acquiredObjectGuiPosition = %this.LeftView.getSceneViewWindowPoint(%acquiredObjectTopLeft);
    
    %extent = "32 32";
    %zoomMultiplier = %this.getZoomMultiplier();
    %extent.x *= %zoomMultiplier;
    %extent.y *= %zoomMultiplier;
    
    %rotateLeftButtonPosition = VectorSub(%acquiredObjectGuiPosition, VectorScale(%extent, 0.5));
    %rotateLeftButtonPosition.x = mRound(%rotateLeftButtonPosition.x);
    %rotateLeftButtonPosition.y = mRound(%rotateLeftButtonPosition.y);
    
    %this.RotateLeftButton = new GuiSpriteCtrl()
    {
       position = %rotateLeftButtonPosition;
       HorizSizing = "relative";
       VertSizing = "relative";
       MinExtent = "2 2";
       extent = %extent;
       Image = "{PhysicsLauncherTools}:RotateLeftArrow_normal";
    };
    
    %rotateLeftMouseEvent = new GuiMouseEventCtrl()
    {
        HorizSizing = "relative";
        VertSizing = "relative";
        MinExtent = "2 2";
        extent = %extent;
        class = "RotateLeftButton";
    };
    
    %this.RotateLeftButton.add(%rotateLeftMouseEvent);
    
    %this.LeftView.addToSceneView(%this.RotateLeftButton);
    
    %rotateRightButtonPosition = %rotateLeftButtonPosition;
    %rotateRightButtonPosition.x += (%acquiredObject.getSizeX() * $PhysicsLauncherTools::PixelsPerMeter) * %zoomMultiplier;
    
    %this.RotateRightButton = new GuiSpriteCtrl()
    {
       position = %rotateRightButtonPosition;
       HorizSizing = "relative";
       VertSizing = "relative";
       MinExtent = "2 2";
       extent = %extent;
       Image = "{PhysicsLauncherTools}:RotateRightArrow_normal";
    };
    
    %rotateRightMouseEvent = new GuiMouseEventCtrl()
    {
        HorizSizing = "relative";
        VertSizing = "relative";
        MinExtent = "2 2";
        extent = %extent;
        class = "RotateRightButton";
    };
    
    %this.RotateRightButton.add(%rotateRightMouseEvent);
    
    %this.LeftView.addToSceneView(%this.RotateRightButton);
    
    %this.ToolEquipped = "RotationTool";
}

/// <summary>
/// Handles a Mouse Down Event on the Rotate Left Button
/// </summary>
/// <param="modifier">Modifier present, if any, during Down event</param>
/// <param="mousePoint">The Point at which the Down event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function RotateLeftButton::onMouseDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
   // Schedule 1 degree rotation with double delay to allow for a potential quick rotate
   $LevelBuilderTool::RotationScheduleId = LevelBuilderToolPresenter.schedule($LevelBuilderTool::SmoothRotationFrequency * 3, RotateSelectedObject, 1, true);
}

/// <summary>
/// Handles a Mouse Up Event on the Rotate Left Button
/// </summary>
/// <param="modifier">Modifier present, if any, during Up event</param>
/// <param="mousePoint">The Point at which the Up event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function RotateLeftButton::onMouseUp(%this, %modifier, %mousePoint, %mouseClickCount)
{
    if ($LevelBuilderTool::RotationScheduleId == -1)
        return;

    cancel($LevelBuilderTool::RotationScheduleId);
   
    $LevelBuilderTool::RotationScheduleId = -1;
   
    if (!$LevelBuilderTool::SmoothRotating)
      LevelBuilderToolPresenter.RotateSelectedObject($LevelBuilderTool::QuickRotateDegrees, false);
    else
      $LevelBuilderTool::SmoothRotating = false;
}

/// <summary>
/// Handles a Mouse Leave Event on the Rotate Left Button
/// </summary>
/// <param="modifier">Modifier present, if any, during the Leave event</param>
/// <param="mousePoint">The Point at which the Leave event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function RotateLeftButton::onMouseLeave(%this, %modifier, %mousePoint, %mouseClickCount)
{
    LevelBuilderToolPresenter.cancelRotation();
}

/// <summary>
/// Rotates the currently acquired Object by the given Angle (in Degrees)
/// </summary>
/// <param="angleInDegrees">Amount to rotate (in Degrees)</param>
/// <param="continuous">True if the rotation should be scheduled to repeat</param>
function LevelBuilderToolPresenter::RotateSelectedObject(%this, %angleInDegrees, %continuous)
{
   if (%continuous)
   {
      cancel($LevelBuilderTool::RotationScheduleId);
      
      if (!$LevelBuilderTool::SmoothRotating)
         $LevelBuilderTool::SmoothRotating = true;
   }
   
   %acquiredObject = %this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0);
   
   %acquiredObject.setAngle(%acquiredObject.getAngle() + %angleInDegrees);
   
   if (%acquiredObject.getAngle() > 360)
      %acquiredObject.setAngle(%acquiredObject.getAngle() - 360);
   else if (%acquiredObject.getAngle() < 0)
      %acquiredObject.setAngle(360 + %acquiredObject.getAngle());
      
   if (%continuous)
      $LevelBuilderTool::RotationScheduleId = %this.schedule($LevelBuilderTool::SmoothRotationFrequency, RotateSelectedObject, %angleInDegrees, true);
}

/// <summary>
/// Cancels a Rotation occurring on the Selected Object
/// </summary>
function LevelBuilderToolPresenter::cancelRotation(%this)
{
    cancel($LevelBuilderTool::RotationScheduleId);
   
    $LevelBuilderTool::RotationScheduleId = -1;

    $LevelBuilderTool::SmoothRotating = false;
}

/// <summary>
/// Handles a Mouse Down Event on the Rotate Right Button
/// </summary>
/// <param="modifier">Modifier present, if any, during Down event</param>
/// <param="mousePoint">The Point at which the Down event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function RotateRightButton::onMouseDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
   // Schedule 1 degree rotation with double delay to allow for a potential quick rotate
   $LevelBuilderTool::RotationScheduleId = LevelBuilderToolPresenter.schedule($LevelBuilderTool::SmoothRotationFrequency * 3, RotateSelectedObject, -1, true);
}

/// <summary>
/// Handles a Mouse Up Event on the Rotate Right Button
/// </summary>
/// <param="modifier">Modifier present, if any, during Up event</param>
/// <param="mousePoint">The Point at which the Up event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function RotateRightButton::onMouseUp(%this, %modifier, %mousePoint, %mouseClickCount)
{
    if ($LevelBuilderTool::RotationScheduleId == -1)
        return;
        
    cancel($LevelBuilderTool::RotationScheduleId);

    $LevelBuilderTool::RotationScheduleId = -1;

    if (!$LevelBuilderTool::SmoothRotating)
      LevelBuilderToolPresenter.RotateSelectedObject(-$LevelBuilderTool::QuickRotateDegrees, false);
    else
      $LevelBuilderTool::SmoothRotating = false;
}

/// <summary>
/// Handles a Mouse Leave Event on the Rotate Left Button
/// </summary>
/// <param="modifier">Modifier present, if any, during the Leave event</param>
/// <param="mousePoint">The Point at which the Leave event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function RotateRightButton::onMouseLeave(%this, %modifier, %mousePoint, %mouseClickCount)
{
    LevelBuilderToolPresenter.cancelRotation();
}

function LevelBuilderToolPresenter::onRightViewDuplicateSelected(%this)
{
    if (%this.ToolEquipped $= "RotationTool")
    {
        %this.RotateLeftButton.delete();
        %this.RotateRightButton.delete();
    }
    else
        %this.LevelBuilderSceneEditor.clearActiveTool();
        
    %this.LeftView.setAcceptDuplicationStamp(true);
    
    %t2dObjectCtrl = new GuiSceneObjectCtrl()
    {
        position = "-512 -512";
        HorizSizing = "relative";
        VertSizing = "relative";
        MinExtent = "2 2";
        class="ObjectPreview";
        Profile="GuiModelessDialogProfile";
    };
    
    %t2dObjectCtrl.setup(%this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0));
    %sceneObject = %t2dObjectCtrl.getSceneObject();
    
    %zoomMultiplier = %this.getZoomMultiplier();
    
    if (%sceneObject.getSizeY() > %sceneObject.getSizeX())
        %t2dObjectCtrl.setExtent(%sceneObject.getSizeY() * %zoomMultiplier, %sceneObject.getSizeY() * %zoomMultiplier);
    else
        %t2dObjectCtrl.setExtent(%sceneObject.getSizeX() * %zoomMultiplier, %sceneObject.getSizeX() * %zoomMultiplier);
        
    %dimTexture = new GuiSpriteCtrl()
    {
        HorizSizing = "relative";
        VertSizing = "relative";
        MinExtent = "2 2";
        Image = "{PhysicsLauncherTools}:DimImage";
        Profile = "GuiModelessDialogProfile";
        extent= %t2dObjectCtrl.getExtent();
    };
    
    %t2dObjectCtrl.add(%dimTexture);
    
    %this.LeftView.addDuplicationStamp(%t2dObjectCtrl);
    
    %this.ToolEquipped = "DuplicationTool";
}

function LevelBuilderToolPresenter::onRightViewBringToFrontSelected(%this)
{
    %object = %this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0);
    %object.setSceneLayerDepthFront();
}

function LevelBuilderToolPresenter::onRightViewSendToBackSelected(%this)
{
    %object = %this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0);
    %object.setSceneLayerDepthBack();
}

function LevelBuilderToolPresenter::onRightViewDeleteSelected(%this)
{
    // Go back to Selection state if we were Rotating/Duplicating
    if (%this.ToolEquipped !$= "SelectionTool")
    {
        %this.RightView.selectSelectionTool();
        %this.onRightViewSelectionToolSelected();
    }
        
    %this.LevelBuilderSceneEditor.deleteAcquiredObjects();
}

function LevelBuilderToolPresenter::onRightViewSelectedObjectEditSelected(%this)
{
    %acquiredObject = %this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0);
    
    if (%acquiredObject.getSceneLayer() == $PhysicsLauncherTools::LauncherCollisionObjectSceneLayer)
    {
        LauncherTool.load();
        LauncherTool.selectLauncherObject(LauncherSet.findByInternalName(LauncherSceneGroup.getInternalName()));
    }
    else
    {
        WorldObjectTool.load();
        WorldObjectTool.selectObject(%this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0).getPrefab());
    }
}

/// <summary>
/// Handles a Mouse Drag Event in the Objects Tab Page
/// </summary>
/// <param="modifier">Modifier present, if any, during Drag</param>
/// <param="mousePoint">The Point at which the Drag began</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function PrefabPreviewMouseEventCtrl::onMouseDragged(%this, %modifier, %mousePoint, %mouseClickCount)
{
    if (!%this.getParent().isActive())
        return;
        
    %position = %mousePoint;
    %halfParentWidth = %this.getParent().getExtent().x / 2;
    %halfParentHeight = %this.getParent().getExtent().y / 2;
    %position.x -= %halfParentWidth;
    %position.y -= %halfParentHeight;
    
    %dragControl = new GuiDragAndDropControl() {
        canSaveDynamicFields = "0";
        Profile = "GuiDefaultProfile";
        HorizSizing = "relative";
        VertSizing = "relative";
        Position = %position;
        Extent = "64 64";
        MinExtent = "2 2";
        canSave = "1";
        Visible = "1";
        hovertime = "1000";
        deleteOnMouseUp = true;
    };
    
    %t2dObjectCtrl = new GuiSceneObjectCtrl()
    {
        position = "0 0";
        extent = "64 64";
        MinExtent = "2 2";
        HorizSizing = "relative";
        VertSizing = "relative";
    };
    
    %t2dObjectCtrl.setSceneObject(%this.getParent().getSceneObject());
        
    %dragControl.add(%t2dObjectCtrl);
    
    EditorShellGui.add(%dragControl);
    
    %dragControl.startDragging(%halfParentWidth, %halfParentHeight);
}

function LevelBuilderToolPresenter::onRightViewGravityUpSelected(%this)
{
    %gravity = getWord(%this.Scene.getGravity(), 1);
    
    if (%gravity < 0)
        %this.Scene.setGravity(0, %gravity * -1);
        
    if (!%this.RightView.getGravityStrengthListActive())
        %this.RightView.setGravityStrengthListActive(true);
}

function LevelBuilderToolPresenter::onRightViewGravityNoneSelected(%this)
{
    %this.Scene.setGravity(0, 0);
    %this.RightView.setGravityStrengthListActive(false);
}

function LevelBuilderToolPresenter::onRightViewGravityDownSelected(%this)
{
    %gravity = getWord(%this.Scene.getGravity(), 1);
    
    if (%gravity > 0)
        %this.Scene.setGravity(0, %gravity * -1);
        
    if (!%this.RightView.getGravityStrengthListActive())
        %this.RightView.setGravityStrengthListActive(true);
}

function LevelBuilderToolPresenter::onRightViewGravityStrengthSelect(%this)
{
    %gravityMultiplier = 1;
    
    switch$ (%this.RightView.getSelectedGravityStrength())
    {
        case "Highest":
            %gravityMultiplier = $LevelBuilderTool::GravityMultiplierHighest;
        
        case "Higher":
            %gravityMultiplier = $LevelBuilderTool::GravityMultiplierHigher;
            
        case "High":
            %gravityMultiplier = $LevelBuilderTool::GravityMultiplierHigh;
            
        case "Low":
            %gravityMultiplier = $LevelBuilderTool::GravityMultiplierLow;
            
        case "Lower":
            %gravityMultiplier = $LevelBuilderTool::GravityMultiplierLower;
            
        case "Lowest":
            %gravityMultiplier = $LevelBuilderTool::GravityMultiplierLowest;
            
        default:
    }
    
    %this.Scene.setGravity(0, ((getWord(%this.Scene.getGravity(), 1) > 0) ? -$LevelBuilderTool::GravityConstant : $LevelBuilderTool::GravityConstant) * %gravityMultiplier);
}

function LevelBuilderToolPresenter::onRightViewLevelSizeSelect(%this)
{
    %levelSize = %this.RightView.getSelectedLevelSize();
    
    if (%levelSize $= $LevelBuilderTool::LevelSize)
        return;
        
    %oldLevelSize = %this.Scene.levelSize;
        
    switch$ (%levelSize)
    {
        case "1024x1024":
            %this.Scene.levelSize = "16 16";
        
        case "2048x1024":
            %this.Scene.levelSize = "32 16";
        
        case "2048x2048":
            %this.Scene.levelSize = "32 32";
        
        case "1024x2048":
            %this.Scene.levelSize = "16 32";
        
        default:
    }
    
    if ($LevelBuilderTool::LevelSize !$= "")
    {
        %levelSizeChangeXRatio = %this.Scene.levelSize.x / %oldLevelSize.x;
        %levelSizeChangeYRatio = %this.Scene.levelSize.y / %oldLevelSize.y;
        
        %sceneObjectCount = %this.Scene.getSceneObjectCount();
        
        // Reposition Scene Objects
        for (%i = 0; %i < %sceneObjectCount; %i++)
        {
            %sceneObject = %this.Scene.getSceneObject(%i);
            
            %sceneObject.setPositionX(%sceneObject.getPositionX() * %levelSizeChangeXRatio);
            %sceneObject.setPositionY(%sceneObject.getPositionY() * %levelSizeChangeYRatio);
        }
        
        Sky.setSize(%this.Scene.levelSize);
        Background1.setSize(%this.Scene.levelSize);
        Background2.setSize(%this.Scene.levelSize);
        Foreground.setSize(%this.Scene.levelSize);
    }
    
    %this.onLeftViewZoomSelect();
    
    %this.LeftView.scrollSceneScrollToBottom();
    %this.LeftView.setSceneScrollScrollPosition((%this.LeftView.getSceneScrollSize().x / 2) - (%this.LeftView.getSceneScrollSize().x / 2), 
        %this.LeftView.getSceneScrollScrollPositionY());
    
    $LevelBuilderTool::LevelSize = %levelSize;
}

function LevelBuilderToolPresenter::onRightViewBackgroundFormatTileSelect(%this)
{
    %this.applyBackgroundFormat();
}

function LevelBuilderToolPresenter::onRightViewBackgroundFormatStretchSelect(%this)
{
    %this.applyBackgroundFormat();
}

function LevelBuilderToolPresenter::onRightViewForegroundParallaxSpeedSelect(%this)
{
    %selectedForegroundParallaxSpeed = %this.RightView.getSelectedForegroundParallaxSpeed();
    
    if (%selectedForegroundParallaxSpeed $= $LevelBuilderTool::ForegroundParallaxSpeed)
        return;
        
    if ($LevelBuilderTool::ForegroundParallaxSpeed !$= "")
    {    
        %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedNone;        
        
        switch$ (%selectedForegroundParallaxSpeed)
        {
            case "Fastest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFastest;
                
            case "Fast":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFast;
                
            case "Medium":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedMedium;
                
            case "Slow":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlow;
                
            case "Slowest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlowest;
            
            default:
        }
        
        Foreground.callOnBehaviors(setHorizontalScrollSpeed, %parallaxSpeed);
        
        if (!%this.RightView.getBackgroundFormatIsTile())
        {
            %repeatX = 1 / (((Foreground.getSizeX() - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * %parallaxSpeed);
            Foreground.setRepeatX(%repeatX);
        }
    }
        
    $LevelBuilderTool::ForegroundParallaxSpeed = %selectedForegroundParallaxSpeed;
}

function LevelBuilderToolPresenter::onRightViewBackground1ParallaxSpeedSelect(%this)
{
    %selectedBackground1ParallaxSpeed = %this.RightView.getSelectedBackground1ParallaxSpeed();
    
    if (%selectedBackground1ParallaxSpeed $= $LevelBuilderTool::Background1ParallaxSpeed)
        return;
        
    if ($LevelBuilderTool::Background1ParallaxSpeed !$= "")
    {    
        %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedNone;        
        
        switch$ (%selectedBackground1ParallaxSpeed)
        {
            case "Fastest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFastest;
                
            case "Fast":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFast;
                
            case "Medium":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedMedium;
                
            case "Slow":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlow;
                
            case "Slowest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlowest;
            
            default:
        }
        
        Background1.callOnBehaviors(setHorizontalScrollSpeed, %parallaxSpeed);
        
        if (!%this.RightView.getBackgroundFormatIsTile())
        {
            %repeatX = 1 / (((Background1.getSizeX() - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * %parallaxSpeed);
            Background1.setRepeatX(%repeatX);
        }
    }
        
    $LevelBuilderTool::Background1ParallaxSpeed = %selectedBackground1ParallaxSpeed;
}

function LevelBuilderToolPresenter::onRightViewBackground2ParallaxSpeedSelect(%this)
{
    %selectedBackground2ParallaxSpeed = %this.RightView.getSelectedBackground2ParallaxSpeed();
    
    if (%selectedBackground2ParallaxSpeed $= $LevelBuilderTool::Background2ParallaxSpeed)
        return;
        
    if ($LevelBuilderTool::Background2ParallaxSpeed !$= "")
    {    
        %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedNone;        
        
        switch$ (%selectedBackground2ParallaxSpeed)
        {
            case "Fastest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFastest;
                
            case "Fast":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFast;
                
            case "Medium":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedMedium;
                
            case "Slow":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlow;
                
            case "Slowest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlowest;
            
            default:
        }
        
        Background2.callOnBehaviors(setHorizontalScrollSpeed, %parallaxSpeed);
        
        if (!%this.RightView.getBackgroundFormatIsTile())
        {
            %repeatX = 1 / (((Background2.getSizeX() - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * %parallaxSpeed);
            Background2.setRepeatX(%repeatX);
        }
    }
        
    $LevelBuilderTool::Background2ParallaxSpeed = %selectedBackground2ParallaxSpeed;
}

function LevelBuilderToolPresenter::onRightViewSkyParallaxSpeedSelect(%this)
{
    %selectedSkyParallaxSpeed = %this.RightView.getSelectedSkyParallaxSpeed();
    
    if (%selectedSkyParallaxSpeed $= $LevelBuilderTool::SkyParallaxSpeed)
        return;
        
    if ($LevelBuilderTool::SkyParallaxSpeed !$= "")
    {    
        %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedNone;        
        
        switch$ (%selectedSkyParallaxSpeed)
        {
            case "Fastest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFastest;
                
            case "Fast":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedFast;
                
            case "Medium":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedMedium;
                
            case "Slow":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlow;
                
            case "Slowest":
                %parallaxSpeed = $LevelBuilderTool::ParallaxSpeedSlowest;
            
            default:
        }
        
        Sky.callOnBehaviors(setHorizontalScrollSpeed, %parallaxSpeed);
        
        if (!%this.RightView.getBackgroundFormatIsTile())
        {
            %repeatX = 1 / (((Sky.getSizeX() - $PhysicsLauncherTools::CameraWidthInMeters) / 2) * %parallaxSpeed);
            Sky.setRepeatX(%repeatX);
        }
    }
        
    $LevelBuilderTool::SkyParallaxSpeed = %selectedSkyParallaxSpeed;
}

function LevelBuilderToolPresenter::onRightViewForegroundAssetSelect(%this)
{
    AssetPicker.open(ImageAsset, "", "", %this.ForegroundAssetSelect);
}

function ForegroundAssetSelect::setSelectedAsset(%this, %asset)
{
    Foreground.setImage(%asset);
    LevelBuilderToolPresenter.RightView.setForegroundAssetDisplay(AssetDatabase.getAssetName(%asset));
}

function LevelBuilderToolPresenter::onRightViewBackground1AssetSelect(%this)
{
    AssetPicker.open(ImageAsset, "", "", %this.Background1AssetSelect);
}

function Background1AssetSelect::setSelectedAsset(%this, %asset)
{
    Background1.setImage(%asset);
    LevelBuilderToolPresenter.RightView.setBackground1AssetDisplay(AssetDatabase.getAssetName(%asset));
}

function LevelBuilderToolPresenter::onRightViewBackground2AssetSelect(%this)
{
    AssetPicker.open(ImageAsset, "", "", %this.Background2AssetSelect);
}

function Background2AssetSelect::setSelectedAsset(%this, %asset)
{
    Background2.setImage(%asset);
    LevelBuilderToolPresenter.RightView.setBackground2AssetDisplay(AssetDatabase.getAssetName(%asset));
}

function LevelBuilderToolPresenter::onRightViewSkyAssetSelect(%this)
{
    AssetPicker.open(ImageAsset, "", "", %this.SkyAssetSelect);
}

function SkyAssetSelect::setSelectedAsset(%this, %asset)
{
    Sky.setImage(%asset);
    LevelBuilderToolPresenter.RightView.setSkyAssetDisplay(AssetDatabase.getAssetName(%asset));
}

//--------------------------------
// Object Preview
//--------------------------------

/// <summary>
/// Sets up the Object Preview control to display the given object
/// </summary>
/// <param="sceneObject">Scene Object to display</param>
function ObjectPreview::setup(%this, %sceneObject)
{
    %sprite = new Sprite();
    %sprite.scene = %this.SelectedObjectPreviewScene;
    
    %isWorldObject = false;
    
    if (%sceneObject.getBehavior(WorldObjectBehavior))
        %isWorldObject = true;
        
    %isAnimated = false;
        
    if (%isWorldObject)
    {
        %asset = WorldObjectBuilder::getImageForState(%sceneObject, 0);  
        
        if (AssetDatabase.getAssetType(%asset) $= "AnimationAsset")
            %isAnimated = true;
    }
    else
    {
        if (%sceneObject.animation !$= "")
        {
            %asset = %sceneObject.animation;
            %isAnimated = true;
        }
        else
            %asset = %sceneObject.getImage();
    }
    
    if (%isAnimated)
    {
        %animationAsset = AssetDatabase.acquireAsset(%asset);
        %animationImageAsset = AssetDatabase.acquireAsset(%animationAsset.image);
        %sprite.setSize(%animationImageAsset.getFrameSize(0));
        AssetDatabase.releaseAsset(%animationImageAsset.getAssetId());
        %sprite.playAnimation(%animationAsset.getAssetId());
        AssetDatabase.releaseAsset(%animationAsset.getAssetId());
    }
    else
    {
        %imageAsset = AssetDatabase.acquireAsset(%asset);
        %sprite.setSize(%imageAsset.getFrameSize(0));
        %sprite.setImage(%imageAsset.getAssetId());
        AssetDatabase.releaseAsset(%imageAsset.getAssetId());
    }
    
        
    %sprite.PrefabName = %sceneObject.getName();
    
    %this.setSceneObject(%sprite);
}

function LevelBuilderToolPresenter::initializeZoomOptions(%this)
{
    %this.LeftView.addZoomOption("150%");
    %this.LeftView.addZoomOption("125%");
    %this.LeftView.addZoomOption("100%");
    %this.LeftView.addZoomOption("75%");
    %this.LeftView.addZoomOption("50%");
    %this.LeftView.addZoomOption("25%");
    
    %this.LeftView.selectZoomOption("100%");
}

function LevelBuilderToolPresenter::refreshWorldList(%this)
{
    $LevelBuilderTool::CurrentWorld = "";
    
    %this.LeftView.clearWorldList();    
    
    for (%i = 0; %i < WorldData.getCount(); %i++)
        %this.LeftView.addToWorldList(WorldData.getObject(%i).getName());
     
    // Set to the first World after Unmapped, if available
    %this.LeftView.selectWorld(WorldData.getObject(WorldData.getCount() > 1).getName(), true);
}

function LevelBuilderToolPresenter::onRightViewSelectionToolSelected(%this)
{
    if (%this.ToolEquipped $= "RotationTool")
    {
        %this.RotateLeftButton.delete();
        %this.RotateRightButton.delete();
    }
    else if (%this.ToolEquipped $= "DuplicationTool")
        %this.DuplicationStamp.delete();
    
    %this.LeftView.setAcceptDuplicationStamp(false);    
    
    %this.LevelBuilderSceneEditor.setActiveTool(%this.LevelBuilderSelector);
    
    %this.ToolEquipped = "SelectionTool";
}

function LevelBuilderToolPresenter::onLeftViewSleep(%this)
{
    %this.saveCurrentLevel(true);
    
    PhysicsLauncherTools::deleteSceneContents(%this.Scene);
    %this.Scene.delete();
    
    %this.HintsAndTips.stop();
    %this.HintsAndTips.delete();
}

function LevelBuilderToolPresenter::onLeftViewControlDropped(%this, %data)
{
    %object = %data.control.getSceneObject();    
    
    %spriteToAdd = new Sprite();
    %spriteToAdd.setPosition(%this.LeftView.getSceneViewWorldPoint(%data.position));
    %spriteToAdd.setSize(Vector2Scale(%object.getSize(), $PhysicsLauncherTools::MetersPerPixel));
    %this.LevelBuilderSceneEditor.clampObjectWithinLevel(%spriteToAdd);
    %spriteToAdd.setPrefab(%object.PrefabName);
    
    %this.Scene.add(%spriteToAdd);
}

function LevelBuilderToolPresenter::onLeftViewDuplicationStampMove(%this, %data)
{
    %position = Vector2Sub(%data.mousePoint, %data.globalPosition);
    %t2dObjectCtrl = %data.object;
    %t2dObjectCtrl.setPosition(%position.x - (%t2dObjectCtrl.getExtent().x / 2), %position.y - (%t2dObjectCtrl.getExtent().y / 2));
}

function LevelBuilderToolPresenter::onLeftViewDuplicationStampDown(%this, %data)
{
    %spriteToAdd = new Sprite();
    %spriteToAdd.setPosition(%this.LeftView.getSceneViewWorldPoint(Vector2Sub(%data.mousePoint, %data.globalPosition)));
    %sceneObject = %data.object.getSceneObject();
    %spriteToAdd.setSize(Vector2Scale(%sceneObject.getSize(), $PhysicsLauncherTools::MetersPerPixel));
    %this.LevelBuilderSceneEditor.clampObjectWithinLevel(%spriteToAdd);
    %spriteToAdd.setPrefab(%this.LevelBuilderSceneEditor.getAcquiredObjects().getObject(0).getPrefab());
    
    %this.Scene.add(%spriteToAdd);
}

function LevelBuilderToolPresenter::onLeftViewWorldSelect(%this)
{
    if ($LevelBuilderTool::CurrentWorld $= %this.LeftView.getSelectedWorld())
        return;
    
    $LevelBuilderTool::CurrentWorld = %this.LeftView.getSelectedWorld();
    %this.refreshLevelList();
    
    %this.RightView.selectSelectionTool();
    %this.onRightViewSelectionToolSelected();    
    
    %this.setSelectedObjectManipulationActive(false);
    
    %this.LevelBuilderSceneEditor.clearAcquisition();
}

function LevelBuilderToolPresenter::setSelectedObjectManipulationActive(%this, %active)
{
    %this.RightView.setRotateActive(%active);
    %this.RightView.setDuplicateActive(%active);
    %this.RightView.setBringToFrontActive(%active);
    %this.RightView.setSendToBackActive(%active);
    %this.RightView.setDeleteActive(%active);
    %this.RightView.setSelectedObjectEditActive(%active);
}

function LevelBuilderToolPresenter::refreshLevelList(%this)
{
    $LevelBuilderTool::CurrentLevel = "";
    
    %selectedWorld = %this.LeftView.getSelectedWorld();
    
    %index = 0;
    
    %level = %selectedWorld.getFieldValue("LevelList" @ %index);
    
    %this.LeftView.clearLevelList();
    
    while (%level !$= "")
    {
        %this.LeftView.addToLevelList(%level);
        %index++;
        %level = %selectedWorld.getFieldValue("LevelList" @ %index);
    }
    
    if (%index == 0)
        return;
        
    %firstLevel = %selectedWorld.getFieldValue("LevelList0");

    %this.LeftView.selectLevel(%firstLevel, false);  
    
    $LevelBuilderTool::CurrentLevel = %firstLevel;
        
    TamlRead("^gameTemplate/data/levels/" @ $LevelBuilderTool::CurrentLevel @ ".scene.taml");
    %this.LeftView.setSceneViewScene(%this.Scene);
    SlingshotLauncherBuilder::getBuilderObject(LauncherSceneGroup).setVisible(false);
    
    if (%this.CollisionViewEnabled)
        %this.Scene.setDebugOn($CollisionShapesDebugId);
}

function LevelBuilderToolPresenter::onLeftViewLevelSelect(%this)
{
    if ($LevelBuilderTool::CurrentLevel $= %this.LeftView.getSelectedLevel())
        return;
        
    %this.saveCurrentLevel();
        
    $LevelBuilderTool::CurrentLevel = %this.LeftView.getSelectedLevel();
    
    PhysicsLauncherTools::deleteSceneContents(%this.Scene);
    %this.Scene.delete();
        
    TamlRead("^gameTemplate/data/levels/" @ $LevelBuilderTool::CurrentLevel @ ".scene.taml");
    
    %this.LeftView.setSceneViewScene(%this.Scene);
    SlingshotLauncherBuilder::getBuilderObject(LauncherSceneGroup).setVisible(false);
    
    if (%this.CollisionViewEnabled)
        %this.Scene.setDebugOn($CollisionShapesDebugId);
    
    %this.RightView.selectSelectionTool();
    %this.onRightViewSelectionToolSelected();
    
    %this.setSelectedObjectManipulationActive(false);
    %this.LevelBuilderSceneEditor.clearAcquisition();
}

function LevelBuilderToolPresenter::onLeftViewCollisionToggle(%this)
{
    if (%this.CollisionViewEnabled)
    {
        %this.Scene.setDebugOff($CollisionShapesDebugId);
        %this.CollisionViewEnabled = false;
    }
    else
    {
        %this.Scene.setDebugOn($CollisionShapesDebugId);
        %this.CollisionViewEnabled = true;
    }
}

function LevelBuilderToolPresenter::onLeftViewZoomSelect(%this)
{
    %multiplier = %this.getZoomMultiplier();
    
    %levelSize = %this.Scene.levelSize;
    
    %this.LeftView.setSceneViewSize((%levelSize.x * $PhysicsLauncherTools::PixelsPerMeter) * %multiplier, 
        (%levelSize.y * $PhysicsLauncherTools::PixelsPerMeter) * %multiplier);
        
    %this.LeftView.setSceneViewCameraPositionAndSize(0, 0, %levelSize.x, %levelSize.y);
    
    %this.LeftView.refreshSceneScroll();
}

/// <summary>
/// Returns a Multiplier base on the current selection in the Zoom Dropdown
/// </summary>
function LevelBuilderToolPresenter::getZoomMultiplier(%this)
{
    %multiplier = 0;
    
    switch$ (%this.LeftView.getSelectedZoomOption())
    {
        case "150%":
            %multiplier = 1.5;
        
        case "125%":
            %multiplier = 1.25;
            
        case "75%":
            %multiplier = 0.75;
        
        case "50%":
            %multiplier = 0.5;
            
        case "25%":
            %multiplier = 0.25;
        
        default:
            %multiplier = 1;
    }
    
    return %multiplier;
}

function LevelBuilderToolPresenter::onLeftViewPhysicsPreviewToggle(%this)
{
    if (!%this.PhysicsPreviewEnabled)
    {
        // Disable World/Level Dropdowns
        %this.LeftView.setWorldListActive(false);
        %this.LeftView.setLevelListActive(false);
        
        %this.RightView.selectSelectionTool();
        %this.onRightViewSelectionToolSelected();
        %this.RightView.setSelectionToolActive(false);
        %this.setSelectedObjectManipulationActive(false);
        %this.LevelBuilderSceneEditor.clearAcquisition();
        %this.LevelBuilderSceneEditor.clearActiveTool();

        // Disable Tab Book Controls
        %this.RightView.enableSceneTabBook(false);
        
        %this.saveCurrentLevel();
        
        %this.PhysicsPreviewEnabled = true;
    }
    else
    {
        // Enable Tab Book Controls        
        %this.RightView.enableSceneTabBook(true);
        
        %this.RightView.setSelectionToolActive(true);
        %this.onRightViewSelectionToolSelected();
        
        // Enable Level/World Dropdowns
        %this.LeftView.setLevelListActive(true);
        %this.LeftView.setWorldListActive(true);
        
        %this.PhysicsPreviewEnabled = false;
    }
    
    PhysicsLauncherTools::deleteSceneContents(%this.Scene);
    %this.Scene.delete();

    TamlRead("^gameTemplate/data/levels/" @ $LevelBuilderTool::CurrentLevel @ ".scene.taml");
    
    %this.LeftView.setSceneViewScene(%this.Scene);
    
    SlingshotLauncherBuilder::getBuilderObject(LauncherSceneGroup).setVisible(false);
    
    if (%this.CollisionViewEnabled)
        %this.Scene.setDebugOn($CollisionShapesDebugId);
    
    %this.Scene.setIsEditorScene(!%this.PhysicsPreviewEnabled);
}

/// <summary>
/// Loads the Level Builder Tool
/// </summary>
function LevelBuilderToolPresenter::load(%this)
{
    EditorShellGui.clearViews();
    EditorShellGui.addView(LevelBuilderToolLeftPassiveView, "large");
    EditorShellGui.addView(LevelBuilderToolRightPassiveView, "medium");
}

/// <summary>
/// Loads the given Level in the Level Builder Tool
/// </summary>
/// <param="level">Level to be Loaded</param>
function LevelBuilderToolPresenter::loadLevel(%this, %level)
{
    %this.LeftView.selectLevel(%level, false);
}

/// <summary>
/// Saves the Current Level
/// </summary>
/// <param="leavingTool">True if the Save request is a result of Leaving the Level Builder</param>
function LevelBuilderToolPresenter::saveCurrentLevel(%this, %leavingTool)
{
    if (($LevelBuilderTool::CurrentLevel $= "") || (%leavingTool && %this.PhysicsPreviewEnabled))
        return;
        
    SlingshotLauncherBuilder::getBuilderObject(LauncherSceneGroup).setVisible(true);
    TamlWrite(%this.Scene, "^gameTemplate/data/levels/" @ $LevelBuilderTool::CurrentLevel @ ".scene.taml");
}



//--------------------------------
// Level Builder Scene Editor
//--------------------------------

/// <summary>
/// Handles Object Acquisition in the Scene View
/// </summary>
/// <param="object">Object which has been Acquired</param>
function LevelBuilderSceneEditor::onAcquireObject(%this, %object)
{
    if (%object.getSceneLayer() == $PhysicsLauncherTools::LauncherCollisionObjectSceneLayer)
    {
        LevelBuilderToolPresenter.RightView.setRotateActive(false);
        LevelBuilderToolPresenter.RightView.setDuplicateActive(false);
        LevelBuilderToolPresenter.RightView.setBringToFrontActive(false);
        LevelBuilderToolPresenter.RightView.setSendToBackActive(false);
        LevelBuilderToolPresenter.RightView.setDeleteActive(false);
        
        $LevelBuilderTool::LastLauncherCollisionObjectPosition = %object.getPosition();
        
        %builderObject = SlingshotLauncherBuilder::getBuilderObject(LauncherSceneGroup);
        
        %builderObject.setVisible(true);
        %builderObject.setDebugOn($CollisionShapesDebugId);
        
        LevelBuilderToolPresenter.RightView.setSelectedObjectNameDisplay(LauncherSceneGroup.getInternalName());  
        LevelBuilderToolPresenter.RightView.setSelectedObjectStatsVisible(false);
        LevelBuilderToolPresenter.RightView.setSelectedObjectEditText("Edit Launcher");
        
        // Setup Fork Background Preview
        LevelBuilderToolPresenter.RightView.setupLauncherBackgroundPreview(LevelBuilderToolPresenter.Scene.findObjectByInternalName($SlingshotLauncherBuilder::ForkBackgroundObjectInternalName, true));        

        // Setup Fork Foreground Preview
        LevelBuilderToolPresenter.RightView.setupSelectedObjectPreview(LevelBuilderToolPresenter.Scene.findObjectByInternalName($SlingshotLauncherBuilder::ForkForegroundObjectInternalName, true));
    }
    else
    {
        LevelBuilderToolPresenter.RightView.setSelectedObjectNameDisplay(%object.getInternalName());
        
        LevelBuilderToolPresenter.RightView.setSelectedObjectStatsVisible(true);
        
        LevelBuilderToolPresenter.RightView.setSelectedObjectMass(WorldObjectBuilder::getMass(%object));
        LevelBuilderToolPresenter.RightView.setSelectedObjectFriction(WorldObjectBuilder::getFrictionLevel(%object));
        LevelBuilderToolPresenter.RightView.setSelectedObjectBounce(WorldObjectBuilder::getRestitutionLevel(%object));
        LevelBuilderToolPresenter.RightView.setSelectedObjectHitPoints(WorldObjectBuilder::getHitPoints(%object));
        
        LevelBuilderToolPresenter.RightView.setSelectedObjectEditText("Edit Object");
        
        LevelBuilderToolPresenter.RightView.setupSelectedObjectPreview(%object);
        
        LevelBuilderToolPresenter.RightView.setRotateActive(true);
        LevelBuilderToolPresenter.RightView.setDuplicateActive(true);
        LevelBuilderToolPresenter.RightView.setBringToFrontActive(true);
        LevelBuilderToolPresenter.RightView.setSendToBackActive(true);
        LevelBuilderToolPresenter.RightView.setDeleteActive(true);
    }
    
    LevelBuilderToolPresenter.RightView.setSelectedObjectEditActive(true);
}

/// <summary>
/// Handles a Change in the Position of the Acquired Object
/// </summary>
/// <param="object">Object which has been moved</param>
function LevelBuilderSceneEditor::onObjectSpatialChanged(%this, %object)
{
    %this.clampObjectWithinLevel(%object);
    
    if (%object.getSceneLayer() != $PhysicsLauncherTools::LauncherCollisionObjectSceneLayer)
        return;
    
    %sceneObjectGroupCount = LauncherSceneGroup.getCount();
    
    %positionDelta = Vector2Sub(%object.getPosition(), $LevelBuilderTool::LastLauncherCollisionObjectPosition);
        
    for (%i = 0; %i < %sceneObjectGroupCount; %i++)
    {
        %sceneObject = LauncherSceneGroup.getObject(%i);
        
        if (%sceneObject == %object)
            continue;
            
        %sceneObject.setPosition(Vector2Add(%sceneObject.getPosition(), %positionDelta));
    }
    
    $LevelBuilderTool::LastLauncherCollisionObjectPosition = %object.getPosition();
}

/// <summary>
/// Restricts positioning of an Object to within the Bound of the Level
/// </summary>
/// <param="object">Object being positioned</param>
function LevelBuilderSceneEditor::clampObjectWithinLevel(%this, %object)
{
    %halfObjectSizeX = %object.getSizeX() / 2;
    %halfObjectSizeY = %object.getSizeY() / 2;
    %halfLevelSizeX = LevelBuilderToolPresenter.Scene.levelSize.x / 2;
    %halfLevelSizeY = LevelBuilderToolPresenter.Scene.levelSize.y / 2;
    
    if ((%object.getPositionX() - %halfObjectSizeX) < -%halfLevelSizeX)
        %object.setPositionX(-%halfLevelSizeX + %halfObjectSizeX);
    else if ((%object.getPositionX() + %halfObjectSizeX) > %halfLevelSizeX)
        %object.setPositionX(%halfLevelSizeX - %halfObjectSizeX);
        
    if ((%object.getPositionY() - %halfObjectSizeY) < -%halfLevelSizeY)
        %object.setPositionY(-%halfLevelSizeY + %halfObjectSizeY);
    else if ((%object.getPositionY() + %halfObjectSizeY) > %halfLevelSizeY)
        %object.setPositionY(%halfLevelSizeY - %halfObjectSizeY);
}

/// <summary>
/// Handles Object Relinquishment in the Scene View
/// </summary>
/// <param="object">Object which has been Relinquished</param>
function LevelBuilderSceneEditor::onRelinquishObject(%this, %object)
{
    LevelBuilderToolPresenter.RightView.setSelectedObjectEditActive(false);    
    LevelBuilderToolPresenter.RightView.setSendToBackActive(false);
    LevelBuilderToolPresenter.RightView.setBringToFrontActive(false);
        
    LevelBuilderToolPresenter.RightView.setSelectedObjectNameDisplay("");
    
    if (%object.getSceneLayer() == $PhysicsLauncherTools::LauncherCollisionObjectSceneLayer)
    {        
        %builderObject = SlingshotLauncherBuilder::getBuilderObject(LauncherSceneGroup);
        
        %builderObject.setDebugOff($CollisionShapesDebugId);
        %builderObject.setVisible(false);
        
        LevelBuilderToolPresenter.RightView.setSelectedObjectStatsVisible(true);
        LevelBuilderToolPresenter.RightView.setSelectedObjectEditText("Edit Object");
        
        LevelBuilderToolPresenter.RightView.clearLauncherBackgroundPreview();
    }
    else
    {
        LevelBuilderToolPresenter.RightView.setDeleteActive(false);
        LevelBuilderToolPresenter.RightView.setDuplicateActive(false);
        LevelBuilderToolPresenter.RightView.setRotateActive(false);
        
        LevelBuilderToolPresenter.RightView.setSelectedObjectMass("");
        LevelBuilderToolPresenter.RightView.setSelectedObjectFriction("");
        LevelBuilderToolPresenter.RightView.setSelectedObjectBounce("");
        LevelBuilderToolPresenter.RightView.setSelectedObjectHitPoints("");
    }
    
    LevelBuilderToolPresenter.RightView.clearSelectedObjectPreview();
}