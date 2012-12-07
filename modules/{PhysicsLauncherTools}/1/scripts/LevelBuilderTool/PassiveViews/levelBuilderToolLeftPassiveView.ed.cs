//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------------------------------------
// Level Builder Tool Left Passive View
//--------------------------------------------------------------------------------------------------------------------------------

/// <summary>
/// Sets Up the Level Builder Tool Left View On Wake
/// </summary>
function LevelBuilderToolLeftPassiveView::onWake(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::Wake");
}

function LevelBuilderToolLeftPassiveView::setSceneViewLayerMask(%this, %layerMask)
{
    Lbtlpv_Sc_Ss_SceneView.setLayerMask(%layerMask);
}

function LevelBuilderToolLeftPassiveView::setSceneViewSceneEdit(%this, %sceneEdit)
{
    Lbtlpv_Sc_Ss_SceneView.setSceneEdit(%sceneEdit);
}

function LevelBuilderToolLeftPassiveView::setSceneViewScene(%this, %scene)
{
    Lbtlpv_Sc_Ss_SceneView.setScene(%scene);
}

function LevelBuilderToolLeftPassiveView::setSceneViewSize(%this, %width, %height)
{
    Lbtlpv_Sc_Ss_SceneView.setExtent(%width, %height);
}

function LevelBuilderToolLeftPassiveView::setSceneViewCameraPositionAndSize(%this, %x, %y, %width, %height)
{
    Lbtlpv_Sc_Ss_SceneView.setCurrentCameraPosition(%x, %y, %width, %height);
}

function LevelBuilderToolLeftPassiveView::addZoomOption(%this, %zoomOption)
{
    Lbtlpv_ZoomDropdown.add(%zoomOption, Lbtlpv_ZoomDropdown.size());
}

function LevelBuilderToolLeftPassiveView::selectZoomOption(%this, %zoomOption)
{
    Lbtlpv_ZoomDropdown.setSelected(Lbtlpv_ZoomDropdown.findText(%zoomOption));
}

function LevelBuilderToolLeftPassiveView::getSelectedZoomOption(%this)
{
    return Lbtlpv_ZoomDropdown.getText();
}

function LevelBuilderToolLeftPassiveView::setWorldListActive(%this, %active)
{
    Lbtlpv_WorldDropdown.setActive(%active);
}

function LevelBuilderToolLeftPassiveView::clearWorldList(%this)
{
    Lbtlpv_WorldDropdown.clear();
}

function LevelBuilderToolLeftPassiveView::addToWorldList(%this, %world)
{
    Lbtlpv_WorldDropdown.add(%world, Lbtlpv_WorldDropdown.size());
}

function LevelBuilderToolLeftPassiveView::selectWorld(%this, %world, %callback)
{
    Lbtlpv_WorldDropdown.setSelected(Lbtlpv_WorldDropdown.findText(%world), %callback);
} 

function LevelBuilderToolLeftPassiveView::setAcceptDuplicationStamp(%this, %accept)
{
    Lbtlpv_Sc_Ss_Sv_MouseEventCtrl.setProfile(%accept ? "GuiDefaultProfile" : "GuiModelessDialogProfile");
}

function LevelBuilderToolLeftPassiveView::getSceneViewWorldPoint(%this, %position)
{
    return Lbtlpv_Sc_Ss_SceneView.getWorldPoint(Vector2Sub(%position, Lbtlpv_Sc_Ss_SceneView.getGlobalPosition()));
}

function LevelBuilderToolLeftPassiveView::getSceneViewWindowPoint(%this, %position)
{
    return Lbtlpv_Sc_Ss_SceneView.getWindowPoint(%position);
}

function LevelBuilderToolLeftPassiveView::selectLevel(%this, %level)
{
    Lbtlpv_LevelDropdown.setSelected(Lbtlpv_LevelDropdown.findText(%level));
}

function LevelBuilderToolLeftPassiveView::getSelectedWorld(%this)
{
    return Lbtlpv_WorldDropdown.getText();
}

function LevelBuilderToolLeftPassiveView::setLevelListActive(%this, %active)
{
    Lbtlpv_LevelDropdown.setActive(%active);
}

function LevelBuilderToolLeftPassiveView::clearLevelList(%this)
{
    Lbtlpv_LevelDropdown.clear();
}

function LevelBuilderToolLeftPassiveView::addToLevelList(%this, %level)
{
    Lbtlpv_LevelDropdown.add(%level, Lbtlpv_LevelDropdown.size());
}

function LevelBuilderToolLeftPassiveView::selectLevel(%this, %level, %callback)
{
    Lbtlpv_LevelDropdown.setSelected(Lbtlpv_LevelDropdown.findText(%level), %callback);
}

