//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$ImageEditorMinCellSizeX = 4;
$ImageEditorMinCellSizeY = 4;

function ImageEditor::editImage(%this, %image)
{
    if (!isObject(%image))
        return;
      
    if (isObject($ImageEditorScene))
        $ImageEditorScene.delete();
        
    $ImageEditorScene = new Scene(ImageEditorScene);

    Canvas.pushDialog(ImageBuilderGui);
    ImageEditor.launchImageEditor(%image);
}

// --------------------------------------------------------------------
// function ImageEditor::launchImageEditor()
//
// This is called both when a new image is created and when an already
// existing image map is being edited.  We copy the image map datablock
// into a temporary holder so if we cancel we can reset the settings.
// --------------------------------------------------------------------
function ImageEditor::launchImageEditor(%this, %image)
{  
    ImageEditor.setupPreviewWindow();

    // Store the starting name for name checks
    %this.sourceName = %image.AssetName;
    %this.assetName = %image.AssetName;
    %this.sourceDatablock = %image.getID();

    populateTemporaryAsset(%image, "TempImage");
    
    %this.assetSourceId = %this.tempDatablock.getAssetId();

    if (%image.imageFile !$= "")
    {
        ImageBuilderImageLocation.setText(%image.imageFile); 
    }
        
    ImageEditorTagList.refresh(0, "");

    %this.loadPreview(%this.tempDatablock);

    ImageEditorTagContainer.populateTagList(%this.assetSourceId);
    
    ImageEditorSaveButton.update();
    
    ImageEditorAutoApply();
}


// --------------------------------------------------------------------
// ImageEditor::saveImage()
//
// This gets called when the user clicks the save button, it clears
// out the proper data and adds the newly created image to the
// managed datablock (if it isn't already there in the case of editing), 
// as well as calling the object libraries to refresh.
// --------------------------------------------------------------------
function ImageEditor::saveImage(%this)
{
    // hide away our Image Builder GUI
    ImageEditor.forcePreviewClear = false;
    Canvas.popDialog(ImageBuilderGui);
    
    %image = %this.sourceDatablock;
    %previewImage = %this.tempDatablock;

    %imageDBName = ImageEditorTxtImageName.getValue();
    %imageDBName = strreplace(%imageDBName, " ", "_");   

    restoreDB(%previewImage.getID(), %image.getID(), %imageDBName);
    
    // Copy tags
    %this.copyTags(%this.tempDatablock, %this.sourceDatablock); 

    // if we're creating a new image then add it to the managed datablocks
    if (!%this.editing)
    {
        %pathNoExtension = expandPath("^{UserAssets}/images/") @ %imageDBName;

        TamlWrite(%image, %pathNoExtension @ ".asset.taml");
        
        %image.delete();
        
        %moduleDefinition = ModuleDatabase.getDefinitionFromId("{UserAssets}");
        AssetDatabase.addSingleDeclaredAsset(%moduleDefinition, %pathNoExtension @ ".asset.taml");
        
        
        removeResPath(filePath(%newFileLocation));
    }
    else
    {   
        %path = expandPath("^{UserAssets}/images/");
        
        // We have changed the name of the asset we are editing. Need to update
        if (%imageDBName !$= %this.sourceName)
        {
            %assetModule = AssetDatabase.getAssetModule(%image.getAssetId()).ModuleId;
            %newID = %assetModule @ ":" @ %imageDBName;
            
            AssetDatabase.renameDeclaredAsset(%image.getAssetId(), %newID);
            AssetDatabase.renameReferencedAsset(%image.getAssetId(), %newID);
            
            if (isFile(%path @ %this.sourceName @ ".asset.taml"))
                fileDelete(%path @ %this.sourceName @ ".asset.taml");
            TamlWrite(%image, %path @ %imageDBName @ ".asset.taml");
        }
        else
        {
            TamlWrite(%image, %path @ %this.sourceName @ ".asset.taml");
            
            //%assetModule = AssetDatabase.getAssetModule(%image.getAssetId()).ModuleId;
            //%assetID = %assetModule @ ":" @ %this.sourceName;        
            //
            //if (!AssetDatabase.isDeclaredAsset(%assetID))
            //{
                //%moduleDefinition = ModuleDatabase.getDefinitionFromId("{UserAssets}");
                //AssetDatabase.addSingleDeclaredAsset(%moduleDefinition, %path @ %this.sourceName @ ".asset.taml");
            //}
            
        }
        
        
        
        // If the image file does not already exist in the UserAssets directory
        // copy it there
        %newImageFileLocation = findImageFileLocation(%path, fileBase(%image.ImageFile));
        if (%newImageFileLocation $= "")
        {
            %oldImageFileLocation = findImageFileLocation(filePath(%image.ImageFile), fileBase(%image.ImageFile));
            if (%oldImageFileLocation !$= "")
            {
                %newImageFileLocation = %path @ fileBase(%oldImageFileLocation) @ fileExt(%oldImageFileLocation);
                pathCopy(%oldImageFileLocation, %newImageFileLocation);
            }
        }

        AssetDatabase.releaseAsset(%this.sourceDatablock.getAssetId());
        AssetDatabase.releaseAsset(%this.tempDatablock.getAssetId());
        
        if (isObject(%this.sourceDatablock))
            %this.sourceDatablock.delete();
            
        if (isObject(%this.tempDatablock))
            %this.tempDatablock.delete();
        
        // Just update our set
        LBProjectObj.persistToDisk();
    }
    
    // clear the preview and our data
    %this.clearPreview();
    %this.clearData();
    
    %this.lastCopiedImageFile = "";
    
    AssetLibrary.schedule(100, "updateGui");
}

