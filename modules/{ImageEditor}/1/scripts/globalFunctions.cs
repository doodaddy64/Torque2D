//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

// --------------------------------------------------------------------
// launchNewImage()
//
// This will launch the file browser to create a new image map.
// --------------------------------------------------------------------
function launchNewImage()
{ 
    %baseName = "UntitledSprite";
    %appendix = "1";

    while(isObject(%baseName @ %appendix))
        %appendix++;

    //%fileBase = %baseName @ %appendix;
    
    %fileBase = "";

    // Create temporary image asset
    %image = new ImageAsset()
    {
        AssetName = %fileBase;
        //ImageFile = "^{EditorAssets}/data/images/DefaultImage";
        ImageFile = "";
    };

    $pref::T2D::imageEchoErrors = 1;

    ImageEditor.forcePreviewClear = true;
    ImageEditor.editing = false;   

    ImageEditor.editImage(%image); 
}

// --------------------------------------------------------------------
// launchEditImage()
//
// Passing an image map into this will cause the Image Builder to be 
// launched editing that image map.
// --------------------------------------------------------------------
function launchEditImage(%assetID)
{   
    ImageEditor.editing = true;
    
    %image = AssetDatabase.acquireAsset(%assetID);
    
    //ImageEditor.assetSourceId = %image.getAssetId();
    ImageEditor.editImage(%image);
}

function validateDatablockName(%name)
{
    // remove whitespaces at beginning and end
    %name = trim(%name);

    // If it begins with a number place a _ before it   
    // the first character
    %firstChar = getSubStr(%name, 0, 1);

    // if the character is a number remove it
    if (strpos("0123456789", %firstChar) != -1)
        %name = "_" @ %name;

    // replace whitespaces with underscores
    %name = strreplace(%name, " ", "_");

    // remove any other invalid characters
    %name = stripChars(%name, "-+*/%$&§=()[].?\"#,;!~<>|°^{}");

    if (%name $= "")
        %name = "Unnamed";
    
    // Generate valid Non-Existent name
    if (isObject(%name))
    {
        // Add numbers starting with 1 until we find a valid one
        %i = 1;
        
        while(isFile(%name @ %i))
            %i++;
        
        %name = %name @ %i;
    }

    return %name;
}

function updateDependentObjects(%previousName, %newName)
{
    // Get all the managed datablocks
    %datablockCount = $managedDatablockSet.getCount();

    // Loop through the animation datablocks and update image names
    for(%i = 0; %i < %datablockCount; %i++)
    {
        %datablock = $managedDatablockSet.getObject(%i);

        // Animations
        if (%datablock.isMemberOfClass("AnimationAsset"))
        {
            if (%datablock.image $= %previousName)
            {
                %datablock.image = %newName;
                //%datablock.calculateAnimation();
                GuiFormManager::SendContentMessage($LBCAnimatedSprite, %this, "refresh 1");
            }
        }
    }
   
    // Loop through all scene objects and update images
    %scene = ToolManager.getLastWindow().getScene();

    %count = %scene.getSceneObjectCount();

    for(%i = 0; %i < %count; %i++)
    {
        %object = %scene.getSceneObject(%i);

        // Sprites and scrollers
        if (%object.isMemberofClass("t2dStaticSprite") || %object.isMemberofClass("Scroller"))
        {
            %imageName = %object.getImage().getName();

            if (%imageName $= %previousName)
                %object.setImage(%newName);
        }

        if (%object.isMemberOfClass("t2dAnimatedSprite"))
        {
            if (%object.animationName $= %previousName)
                %object.setAnimation(%newName);
        }
    }
}

