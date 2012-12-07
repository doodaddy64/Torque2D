//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------------------
// Level Builder Tool Right Passive View
//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// Sets Up the Level Builder Tool Right View On Wake
/// </summary>
function LevelBuilderToolRightPassiveView::onWake(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::Wake");
}

function LevelBuilderToolRightPassiveView::selectSceneTab(%this, %tab)
{
    switch$ (%tab)
    {
        case "Objects":
            Lbtrpv_Vb_SceneTabBook.selectPage(0);
        
        case "Gravity":
            Lbtrpv_Vb_SceneTabBook.selectPage(1);
        
        case "Backgrounds":
            Lbtrpv_Vb_SceneTabBook.selectPage(2);
        
        default:
    }
}

function LevelBuilderToolRightPassiveView::setSelectionToolActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_SelectionToolButton.setActive(%active); 
}

function LevelBuilderToolRightPassiveView::setRotateActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_RotateButton.setActive(%active); 
}

function LevelBuilderToolRightPassiveView::setDuplicateActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_DuplicateButton.setActive(%active);
}

function LevelBuilderToolRightPassiveView::setBringToFrontActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_BringToFrontButton.setActive(%active);
}

function LevelBuilderToolRightPassiveView::setSendToBackActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_SendToBackButton.setActive(%active);
}

function LevelBuilderToolRightPassiveView::setDeleteActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_DeleteButton.setActive(%active); 
}

function LevelBuilderToolRightPassiveView::setSelectedObjectNameDisplay(%this, %name)
{
    Lbtrpv_Vb_Stb_Otp_Odc_NameDisplay.setText(%name);   
}

function LevelBuilderToolRightPassiveView::setSelectedObjectStatsVisible(%this, %visible)
{
    Lbtrpv_Vb_Stb_Otp_Odc_ObjectStatsContainer.setVisible(%visible);
}

function LevelBuilderToolRightPassiveView::setSelectedObjectEditText(%this, %text)
{
    Lbtrpv_Vb_Stb_Otp_EditObjectButton.setText(%text);
}

function LevelBuilderToolRightPassiveView::setupLauncherBackgroundPreview(%this, %object)
{
    Lbtrpv_Vb_Stb_Otp_Odc_LauncherBackgroundPreview.setup(%object);
}

function LevelBuilderToolRightPassiveView::setupSelectedObjectPreview(%this, %object)
{
    Lbtrpv_Vb_Stb_Otp_Odc_ObjectPreview.setup(%object);
}

function LevelBuilderToolRightPassiveView::setSelectedObjectMass(%this, %mass)
{
    Lbtrpv_Vb_Stb_Otp_Odc_Osc_MassDisplay.setText(%mass);
}

function LevelBuilderToolRightPassiveView::setSelectedObjectFriction(%this, %friction)
{
    Lbtrpv_Vb_Stb_Otp_Odc_Osc_FrictionDisplay.setText(%friction);
}

function LevelBuilderToolRightPassiveView::setSelectedObjectBounce(%this, %bounce)
{
    Lbtrpv_Vb_Stb_Otp_Odc_Osc_BounceDisplay.setText(%bounce);
}

function LevelBuilderToolRightPassiveView::setSelectedObjectHitPoints(%this, %hitPoints)
{
    Lbtrpv_Vb_Stb_Otp_Odc_Osc_HitPointsDisplay.setText(%hitPoints);
}

function LevelBuilderToolRightPassiveView::setSelectedObjectEditActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Otp_EditObjectButton.setActive(%active);
}

function LevelBuilderToolRightPassiveView::clearLauncherBackgroundPreview(%this)
{
    Lbtrpv_Vb_Stb_Otp_Odc_LauncherBackgroundPreview.setSceneObject("");
}

function LevelBuilderToolRightPassiveView::clearSelectedObjectPreview(%this)
{
    Lbtrpv_Vb_Stb_Otp_Odc_ObjectPreview.setSceneObject("");
}

function LevelBuilderToolRightPassiveView::selectSelectionTool(%this)
{
    Lbtrpv_Vb_Stb_Otp_SelectionToolButton.setStateOn(true);
}

function LevelBuilderToolRightPassiveView::enableSceneTabBook(%this, %enable)
{
    Lbtrpv_Vb_SceneTabBook.callOnChildren(setActive, %enable);
}