// --------------------------------------------------------------------
// ImageEditor::cancel()
//
// This gets called when the user clicks the cancel button, it cleans up
// some resources and then closes out the Image Builder.
// --------------------------------------------------------------------
function ImageEditor::cancel(%this)
{
    // rdbnote: remove the image from the ignore list
    $ignoredDatablockSet.remove(%this.sourceDatablock);

    %this.clearTags(%this.tempDatablock);
    
    if (!%this.editing)
    {
        //AssetDatabase.releaseAsset(%this.tempDatablock.getAssetId());
        //AssetDatabase.removeSingleDeclaredAsset(%this.tempDatablock.getAssetId());
        if (isObject(%this.sourceDatablock))
            %this.sourceDatablock.delete();
        if (isObject(%this.tempDatablock))
            %this.tempDatablock.delete();
    }

    %this.clearPreview();
    %this.clearData();
    
    // Delete any unused image files that were copied by the image editor
    %this.removeLastCopiedImageFile();
    
    Canvas.popDialog(ImageBuilderGui);
}

// --------------------------------------------------------------------
// ImageEditor::imageFileBrowser()
//
// This gets called when the user clicks the file browser button, it 
// calls the cancel functionality then spawns the file browser.
// --------------------------------------------------------------------
function ImageEditor::imageFileBrowser(%this)
{
    %image = %this.selectedImage;

    if (!isObject(%image))
        return; 

    %dlg = new OpenFileDialog()
    {
        Filters = $T2D::ImageSpec;
        //DefaultFile = %image.getImageName();
        ChangePath = false;
        MustExist = true;
        MultipleFiles = false;
    };

    // Update Image        
    if (%dlg.Execute())
    {
        %fileName = %dlg.FileName;
        %fileOnlyName = fileName(%fileName);   

        %checkPath    = expandPath("^{UserAssets}/images/");
        %fileOnlyPath = filePath(expandPath(%fileName) );
        %fileBasePath = getSubStr(%fileOnlyPath, 0, strlen(%checkPath) ); 

        // Check file validity
        if (!isValidImageFile(%fileName))
        {
            MessageBoxOK("Warning", "'"@%fileOnlyName@"' is not a valid image file.", "");
            return;
        }   

        if (%checkPath !$= %fileBasePath)         
        {                     
                       
            
            %newFileLocation = expandPath("^{UserAssets}/images/" @ %fileOnlyName);

            if (!isValidImageFile(%newFileLocation))
            {
                %this.removeLastCopiedImageFile();
                
                addResPath(filePath(%newFileLocation) );

                pathCopy(%fileName, %newFileLocation);
            
                %this.lastCopiedImageFile = %newFileLocation;
            }
        }  

        %noExt = strreplace(%newFileLocation, ".jpg", "");
        %noExt = strreplace(%noExt, ".jpeg", "");
        %noExt = strreplace(%noExt, ".png", "");
        %noExt = strreplace(%noExt, ".pvr", "");

        %image.imageFile = %noExt;
        %this.reCreateImage(%this.selectedImage);
        ImageEditorAutoApply();
    }
    
    %dlg.delete();   
}

