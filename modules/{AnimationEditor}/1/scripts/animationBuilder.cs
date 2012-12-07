//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function AnimationBuilder::onAdd(%this)
{
    %this.tempAnimation = "";
    %this.newAnimation = false;
    %this.animationIsDirty = false;
    %this.sourceImageMap = "";
    %this.animationName = "";
    
    // Default animation values.
    %this.defaultFPS = 30;
    %this.defaultAnimationCycle = true;
    %this.defaultRandomStart = false;
    %this.defaultName = "Animation";
    %this.defaultImageMapName = "Sprite";

    // Bounds
    %this.maxFPS = $MaxFPS;
    %this.minFPS = $MinFPS;

    // The scene to use for the drag and drop controls.
    %this.draggingScene = new Scene();
}

function AnimationBuilder::updateGui(%this)
{
    ABTagList.refresh(0);
    ABTagContainer.populateTagList(%this.animationId);

    if (isObject(%this.tempAnimation))
    {
        ABStoryboardWindow.update(%this.sourceImageMap, %this.tempAnimation.animationFrames);
        ABCycleAnimationCheck.setValue(%this.tempAnimation.animationCycle);
        if (%this.animationName !$= %this.tempAnimation.AssetName)
        {
            %this.animationName = %this.tempAnimation.AssetName;
        }
        ABNameField.setText(%this.animationName);
        ABImageMapField.setText(AssetDatabase.getAssetName(%this.sourceImageMap));

        %frameCount = getWordCount(%this.tempAnimation.animationFrames);

        if (%frameCount > 0)
        {
            ABFPSField.setText(mCeil(%frameCount / %this.tempAnimation.animationTime));
            ABAnimationPreviewWindow.display(%this.tempAnimation);
        }
        else
        {
            ABFPSField.setText(mCeil(%this.defaultFPS));
            ABAnimationPreviewWindow.clear();
        }
    }
    
    ABSaveButton.update();
}

function AnimationBuilder::onRemove(%this)
{
    if (isObject(%this.draggingScene))
        %this.draggingScene.delete();
}

function AnimationBuilder::open(%this)
{
    activatePackage(AnimationBuilderPackage);

    %this.animationIsDirty = false;

    if (isObject(%this.tempAnimation))
    {
        Ab_ImageMapPreviewWindow.display(%this.sourceImageMap);
        ABTagContainer.populateTagList(%this.animationId);
    }
    else if (isObject(%this.sourceImageMap))
        %this.tempAnimation = generateTemporaryAnimation(%this.sourceImageMap);
    else
        warn("AnimationBuilder::open - no animation to edit.");

    Canvas.pushDialog(AnimationBuilderGui);

    %this.updateGui();
}

function AnimationBuilder::close(%this)
{
    Canvas.popDialog(AnimationBuilderGui);
    Ab_ImageMapPreviewWindow.clear();
    ABAnimationPreviewWindow.clear();

    %this.newAnimation = false;

    if (isObject(%this.tempAnimation))
        %this.tempAnimation.delete();

    %this.animationName = "";
    %this.tempAnimation = "";
    %this.sourceImageMap = "";
    %this.animationIsDirty = false;

    cleanTemporaryAssets();

    deactivatePackage(AnimationBuilderPackage);

    $AssetAutoTag = "";
}

function AnimationBuilder::createAnimation(%this)
{
    // We want celled and linked image maps
    %this.newAnimation = true;

    ABNameField.setText = "";
    ABImageMapName.text = "";

    if (isObject(ABStoryBoardWindow.staticSpriteGroup))
        ABStoryBoardWindow.staticSpriteGroup.deleteContents();

    Ab_ImageMapPreviewWindow.clear();
    ABStoryboardWindow.clear();

    %this.open();
}

function AnimationBuilder::newAnimation(%this, %imageMapAssetId, %newAnim)
{
    if (%this.tempAnimation !$= "" && %newAnim)
    {
        AssetDatabase.releaseAsset(%this.tempAnimation);
        %this.tempAnimation = "";
    }
    Ab_ImageMapPreviewWindow.clear();

    cleanTemporaryAssets();

    // Check to see if the selected image map is a declared asset
    // If not, back out
    if (!AssetDatabase.isDeclaredAsset(%imageMapAssetId))
        return;

    %this.newAnimation = true;
    %this.sourceImageMap = %imageMapAssetId;
    %this.animationId = "";
    %this.animationName = "";
    %this.tempAnimation = generateTemporaryAnimation(%this.sourceImageMap, %this.newAnimation);
    %this.open();
}