function LevelBuilderToolRightPassiveView::clearObjectList(%this)
{
    Lbtrpv_Vb_Stb_Otp_Os_ObjectsArray.deleteContents();
    Lbtrpv_Vb_Stb_Otp_Os_ObjectsArray.clear();
}

function LevelBuilderToolRightPassiveView::addToObjectList(%this, %object)
{
    Lbtrpv_Vb_Stb_Otp_Os_ObjectsArray.addGuiControl(%object);
}

function LevelBuilderToolRightPassiveView::refreshObjectScroll(%this)
{
    %objectsScrollerExtent = Lbtrpv_Vb_Stb_Otp_ObjectsScroller.getExtent();
    Lbtrpv_Vb_Stb_Otp_ObjectsScroller.setExtent(getWord(%objectsScrollerExtent, 0), getWord(%objectsScrollerExtent, 1));
}

function LevelBuilderToolRightPassiveView::selectGravityUp(%this)
{
    Lbtrpv_Vb_Stb_Gtp_UpButton.setStateOn(true);
}

function LevelBuilderToolRightPassiveView::selectGravityNone(%this)
{
    Lbtrpv_Vb_Stb_Gtp_NoneButton.setStateOn(true);
}

function LevelBuilderToolRightPassiveView::selectGravityDown(%this)
{
    Lbtrpv_Vb_Stb_Gtp_DownButton.setStateOn(true);
}

function LevelBuilderToolRightPassiveView::selectGravityStrength(%this, %strength)
{
    Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.setSelected(Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.findText(%strength));
}

function LevelBuilderToolRightPassiveView::selectLevelSize(%this, %levelSize)
{
    Lbtrpv_Vb_Stb_Btp_Fc_LevelSizeDropdown.setSelected(Lbtrpv_Vb_Stb_Btp_Fc_LevelSizeDropdown.findText(%levelSize));
}

function LevelBuilderToolRightPassiveView::selectBackgroundFormatTile(%this)
{
    Lbtrpv_Vb_Stb_Btp_Fc_TileRadio.setStateOn(true);
}

function LevelBuilderToolRightPassiveView::selectBackgroundFormatStretch(%this)
{
    Lbtrpv_Vb_Stb_Btp_Fc_StretchRadio.setStateOn(true);
}

function LevelBuilderToolRightPassiveView::getBackgroundFormatIsTile(%this)
{
    return Lbtrpv_Vb_Stb_Btp_Fc_TileRadio.getStateOn();
}

function LevelBuilderToolRightPassiveView::setForegroundAssetDisplay(%this, %asset)
{
    Lbtrpv_Vb_Stb_Btp_Fc_ForegroundAssetDisplay.setText(%asset);
}

function LevelBuilderToolRightPassiveView::setBackground1AssetDisplay(%this, %asset)
{
    Lbtrpv_Vb_Stb_Btp_B1c_Background1AssetDisplay.setText(%asset);
}

function LevelBuilderToolRightPassiveView::setBackground2AssetDisplay(%this, %asset)
{
    Lbtrpv_Vb_Stb_Btp_B2c_Background2AssetDisplay.setText(%asset);
}

function LevelBuilderToolRightPassiveView::setSkyAssetDisplay(%this, %asset)
{
    Lbtrpv_Vb_Stb_Btp_Sc_SkyAssetDisplay.setText(%asset);
}

function LevelBuilderToolRightPassiveView::selectForegroundParallaxSpeed(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_Fc_ParallaxSpeedDropdown.setSelected(Lbtrpv_Vb_Stb_Btp_Fc_ParallaxSpeedDropdown.findText(%speed));
}

function LevelBuilderToolRightPassiveView::selectBackground1ParallaxSpeed(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_B1c_ParallaxSpeedDropdown.setSelected(Lbtrpv_Vb_Stb_Btp_B1c_ParallaxSpeedDropdown.findText(%speed));
}

function LevelBuilderToolRightPassiveView::selectBackground2ParallaxSpeed(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_B2c_ParallaxSpeedDropdown.setSelected(Lbtrpv_Vb_Stb_Btp_B2c_ParallaxSpeedDropdown.findText(%speed));
}

function LevelBuilderToolRightPassiveView::selectSkyParallaxSpeed(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_Sc_ParallaxSpeedDropdown.setSelected(Lbtrpv_Vb_Stb_Btp_Sc_ParallaxSpeedDropdown.findText(%speed));
}

function LevelBuilderToolRightPassiveView::addToGravityStrengthList(%this, %strength)
{
    Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.add(%strength, Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.size());
}