// --------------------------------------------------------------------
// ImageEditor::delete()
//
// This gets called when the user clicks the delete button, it will prompt
// the user to make sure they do in fact want to delete this image map.
// --------------------------------------------------------------------
function ImageEditor::delete(%this)
{
    // prompt the user with how many references this image map has
    checkImageReference(%this.sourceDatablock, Toolmanager.getLastWindow().getScene(), "ImageEditor.cancel();");

    // [neo, 15/6/2007 - #3231]
    // This doesnt exist so forward it to the SBCreateTrash method.
    // checkImageReferences(%this.sourceDatablock, Toolmanager.getLastWindow().getScene(), "ImageEditor.cancel();");   

    // Save image map ref as this will be cleared when we call cancel()
    %image = %this.sourceDatablock;

    // Close first so it cleans up correctly
    %this.cancel();

    // Take out the trash...
    SBCreateTrash::deleteImage(%image);

    // set the tool to the selection tool otherwise next click you'll try and create
    // and image that doesn't exist 
    LevelBuilderToolManager::setTool(LevelEditorSelectionTool);
}

function ImageEditor::copyTags(%this, %imageSource, %imageDest)
{
    // Clear tags on the destination asset
    %this.clearTags(%imageDest);
    
    //
    %assetSourceID = %imageSource.getAssetID();
    %assetDestID = %imageDest.getAssetID();
    %assetTagsManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagsManifest.getAssetTagCount(%assetSourceID);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %assetTagsManifest.tag(%assetDestID, %assetTagsManifest.getAssetTag(%assetSourceID, %i));
    }
}

function ImageEditor::clearTags(%this, %image)
{
    %assetID = %image.getAssetID();
    %assetTagsManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagsManifest.getAssetTagCount(%assetID);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %assetTagsManifest.untag(%assetID, %assetTagsManifest.getAssetTag(%assetID, 0));
    }
}

function ImageEditor::clearData(%this)
{
    %this.startingName = "";
    %this.selectedImage = "";
    %this.tempDatablock = "";
    %this.sourceDatablock = "";
}

function ImageEditor::removeLastCopiedImageFile(%this)
{
    // Check if the file exists
    if (!isFile(%this.lastCopiedImageFile))
    {
        %this.lastCopiedImageFile = "";
        return;   
    }
    
    //// Check if the file is used by any other assets
    //%assetQuery = new AssetQuery();
    //AssetDatabase.findAllAssets(%assetQuery, false);
    //%assetCount = %assetQuery.Count;
//
    //for( %index = 0; %index < %assetCount; %index++ )
    //{
        //%assetId = %assetQuery.getAsset( %index );
        //%asset = AssetDatabase.acquireAsset(%assetId);
        //if (%asset.ImageFile $= %imageFilePath)
        //{
             //AssetDatabase.releaseAsset(%assetId);
             //%assetQuery.delete(); 
             //return;  
        //}
            //
        //AssetDatabase.releaseAsset(%assetId);
    //}
    //%assetQuery.delete(); 
    
    // If we reach this point, no asset should be using the image file
    // so we can safely delete it.
    fileDelete(%this.lastCopiedImageFile);
    %this.lastCopiedImageFile = "";
}