// --------------------------------------------------------------------
// loadImageSettings()
//
// This is the first function that will load all of the base image map 
// settings, it will then check which image mode it is and call an
// image mode specific function to load the mode type settings, if
// the image is in full mode then it doesn't need to call anything else.
// --------------------------------------------------------------------
function loadImageSettings(%image)
{  
    ImageEditor.loadingSettings = true;

    $ImageEditorSelectedImage = %image;

    // Set name.
    %assetName = ImageEditor.assetName;
    
    if (%assetName !$= "")
    {
        ImageEditorTxtImageName.setText(%assetName);
    }
        
    if (%image.imageFile !$= "")
    {
        ImageBuilderImageLocation.setText(%image.imageFile); 
    }

    // Fetch cell counts.
    %cellCountX = %image.cellCountX;
    %cellCountY = %image.cellCountY;
   
    // Clamp cell counts.
    if (%cellCountX < 1)
    {
        %cellCountX = 1;
        %image.cellCountX = 1;
    }
    if (%cellCountY < 1)
    {
        %cellCountY = 1;
        %image.cellCountY = 1;
    }
   
    // Update controls.
    ImageEditorCellCountX.setText(%cellCountX);
    ImageEditorCellCountY.setText(%cellCountY);

    loadImageCellSettings(%image);      

   
    // Originally you could set the check boxes for these values and save
    // them for ONE image data block ONLY. When you would try to set the 
    // check boxes for the first image its value would be set to one of
    // these global variables. When trying to do the same for a second
    // image the second images check box value was compared to this global
    // value and if they where the same the second image's data block was
    // never updated. To fix this I am setting these global values to the
    // the values for the image when it was last saved.
   
    $lastComoressPVRValue = false;
    $lastPreloadValue = false;
    $lastAllowUnloadValue = false;
    $lastPreferPerfValue = true;
    $lastFilterPadValue = false;
    $lastUseHDValue = false;

    ImageEditor.loadingSettings = false;
}
// --------------------------------------------------------------------
// loadImageCellSettings()
//
// The will load the cell specific settings from the image map selected.
// --------------------------------------------------------------------
function loadImageCellSettings(%image)
{
    %cellCountX = %image.cellCountX;
    %cellCountY = %image.cellCountY;
    %cellHeight = %image.getCellHeight();
    %cellWidth = %image.getCellWidth();
    %cellOffsetX = %image.cellOffsetX;
    %cellOffsetY = %image.cellOffsetY;
    %cellStrideX = %image.cellStrideX;
    %cellStrideY = %image.cellStrideY;
    %cellRowOrder = %image.cellRowOrder;

    ImageEditorCellCountX.setText(%cellCountX);
    ImageEditorCellCountY.setText(%cellCountY);
    ImageEditorCellHeight.setText(%cellHeight);
    ImageEditorCellWidth.setText(%cellWidth); 
}

function generateGuiImageFromFile(%filePath)
{
    %baseFileName = fileName(%filePath);
    %newFileLocation = expandPath("^{UserAssets}/images/" @ %baseFileName); 
    
    addResPath(filePath(%newFileLocation));
       
    if (isFile(%newFileLocation))
    {
        %uniqueFileName = generateUniqueFileName(%newFileLocation);
        %message = "A file called" SPC %baseFileName SPC "already exists. Would you like to replace it or create a new file called" SPC fileName(%uniqueFileName) @ "?";
        
        %replaceCommand = "saveImageAsSpriteAsset(\"" @ %filePath @ "\", \"" @ %newFileLocation @ "\", false);";
        %createNewCommand = "saveImageAsSpriteAsset(\"" @ %filePath @ "\", \"" @ %uniqueFileName @ "\", true);";
        %cancelCommand = "cancelNewSpriteAsset();";
        
        WarningDialog.setupAndShow(Canvas.getGlobalCenter(), "", %message, "Replace the Existing File", %replaceCommand, "Create a new file", %createNewCommand, "Cancel", %cancelCommand);
    }
    else
    {
        saveImageAsSpriteAsset(%filePath, %newFileLocation, true);
    }
    
    removeResPath(%newFileLocation);
    
    AssetLibrary.schedule(100, "updateGui");
}

function generateUniqueFileName(%fileLocation)
{
    %counter = 1;
    
    %newFileLocation = %fileLocation;
    
    %path = filePath(%fileLocation);
    %fileName = fileBase(%fileLocation);
    %extension = fileExt(%fileLocation);
    
    %basePathName = %fileName;
    
    while(isFile(%newFileLocation))
    {
        %newFileLocation = %path @ "/" @ %fileName @ %counter @ %extension;
        %counter++;
    }
    
    return %newFileLocation;
}

function saveImageAsSpriteAsset(%fileToCopy, %newFileLocation, %isNewImage)
{
    pathCopy(%fileToCopy, %newFileLocation);
    
    if (%isNewImage)
    {
        %fileOnlyName = fileName(%newFileLocation);
        %extension = fileExt(%newFileLocation);
        %fileOnlyName = strreplace(%fileOnlyName, %extension, "");
        
        %image = new ImageAsset();
        
        %pathNoExtension = strreplace(%newFileLocation, %extension, "");
        
        %name = %fileOnlyName @ "Image";
        %image.assetName = %name;
        %image.AssetCategory = "gui";
        %image.imageFile = %pathNoExtension;
        %image.filterMode = "NONE";
        
        TamlWrite(%image, %pathNoExtension @ ".asset.taml");
        
        %image.delete();
        
        %moduleDefinition = ModuleDatabase.getDefinitionFromId("{UserAssets}");
        AssetDatabase.addSingleDeclaredAsset(%moduleDefinition, %pathNoExtension @ ".asset.taml");
    }
    else
    {
        %assetQuery = new AssetQuery();
        
        AssetDatabase.findAssetType(%assetQuery, "ImageAsset");
        
        %assetCount = %assetQuery.Count;
 
        for(%index = 0; %index < %assetCount; %index++)
        {
            %assetId = %assetQuery.getAsset(%index);
            
            %image = AssetDatabase.acquireAsset(%assetId);
            
            if (expandPath(%image.imageFile) $= %newFileLocation)
                %image.compile();
                
            AssetDatabase.releaseAsset(%assetId);
        }
        
        %assetQuery.delete();
    }
}

