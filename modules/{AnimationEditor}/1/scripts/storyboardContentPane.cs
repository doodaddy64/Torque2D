//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function StoryboardContentPane::createFrameControl(%this, %frame, %index)
{
    %preview = new GuiSpriteCtrl()
    {
        class = "ABStoryboardPreviewSprite";
        Profile = "GuiTransparentProfile";
        position = "0 0";
        extent = "64 64";
        MinExtent = "2 2";
        HorizSizing = "relative";
        VertSizing = "relative";
        Image = %this.image;
        Frame = %frame;
        frameNumber = %index;
    };

    %preview.sceneObject = %preview;
    %mouseEvent = new GuiMouseEventCtrl() {
        canSaveDynamicFields = "0";
        Profile = "GuiTransparentProfile";
        class = "Ab_FrameControl";
        HorizSizing = "relative";
        VertSizing = "relative";
        Position = "0 0";
        Extent = "64 64";
        MinExtent = "2 2";
        canSave = "1";
        Visible = "1";
        hovertime = "1000";
        tooltipprofile="GuiToolTipProfile";
        ToolTip="Drag and drop frames from the display above.  Double click on a frame in the Timeline to remove it from the Animation Timeline.";
    };
    %preview.add(%mouseEvent);

    return %preview;
}

function StoryboardContentPane::update(%this, %imageMapAssetId, %frames)
{
    %this.image = %imageMapAssetId;
    %imageMap = AssetDatabase.acquireAsset(%imageMapAssetId);

    %this.deleteContents();

    %count = getWordCount(%frames);
    %this.Extent = ((%count + 1) * (64 + %this.colSpacing)) SPC "64";
    %this.getParent().computeSizes();

    for (%i = 0; %i < %count; %i++)
    {
        %frame = getWord(%frames, %i);
        //echo(" @@@ adding frame " @ %frame @ " to storyboard pane.");
        %this.add(%this.createFrameControl(%frame, %i));
    }

    %dropSpot = new SceneObject()
    {
        size = %imageMap.getFrameSize(0);
    };

    %this.add(%dropSpot);
    %dropSpot.position = %width + (getWord(%dropSpot.size, 0) / 2) SPC "0";

    %width += %imageMap.getFrameSize(0);

    AssetDatabase.releaseAsset(%imageMapAssetId);
    %this.refresh();
}