// --------------------------------------------------------------------
// ImageEditor::reCreateImage()
//
// re-creates the image for the image, this does a majority of the work
// it will loop through and set each image setting appropriately
// based on image mode.  It will also set some defaults and some fail 
// safes to make sure people cannot set invalid values.
// --------------------------------------------------------------------
function ImageEditor::reCreateImage(%this, %image)
{  
    if (!isObject(%image))
        return;

    %image = %image.getImageFile();

    %srcSize = %image.getImageSize();

    %sizeX = getWord(%srcSize, 0);
    %sizeY = getWord(%srcSize, 1);

    //%xSizeCheck = %sizeX / 16;
    //%ySizeCheck = %sizeY / 16;
    %xSizeCheck = $ImageEditorMinCellSizeX;
    %ySizeCheck = $ImageEditorMinCellSizeY;
    %xCountCheck = mRound(%sizeX / $ImageEditorMinCellSizeX);
    %yCountCheck = mRound(%sizeY / $ImageEditorMinCellSizeY);

    %this.AssetName = ImageEditorTxtImageName.getValue();

    if (isObject(%this.sourceDatablock))
        %sourceName = %this.sourceDatablock.AssetName;
    else
        %sourceName = %this.AssetName;
   
    %assetQuery = new AssetQuery();
    AssetDatabase.findAssetName(%assetQuery, %this.AssetName);
    
    %assetExists = %assetQuery.Count > 0;
    
    // Check to make sure we aren't conflicting with another object's name
    if ((ImageEditor.sourceName !$= %this.AssetName && %assetExists) && %this.AssetName !$= %sourceName)
    {
        MessageBoxOK("Warning", "An image with this name already exists.", "");

        %newName = %sourceName;

        ImageEditorTxtImageName.setText(%newName);

        %this.AssetName = %newName;
    }

    // Fetch cell count.
    %cellCountX = ImageEditorCellCountX.getValue();
    %cellCountY = ImageEditorCellCountY.getValue();      
    %cellHeight = ImageEditorCellHeight.getValue();
    %cellWidth = ImageEditorCellWidth.getValue();
   
    // first we check the cell width and height for
    // reasonable values
    if (%cellWidth == 0)
        %cellWidth = %sizeX;  
    else if (%cellWidth < %xSizeCheck)
        %cellWidth = %xSizeCheck;
        
    if (%cellWidth > %sizeX)
        %cellWidth = %sizeX;
        
    if (%cellHeight == 0)
        %cellHeight = %sizeY;  
    else if (%cellHeight < %ySizeCheck)
        %cellHeight = %ySizeCheck;
    
    if (%cellHeight > %sizeY)
        %cellHeight = %sizeY;

    // Clamp cell counts.
    if (%cellCountX < 1)
        %cellCountX = 1;

    if (%cellCountY < 1)
        %cellCountY = 1;
        
    if (%cellCountX > %xCountCheck)
        %cellCountX = %xCountCheck;
    
    if (%cellCountY > %yCountCheck)
        %cellCountY = %yCountCheck;
         
    //if (%cellCountX > 1 || %cellCountY > 1)
    //{
        %div = mFloor(%sizeX / %cellWidth);

        if (ImageEditor.CountExtentOverride $= "Extent")
        {
            if (ImageEditor.linkProperties == true)           
                %cellCountX = %div;
            
            if (%cellCountX > %div)
                %cellCountX = %div;
        }
        else
        {
            %widthFloor = mFloor(%sizeX / %cellCountX);
            if (ImageEditor.linkProperties == true)
                %cellWidth = %widthFloor;

            if (%cellWidth >= 2048)
                %cellWidth = %sizeX;
            else if (%cellWidth <= 0)
                %cellWidth = $ImageEditorMinCellSizeX;
            if (%cellWidth > %widthFloor)
                %cellWidth = %widthFloor;
            
        }

        %div = mFloor(%sizeY / %cellHeight);

        if (ImageEditor.CountExtentOverride $= "Extent")
        {
            if (ImageEditor.linkProperties == true)            
                %cellCountY = %div;
                
            if (%cellCountY > %div)
                %cellCountY = %div;
        }
        else
        {
            %heightFloor = mFloor(%sizeY / %cellCountY);
            if (ImageEditor.linkProperties == true)
                %cellHeight = %heightFloor;

            if (%cellHeight >= 2048)
                %cellHeight = %sizeY;
            else if (%cellHeight <= 0)
                %cellHeight = $ImageEditorMinCellSizeY;
            if (%cellHeight > %heightFloor)
                %cellHeight = %heightFloor;
        }
    //}
 
    // Set common configuration.
    %image.imageFile = %image;
    %image.filterMode = "None";
    %image.cellCountX = %cellCountX;
    %image.cellCountY = %cellCountY;

    %srcSize = %image.getImageSize();
  
    // Split-up bitmap size.
    %sizeX = getWord(%srcSize, 0);
    %sizeY = getWord(%srcSize, 1);      

    // Calculate cell configuration.
    %image.cellCountX   = %cellCountX;
    %image.cellCountY   = %cellCountY;
    %image.cellHeight   = %cellHeight;
    %image.cellWidth    = %cellWidth;
    %image.cellOffsetX  = 0;
    %image.cellOffsetY  = 0;
    %image.cellStrideX  = 0;
    %image.cellStrideY  = 0;
    %image.cellRowOrder = true;
    %image.filterMode = "NONE";
    %image.filterPad = false;

   
    if (ImageEditor.sourceName !$= %this.AssetName)
        updateDependentObjects(ImageEditor.sourceName.getName(), %this.AssetName);

    ImageEditor.sourceName = %this.AssetName;
}