function AnimationBuilder::editAnimation(%this, %animationAssetId)
{
    if (%this.tempAnimation !$= "")
    {
        AssetDatabase.releaseAsset(%this.tempAnimation);
        %this.tempAnimation = "";
    }
    Ab_ImageMapPreviewWindow.clear();

    cleanTemporaryAssets();

    %this.newAnimation = false;

    %assetDatablock = AssetDatabase.acquireAsset(%animationAssetId);

    %this.animationId = %animationAssetId;
    %this.animationName = %assetDatablock.AssetName;
    %this.sourceImageMap = %assetDatablock.imageMap;

    %this.tempAnimation = generateTemporaryAnimation(%this.sourceImageMap, %this.newAnimation);
    %this.tempAnimation.copy(%assetDatablock);

    AssetDatabase.releaseAsset(%animationAssetId);
    
    %this.open();
}

function AnimationBuilder::save(%this)
{
    %newAssetName = ABNameField.getText();
    %oldAssetName = %this.animationName;
    %assetDatablock = %this.tempAnimation;
    %assetId = %assetDatablock.getAssetId();
    %asset = AssetDatabase.acquireAsset(%assetId);
    %path = expandPath("^{UserAssets}/animations/");
    
    if (%this.newAnimation)
    {
        %finalAsset = new AnimationAsset();
        %finalAsset.copy(%asset);
        %finalAsset.AssetName = %newAssetName;
        %newDefinitionPath = %path @ %newAssetName @ ".asset.taml";
        %moduleDefinition = ModuleDatabase.getDefinitionFromId("{UserAssets}");
        
        TamlWrite(%finalAsset, %newDefinitionPath);
        
        AssetDatabase.addSingleDeclaredAsset(%moduleDefinition, %newDefinitionPath);
    }
    else
    {
        %originalDatablock = AssetDatabase.acquireAsset(%this.animationId);
        
        %originalDatablock.copy(%assetDatablock);

        // Editing existing asset, but user changed the name
        // We need to update its references
        // We have changed the name of the asset we are editing. Need to update
        if (%newAssetName !$= %oldAssetName)
        {
            %assetModule = "{UserAssets}:";
            %newID = %assetModule @ %newAssetName;
            
            AssetDatabase.renameDeclaredAsset(%this.animationId, %newID);
            AssetDatabase.renameReferencedAsset(%this.animationId, %newID);
        }
        AssetDatabase.releaseAsset(%originalDatablock);
        AssetDatabase.releaseAsset(%this.tempAnimation);
    }
    AssetDatabase.releaseAsset(%this.tempAnimation);
    AssetDatabase.removeSingleDeclaredAsset(%this.tempAnimation);
    AssetDatabase.releaseAsset(%this.animationId);

    AssetDatabase.saveAssetTags();

    cleanTemporaryAssets();

    AssetLibrary.schedule(100, "updateGui");

    %this.close();
}

function AnimationBuilder::cancel(%this)
{
    AssetDatabase.restoreAssetTags();
    if (isObject(%this.tempAnimation))
        %this.tempAnimation.delete();
    %this.close();
}

function AnimationBuilder::validateCycleAnimation(%this)
{
    if (!isObject(%this.tempAnimation))
        return;

    %this.animationIsDirty = true;
    %this.tempAnimation.animationCycle = ABCycleAnimationCheck.getValue();
}

function AnimationBuilder::validateFPS(%this)
{
    if (!isObject(%this.tempAnimation))
        return;

    %fps = ABFPSField.getText();
    %fps = clamp(%fps, %this.minFPS, %this.maxFPS);
    ABFPSField.setText(%fps);

    if (%this.tempAnimation.getFrameCount() < 1)
        return;

    %this.animationIsDirty = true;

    %this.tempAnimation.animationTime = %this.tempAnimation.getFrameCount() / %fps;
    //%this.tempAnimation.calculateAnimation();
    %this.updateGui();
}

function AnimationBuilder::validateName(%this)
{
    if (!isObject(%this.tempAnimation))
        return;

    %name = ABNameField.getText();
    
    if (AssetDatabase.isDeclaredAsset(%name) && %name !$= %this.tempAnimation.AssetName)
    {
        NoticeGui.display("An animation with this name already exists. Please choose another");
        //MessageBoxOK("Animation Editor", "An animation with this name already exists. Please choose another", "");
        return;
    }
    if (%name !$= ABNameField.animName)
    {
        ABNameField.animName = %name;
        %this.animationIsDirty = true;
    }

    %assetId = %this.tempAnimation.getAssetId();
    %assetName = %this.tempAnimation.AssetName;
    
    AssetDatabase.releaseAsset(%this.tempAnimation.getAssetId());
    
    %newId = strreplace(%assetId, %assetName, %name);
    AssetDatabase.renameDeclaredAsset(%assetId, %newId);
    
    %this.tempAnimation = AssetDatabase.acquireAsset(%newId);
    
    ABNameField.setText(%name);
}