function LevelBuilderToolRightPassiveView::addToForegroundParallaxSpeedList(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_Fc_ParallaxSpeedDropdown.add(%speed, Lbtrpv_Vb_Stb_Btp_Fc_ParallaxSpeedDropdown.size());
}

function LevelBuilderToolRightPassiveView::addToBackground1ParallaxSpeedList(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_B1c_ParallaxSpeedDropdown.add(%speed, Lbtrpv_Vb_Stb_Btp_B1c_ParallaxSpeedDropdown.size());
}

function LevelBuilderToolRightPassiveView::addToBackground2ParallaxSpeedList(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_B2c_ParallaxSpeedDropdown.add(%speed, Lbtrpv_Vb_Stb_Btp_B2c_ParallaxSpeedDropdown.size());
}

function LevelBuilderToolRightPassiveView::addToSkyParallaxSpeedList(%this, %speed)
{
    Lbtrpv_Vb_Stb_Btp_Sc_ParallaxSpeedDropdown.add(%speed, Lbtrpv_Vb_Stb_Btp_Sc_ParallaxSpeedDropdown.size());
}

function LevelBuilderToolRightPassiveView::addToLevelSizeList(%this, %size)
{
    Lbtrpv_Vb_Stb_Btp_Fc_LevelSizeDropdown.add(%size, Lbtrpv_Vb_Stb_Btp_Fc_LevelSizeDropdown.size());
}

function LevelBuilderToolRightPassiveView::setGravityStrengthListActive(%this, %active)
{
    Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.setActive(%active);
}

function LevelBuilderToolRightPassiveView::getGravityStrengthListActive(%this)
{
    return Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.isActive();
}

function LevelBuilderToolRightPassiveView::getSelectedGravityStrength(%this)
{
    return Lbtrpv_Vb_Stb_Gtp_StrengthDropdown.getText();
}

function LevelBuilderToolRightPassiveView::getSelectedLevelSize(%this)
{
    return Lbtrpv_Vb_Stb_Btp_Fc_LevelSizeDropdown.getText();
}

function LevelBuilderToolRightPassiveView::getSelectedForegroundParallaxSpeed(%this)
{
    return Lbtrpv_Vb_Stb_Btp_Fc_ParallaxSpeedDropdown.getText();
}

function LevelBuilderToolRightPassiveView::getSelectedBackground1ParallaxSpeed(%this)
{
    return Lbtrpv_Vb_Stb_Btp_B1c_ParallaxSpeedDropdown.getText();
}

function LevelBuilderToolRightPassiveView::getSelectedBackground2ParallaxSpeed(%this)
{
    return Lbtrpv_Vb_Stb_Btp_B2c_ParallaxSpeedDropdown.getText();
}

function LevelBuilderToolRightPassiveView::getSelectedSkyParallaxSpeed(%this)
{
    return Lbtrpv_Vb_Stb_Btp_Sc_ParallaxSpeedDropdown.getText();
}

/// <summary>
/// Cleans up the Tick Image on Sleep
/// </summary>
function LevelBuilderToolRightPassiveView::onSleep(%this)
{
    %this.tick.delete();
}

/// <summary>
/// Initializes the Right View of the Level Builder Tool
/// </summary>


//----------------------------------------------------------------
// Edit Buttons
//----------------------------------------------------------------



//--------------------------------
// Selection Tool Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Selection Tool button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_SelectionToolButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::SelectionToolSelected");
}

//--------------------------------
// Rotate Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Rotate button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_RotateButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::RotateSelected");
}

//--------------------------------
// Duplicate Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Duplicate button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_DuplicateButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::DuplicateSelected");
}

//--------------------------------
// Bring to Front Tool Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Bring to Front button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_BringToFrontButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::BringToFrontSelected");
}

//--------------------------------
// Send to Back Tool Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Send to Back button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_SendToBackButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::SendToBackSelected");
}

//--------------------------------
// Delete Tool Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Delete button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_DeleteButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::DeleteSelected");
}


//----------------------------------------------------------------
// Selected Object Details
//----------------------------------------------------------------



//--------------------------------
// Edit Object Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Edit Object button
/// </summary>
function Lbtrpv_Vb_Stb_Otp_EditObjectButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::SelectedObjectEditSelected");
}


//----------------------------------------------------------------
// Objects Tab Page
//----------------------------------------------------------------