//function ImageEditor::populateTagContainer(%this)
//{
    //%assetTags = AssetDatabase.getAssetTags();
    //
    //%tagCount = %assetTags.getTagCount();
    //
    //for(%i = 0; %i < %tagCount; %i++)
    //{
        //%tag = %assetTags.getTag(%i);
//
        //%verticalPosition = (30 * %i) + 5;
        //%horizontalPosition = 7;
        //%position = %horizontalPosition SPC %verticalPosition;
//
        //CreateTagBar(ImageEditorTagContainer, %position, %tag);
    //}
//}

function ImageEditor::addTag(%this, %tag)
{
    if (%tag $= "" || %tag $= "Any")
        return;

    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagManifest.getAssetTagCount(%this.assetSourceId);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %tempTag = %assetTagManifest.getAssetTag(%this.assetSourceId, %i);
        if (%tag $= %tempTag)
        {
            echo(" @@@ asset already has tag " @ %tag);
            return;
        }
    }
    %assetTagManifest.tag(%this.assetSourceId, %tag);
    ImageEditorTagContainer.addTagItem(%tag);
    ImageEditorTagContainer.populateTagList(%this.assetSourceId);
    //if (%tag $= "")
        //return;    
    //
    //%assetTagManifest = AssetDatabase.getAssetTags();
    //%assetTagManifest.tag(%this.assetSourceId, %tag);
    //ImageEditorTagContainer.addTagItem(%tag);
}

function ImageEditor::removeTag(%this, %tag)
{
    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagManifest.untag(%this.assetSourceId, %tag);
    ImageEditorTagContainer.removeTagItem(%tag);
    ImageEditorTagContainer.populateTagList(%this.assetSourceId);
    //%assetTagManifest = AssetDatabase.getAssetTags();
    //%assetTagManifest.untag(%this.assetSourceId, %tag); 
}