function AnimationBuilder::insertFrame(%this, %frame, %position)
{
    if (!isObject(%this.tempAnimation))
        return;

    %this.animationIsDirty = true;

    %frames = %this.tempAnimation.animationFrames;
    %frameCount = getWordCount(%frames);
    
    if (%position > %frameCount)
        %position = %frameCount;
    else if (%position < 0)
        %position = 0;
   
    %this.tempAnimation.animationTime = (%frameCount + 1) / ABFPSField.getText();
   
    %newFrames = "";
    for (%i = 0; %i <= %frameCount; %i++)
    {
        if (%i == %position)
            %newFrames = %newFrames SPC %frame;

        if (%i < %frameCount)
            %newFrames = %newFrames SPC getWord(%frames, %i);
    }
   
    %this.tempAnimation.animationFrames = trim(%newFrames);
    %this.updateGui();

    %scrollCtrl = ABStoryboardWindow.getParent();
    %worldWidth = ABStoryboardWindow.colSize + ABStoryboardWindow.colSpacing;
    %windowWidth = getWord(ABStoryboardWindow.Extent, 0);
    %scrollCtrl.setScrollPosition(%windowWidth * %position, 0);
}

function AnimationBuilder::removeFrame(%this, %position)
{
    if (!isObject(%this.tempAnimation))
        return;

    %this.animationIsDirty = true;

    %frames = %this.tempAnimation.animationFrames;
    %frameCount = getWordCount(%frames);

    %this.tempAnimation.animationTime = (%frameCount - 1) / ABFPSField.getText();

    %newFrames = "";
    for (%i = 0; %i < %frameCount; %i++)
    {
        if (%i != %position)
            %newFrames = %newFrames SPC getWord(%frames, %i);
    }

    %this.tempAnimation.animationFrames = trim(%newFrames);

    AnimBuilderEventManager.postEvent("_TimelineDeleteRequest");
}

function AnimationBuilder::appendFrame(%this, %frame)
{
    if (!isObject(%this.tempAnimation))
        return;

    %this.insertFrame(%frame, getWordCount(%this.tempAnimation.animationFrames));
}

function AnimationBuilder::setAllFrames(%this)
{
    if (!isObject(%this.tempAnimation))
        return;

    %this.clearFrames();
    %image = AssetDatabase.acquireAsset(%this.tempAnimation.ImageMap);
    %frameCount = %image.getFrameCount();
    AssetDatabase.releaseAsset(%this.tempAnimation.ImageMap);
    
    for (%i = 0; %i < %frameCount; %i++)
        %this.appendFrame(%i);
}

function AnimationBuilder::clearFrames(%this)
{
    if (!isObject(%this.tempAnimation))
        return;

    %frameCount = %this.tempAnimation.getFrameCount();
    
    for (%i = 0; %i < %frameCount; %i++)
        %this.removeFrame(0);
}

function AnimationBuilder::populateTagContainer(%this)
{
   if (!isObject(%this.tempAnimation))
      return;
      
   %tagCount = getWordCount(%this.tempAnimation.NameTags);
   
   for(%i = 0; %i < %tagCount; %i++)
   {
      %tagID = getWord(%this.tempAnimation.NameTags, %i);
      
      %tag = ProjectNameTags.getTagName(%tagID);
      
      %verticalPosition = (30 * %i) + 5;
      %horizontalPosition = 7;
      %position = %horizontalPosition SPC %verticalPosition;
      
      CreateTagBar(ABTagContainer, %position, %tag);
   }
}