function populateTemporaryAsset(%originalDatablock, %newName)
{
    %assetQuery = new AssetQuery();
    AssetDatabase.findAssetName(%assetQuery, "TempImage");
    
    if (%assetQuery.Count > 0)
    {
        ImageEditor.tempDatablock = AssetDatabase.acquireAsset(%assetQuery.getAsset(0));
    }
    else
    {
        ImageEditor.tempDatablock  = new ImageAsset()
        {
            AssetName = "TempImage";
            imageFile = "^{EditorAssets}/data/images/DefaultImage";
            imageMode = FULL;
            AssetInternal = true;
        };
        
        TamlWrite(ImageEditor.tempDatablock, "^{EditorAssets}/data/images/TempImage.asset.taml");
    
        ImageEditor.tempDatablock.delete();
        
        %moduleDefinition = ModuleDatabase.getDefinitionFromId("{EditorAssets}");
        AssetDatabase.addSingleDeclaredAsset(%moduleDefinition, "^{EditorAssets}/data/images/TempImage.asset.taml");
        
        // Acquire the asset datablock for use
        ImageEditor.tempDatablock = AssetDatabase.acquireAsset("{EditorAssets}:TempImage");
    }
    
    ImageEditor.tempDatablock.imageFile = %originalDatablock.imageFile;
    ImageEditor.tempDatablock.cellWidth = %originalDatablock.cellWidth;
    ImageEditor.tempDatablock.cellHeight = %originalDatablock.cellHeight;
    ImageEditor.tempDatablock.filterMode = %originalDatablock.filterMode;
    ImageEditor.tempDatablock.cellRowOrder = %originalDatablock.cellRowOrder;
    ImageEditor.tempDatablock.cellOffsetX = %originalDatablock.cellOffsetX;
    ImageEditor.tempDatablock.cellOffsetY = %originalDatablock.cellOffsetY;
    ImageEditor.tempDatablock.cellStrideX = %originalDatablock.cellStrideX;
    ImageEditor.tempDatablock.cellStrideY = %originalDatablock.cellStrideY;
    ImageEditor.tempDatablock.cellCountX = %originalDatablock.cellCountX;
    ImageEditor.tempDatablock.cellCountY = %originalDatablock.cellCountY;
    ImageEditor.tempDatablock.force16Bit = %originalDatablock.force16Bit;
    
    ImageEditor.copyTags(%originalDatablock, ImageEditor.tempDatablock);
}

function restoreDB(%oldDatablockId, %newDatablockId, %newName)
{
    if((%oldDatablockId $= "" || %newDatablockId $= "") || !isObject(%oldDatablockId) || !isObject(%newDatablockId))
        return;

    %newDatablockId.AssetName = %newName;
    %newDatablockId.imageFile = %oldDatablockId.imageFile;
    %newDatablockId.filterMode = %oldDatablockId.filterMode;
    %newDatablockId.force16Bit = %oldDatablockId.force16Bit;

    %newDatablockId.cellCountX = %oldDatablockId.cellCountX;
    %newDatablockId.cellCountY = %oldDatablockId.cellCountY;
    %newDatablockId.cellHeight = %oldDatablockId.cellHeight;
    %newDatablockId.cellWidth = %oldDatablockId.cellWidth;
    %newDatablockId.cellOffsetX = %oldDatablockId.cellOffsetX;
    %newDatablockId.cellOffsetY = %oldDatablockId.cellOffsetY;
    %newDatablockId.cellStrideX = %oldDatablockId.cellStrideX;
    %newDatablockId.cellStrideY = %oldDatablockId.cellStrideY;
    %newDatablockId.cellRowOrder = %oldDatablockId.cellRowOrder; 

    //%newDatablockId.compile();

    return %newDatablockId.getID();  
}

function findImageFileLocation(%filePath, %fileBase)
{
   for (%f = findFirstFile(%filePath @ "*"); %f !$= ""; %f = findNextFile(%filePath @ "*"))
   {
        if(%fileBase $= fileBase(%f))
        { 
            %ext = fileExt(%f);
            if (strstr($T2D::ImageSpec, %ext) != -1) 
                return %f;
        }
   }
   
   return "";
}