//----------------------------------------------------------------
// Gravity Tab Page
//----------------------------------------------------------------

/// <summary>
/// Refreshes the Gravity Tab Page
/// </summary>

//--------------------------------
// Gravity North Button
//--------------------------------

/// <summary>
/// Refreshes the Gravity Tab Page
/// </summary>
function Lbtrpv_Vb_Stb_Gtp_UpButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::GravityUpSelected");
}

//--------------------------------
// Gravity None Button
//--------------------------------

/// <summary>
/// Refreshes the Gravity Tab Page
/// </summary>
function Lbtrpv_Vb_Stb_Gtp_NoneButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::GravityNoneSelected");
}

//--------------------------------
// Gravity South Button
//--------------------------------

/// <summary>
/// Refreshes the Gravity Tab Page
/// </summary>
function Lbtrpv_Vb_Stb_Gtp_DownButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::GravityDownSelected");
}

//--------------------------------
// Gravity Strength Dropdown
//--------------------------------

/// <summary>
/// Refreshes the Gravity Strength Dropdown
/// </summary>


/// <summary>
/// Handles a Selection in the Gravity Strength Dropdown
/// </summary>
function Lbtrpv_Vb_Stb_Gtp_StrengthDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::GravityStrengthSelect");  
}


//----------------------------------------------------------------
// Backgrounds Tab Page
//----------------------------------------------------------------

/// <summary>
/// Refreshes the Backgrounds Tab Page
/// </summary>

//--------------------------------
// Level Size Dropdown
//--------------------------------

/// <summary>
/// Refreshes the Level Size Dropdown
/// </summary>

/// <summary>
/// Handles a Selection in the Level Size Dropdown
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Fc_LevelSizeDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::LevelSizeSelect");
}

//--------------------------------
// Format
//--------------------------------


//--------------------------------
// Format Tile Radio
//--------------------------------

/// <summary>
/// Handles the Tile Background Format Selection
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Fc_TileRadio::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::BackgroundFormatTileSelect");
}

//--------------------------------
// Format Stretch Radio
//--------------------------------

/// <summary>
/// Handles the Stretch Background Format Selection
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Fc_StretchRadio::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::BackgroundFormatStretchSelect");
}

//--------------------------------
// Parallax Speed Dropdown Generics
//--------------------------------

//--------------------------------
// Foreground Parallax Speed Dropdown
//--------------------------------

/// <summary>
/// Handles a Selection in the Foreground Parallax Speed Dropdown
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Fc_ParallaxSpeedDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::ForegroundParallaxSpeedSelect");
}

//--------------------------------
// Background 1 Parallax Speed Dropdown
//--------------------------------

/// <summary>
/// Handles a Selection in the Background 1 Parallax Speed Dropdown
/// </summary>
function Lbtrpv_Vb_Stb_Btp_B1c_ParallaxSpeedDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::Background1ParallaxSpeedSelect");
}

//--------------------------------
// Background 2 Parallax Speed Dropdown
//--------------------------------

/// <summary>
/// Handles a Selection in the Background 2 Parallax Speed Dropdown
/// </summary>
function Lbtrpv_Vb_Stb_Btp_B2c_ParallaxSpeedDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::Background2ParallaxSpeedSelect");
}

//--------------------------------
// Sky Parallax Speed Dropdown
//--------------------------------

/// <summary>
/// Handles a Selection in the Sky Parallax Speed Dropdown
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Sc_ParallaxSpeedDropdown::onSelect(%this)
{    
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::SkyParallaxSpeedSelect");
}

//--------------------------------
// Foreground Asset Library Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Foreground Asset Library Button
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Fc_AssetLibraryButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::ForegroundAssetSelect");
}

//--------------------------------
// Background 1 Asset Library Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Background 1 Asset Library Button
/// </summary>
function Lbtrpv_Vb_Stb_Btp_B1c_AssetLibraryButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::Background1AssetSelect");
}

//--------------------------------
// Background 2 Asset Library Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Background 2 Asset Library Button
/// </summary>
function Lbtrpv_Vb_Stb_Btp_B2c_AssetLibraryButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::Background2AssetSelect");
}

//--------------------------------
// Sky Asset Library Button
//--------------------------------

/// <summary>
/// Handles a Click event on the Sky Asset Library Button
/// </summary>
function Lbtrpv_Vb_Stb_Btp_Sc_AssetLibraryButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolRightPassiveView::SkyAssetSelect");
}