function LevelBuilderToolLeftPassiveView::getCollisionOn(%this)
{
    return Lbtlpv_CollisionCheckBox.getStateOn();
}

function LevelBuilderToolLeftPassiveView::getSelectedLevel(%this)
{
    return Lbtlpv_LevelDropdown.getText();
}

function LevelBuilderToolLeftPassiveView::refreshSceneScroll(%this)
{
    Lbtlpv_Sc_SceneScroll.computeSizes();
}

function LevelBuilderToolLeftPassiveView::addToSceneView(%this, %object)
{
    Lbtlpv_Sc_Ss_SceneView.add(%object);
}

function LevelBuilderToolLeftPassiveView::addDuplicationStamp(%this, %stamp)
{
    Lbtlpv_Sc_Ss_Sv_MouseEventCtrl.add(%stamp);
}

function LevelBuilderToolLeftPassiveView::scrollSceneScrollToBottom(%this)
{
    Lbtlpv_Sc_SceneScroll.scrollToBottom();
}

function LevelBuilderToolLeftPassiveView::setSceneScrollScrollPosition(%this, %x, %y)
{
    Lbtlpv_Sc_SceneScroll.setScrollPosition(%x, %y);
}

function LevelBuilderToolLeftPassiveView::getSceneScrollSize(%this)
{
    return Lbtlpv_Sc_SceneScroll.getExtent();
}

function LevelBuilderToolLeftPassiveView::getSceneScrollScrollPositionY(%this)
{
    return Lbtlpv_Sc_SceneScroll.getScrollPositionY();
}

/// <summary>
/// Saves the Current Level On Sleep
/// </summary>
function LevelBuilderToolLeftPassiveView::onSleep(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::Sleep");
}

/// <summary>
/// Handles a Control Dropped Event
/// </summary>
/// <param="control">Control being Dropped</param>
/// <param="position">Position at which the Control is being Dropped</param>
function LevelBuilderToolLeftPassiveView::onControlDropped(%this, %control, %position)
{
    %data = new ScriptObject()
    {
        control = %control;
        position = %position;
    };
    
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::ControlDropped", %data);
}





//--------------------------------
// Scene View Mouse Event Control
//--------------------------------

/// <summary>
/// Handles a Mouse Move Event in the Scene View
/// </summary>
/// <param="modifier">Modifier present, if any, during Move</param>
/// <param="mousePoint">The Point at which the Move occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function Lbtlpv_Sc_Ss_Sv_MouseEventCtrl::onMouseMove(%this, %modifier, %mousePoint, %mouseClickCount)
{
    %data = new ScriptObject()
    {
        mousePoint = %mousePoint;
        globalPosition = %this.getGlobalPosition();
        object = %this.getObject(0);
    };
    
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::DuplicationStampMove", %data);
}

/// <summary>
/// Handles a Mouse Down Event in the Scene View
/// </summary>
/// <param="modifier">Modifier present, if any, during Down event</param>
/// <param="mousePoint">The Point at which the Down event occurred</param>
/// <param="mouseClickCount">Number of times the Mouse has been Clicked</param>
function Lbtlpv_Sc_Ss_Sv_MouseEventCtrl::onMouseDown(%this, %modifier, %mousePoint, %mouseClickCount)
{
    %data = new ScriptObject()
    {
        mousePoint = %mousePoint;
        globalPosition = %this.getGlobalPosition();
        object = %this.getObject(0);
    };
    
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::DuplicationStampDown", %data);
}


//--------------------------------
// World Dropdown
//--------------------------------

/// <summary>
/// Handles to a World Selection event
/// </summary>
function Lbtlpv_WorldDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::WorldSelect");
}


//--------------------------------
// Level Dropdown
//--------------------------------

/// <summary>
/// Handles a Selection in the Level Dropdown
/// </summary>
function Lbtlpv_LevelDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::LevelSelect");
}


//--------------------------------
// Collision CheckBox
//--------------------------------

/// <summary>
/// Handles the Collision CheckBox Click Event
/// </summary>
function Lbtlpv_CollisionCheckBox::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::CollisionToggle");
}


//--------------------------------
// Zoom Dropdown
//--------------------------------

/// <summary>
/// Handles a Selection in the Zoom Dropdown
/// </summary>
function Lbtlpv_ZoomDropdown::onSelect(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::ZoomSelect");
}


//--------------------------------
// Preview Physics Button
//--------------------------------

/// <summary>
/// Toggles Physics Preview
/// </summary>
function Lbtlpv_PreviewPhysicsButton::onClick(%this)
{
    PhysicsLauncherToolsEventManager.postEvent("LevelBuilderToolLeftPassiveView::PhysicsPreviewToggle");
}