function AnimationBuilder::createDraggingControl(%this, %sprite, %spritePosition, %mousePosition, %size)
{
    if (!isObject(%this.tempAnimation))
        return;

    // Create the drag and drop control.
    %dragControl = new GuiDragAndDropControl()
    {
        Profile = "GuiDragAndDropProfile";
        Position = %spritePosition;
        Extent = %size;
        deleteOnMouseUp = true;
    };

    // And the sprite to display.
    %spritePane = new GuiSpriteCtrl()
    {
        scene = %this.draggingScene;
        Extent = %size;
        Image = %sprite.Image;
        Frame = %sprite.Frame;
        frameNumber = %sprite.frameNumber;
        spriteClass = %sprite.class;
    };

    //if (%sprite.getClassName() $= "GuiSpriteCtrl")
        //echo(" @@@ Image: " @ %this.sourceImageMap @ " Frame: " @ %sprite.getImageFrame());
    //else
        //echo(" @@@ Image: " @ %this.sourceImageMap @ " Frame: " @ %sprite.getFrame());

    // Place the guis.
    AnimationBuilderGui.add(%dragControl);
    %dragControl.add(%spritePane);

    // Figure the position to place the control relative to the mouse.
    %xOffset = getWord(%mousePosition, 0) - getWord(%spritePosition, 0);
    %yOffset = getWord(%mousePosition, 1) - getWord(%spritePosition, 1);

    %dragControl.startDragging(%xOffset, %yOffset);
}

function AnimationBuilder::addTag(%this, %tag)
{
    if (%tag $= "")
        return;

    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagManifest.getAssetTagCount(%this.animationId);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %tempTag = %assetTagManifest.getAssetTag(%this.animationId, %i);
        if (%tag $= %tempTag)
        {
            echo(" @@@ asset already has tag " @ %tag);
            return;
        }
    }
    %assetTagManifest.tag(%this.animationId, %tag);
    ABTagContainer.addTagItem(%tag);
    ABTagContainer.populateTagList(%this.animationId);
}

function AnimationBuilder::applyTag(%this, %tag)
{
}

function AnimationBuilder::deleteTagFromList(%this, %tag)
{
}

function AnimationBuilder::changeImageMap(%this)
{
    // If the user already has an image loaded but no animation frames or tags made/changed, 
    // Select will bring up the Asset Selector without any prompts

    // If the user has an image with some tags made, but no animation frames; 
    // Select will prompt the user that they are about to lose any unsaved work, do they want to continue 

    // If the user has an image with frames of animation and no tags;
    // Select will prompt the user that they are about to lose any unsaved work, do they want to continue

    // If the user has an image, frames of animation and tags; 
    // Select will prompt the user that they are about to lose any unsaved work, do they want to continue

    // If the user has changed ONLY the Frame Rate and/or Loop Animation checked;
    // Select will bring up the Asset Selector, unless another case above has occurred
    %frames = %this.tempAnimation.animationFrames;

    %frameCount = getWordCount(%frames);

    if (%frameCount <= 0 || %this.animationIsDirty != 1)
    {
        if (%this.newAnimation && isObject(%this.tempAnimation))
            %this.tempAnimation.delete();

        AssetPicker.open("ImageAsset", "Any", "", AnimationBuilderGui.getId());
    }
    else
    {
        %data = "ImageAsset" SPC "Any" SPC "AnimationBuilderGui";
        ConfirmActionGui.display("You are about to lose any unsaved work. Are you sure you want to continue?", "AnimationBuilder", "openAssetPicker", %data);
        //MessageBoxOKCancel("Continue", "You are about to lose any unsaved work. Are you sure you want to continue?", "AssetLibrary.open(AnimationBuilderGui, $SpriteSheetPage, \"\", \"\", true);", "AnimationBuilder.updateGui();");
    }
}

function AnimationBuilder::openAssetPicker(%this, %data)
{
    %type = getWord(%data, 0);
    %tag = getWord(%data, 1);
    %tool = getWord(%data, 2);
    %category = "";
    AssetPicker.open(%type, %tag, %category, %tool);
}

// This will not work.
function AnimationBuilder::deleteAnimation(%this, %animation)
{
    %scene = ToolManager.getLastWindow().getScene();
    %referenceList = new SimSet();
    %referenceCount = %scene.getAnimationReferenceList(%animation, %referenceList);

    %message = "Do you really want to delete this animation?";
    if (%referenceCount > 0)
    {
        %message = getCountString("There {are} {count} object{s}", %referenceCount) @ " referencing this animation. If the" NL
            "animation is deleted, the objects will also be deleted. Continue?";
    }

    %result = messageBox("Delete Animation", %message, "OkCancel", "Question");

    if (%result == $MROk)
    {
        LBProjectObj.removeDatablock(%animation);

        %referenceList.deleteContents();

        %animation.delete();

        GuiFormManager::SendContentMessage($LBCAnimatedSprite, %this, "refresh 1");
    }

    %referenceList.delete();
}

package AnimationBuilderPackage
{

function AnimationAsset::getFrameCount(%this)
{
    return getWordCount(%this.animationFrames);
}

};
