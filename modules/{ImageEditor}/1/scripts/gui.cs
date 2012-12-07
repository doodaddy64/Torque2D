//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

$AssetNameTextEditMessageString = "Enter asset name...";
$AssetLocationTextEditMessageString = "Click here to select a file...";

// --------------------------------------------------------------------
// ImageBuilderGui::onSleep()
//
// This ensures when we close the ImageBuilderGui the global is toggled
// to false.
// --------------------------------------------------------------------
function ImageBuilderGui::onSleep(%this)
{
    $ImageEditorLoaded = false;   
}

//--------------------------------
// Sprite Sheet Tool Help
//--------------------------------
function SpriteSheetHelpButton::onClick(%this)
{
    gotoWebPage("http://docs.3stepstudio.com/" @ $LoadedTemplate.Name @ "/spritesheet/");
}

// --------------------------------------------------------------------
// ImageEditorAutoApply()
//
// This is the auto apply funcion called by almost every editable control
// in the Image Builder, it makes sure the image editor is loaded
// and that we aren't currently loading a preview (otherwise it re-loads
// everything a number of extra times bogging the builder down).
// --------------------------------------------------------------------
function ImageEditorAutoApply()
{   
    if (!ImageEditor.loadingPreview && $ImageEditorLoaded)
    {
        ImageEditor.reCreateImage(ImageEditor.selectedImage);
        ImageEditor.loadPreview(ImageEditor.selectedImage); 
        ImageEditorSaveButton.update();
    }
}

// --------------------------------------------------------------------
// Image Editor Settings Auto Apply Commands
//
// These all get called when you edit any base setting Image Builder 
// field.  Most do nothing more than call the auto apply, though a 
// few will check the values and/or store them for builder usage.
// --------------------------------------------------------------------
function ImageEditorImageName()
{
    %name = ImageEditorTxtImageName.getValue();

    if (%name $= "")
        return;

    ImageEditorAutoApply();
}

function ImageEditorFilterMode()
{
    ImageEditorAutoApply();
}

function ImageEditorPreload()
{
    %value = ImageEditorCheckBoxPreload.getValue();

    if ($lastPreloadValue $= "" || %value != $lastPreloadValue)
    {
        $lastPreloadValue = %value;
        ImageEditorAutoApply();
    }
}

function ImageEditorImageMode()
{
    %imageMode = ImageEditorComImageMode.getValue();

    ImageEditorAutoApply();
}

// --------------------------------------------------------------------
// Image Editor Cell Settings Auto Apply Commands
//
// These all get called when you edit a cell specific Image Builder 
// field.  Most do nothing more than call the auto apply, though a 
// few will check the values and/or store them for builder usage.
// --------------------------------------------------------------------
function ImageEditorApplyCountX()
{
    if (ImageEditor.linkProperties == true)
    {
        ImageEditorCellWidth.setText(0);
        ImageEditorCellHeight.setText(0);
    }
    ImageEditor.CountExtentOverride = "";
    ImageEditorAutoApply();  
}

function ImageEditorApplyCountY()
{
    if (ImageEditor.linkProperties == true)
    {
        ImageEditorCellWidth.setText(0);
        ImageEditorCellHeight.setText(0);
    }
    ImageEditor.CountExtentOverride = "";
    ImageEditorAutoApply();
}

function ImageEditorApplyCellHeight()
{
    ImageEditor.CountExtentOverride = "Extent";
    ImageEditorAutoApply();
}

function ImageEditorApplyCellWidth()
{
    ImageEditor.CountExtentOverride = "Extent";
    ImageEditorAutoApply();
}

function ImageEditorApplyOffsetY()
{
    ImageEditorAutoApply();
}

function ImageEditorApplyOffsetX()
{
    ImageEditorAutoApply();
}

function ImageEditorApplyStrideX()
{
    ImageEditorAutoApply();
}

function ImageEditorApplyStrideY()
{
    ImageEditorAutoApply();
}

function ImageEditorCellRowOrder()
{
    ImageEditorAutoApply();
}

function ImageEditorCellCountXButtonDown::onClick(%this)
{
    ImageEditorCellCountX.setText(ImageEditorCellCountX.getText() - 1);
    
    // Update the gui
    ImageEditorApplyCountX();
}

function ImageEditorCellCountXButtonUp::onClick(%this)
{
    ImageEditorCellCountX.setText(ImageEditorCellCountX.getText() + 1);
    
    // Update the gui
    ImageEditorApplyCountX();
}

function ImageEditorCellCountYButtonDown::onClick(%this)
{
    ImageEditorCellCountY.setText(ImageEditorCellCountY.getText() - 1);
    
    // Update the gui
    ImageEditorApplyCountY();
}

function ImageEditorCellCountYButtonUp::onClick(%this)
{
    ImageEditorCellCountY.setText(ImageEditorCellCountY.getText() + 1);
    
    // Update the gui
    ImageEditorApplyCountY();
}

function ImageEditorCellWidthButtonDown::onClick(%this)
{
    ImageEditorCellWidth.setText(ImageEditorCellWidth.getText() - 1);
    
    // Update the gui
    ImageEditorApplyCellWidth();
}

function ImageEditorCellWidthButtonUp::onClick(%this)
{
    ImageEditorCellWidth.setText(ImageEditorCellWidth.getText() + 1);
    
    // Update the gui
    ImageEditorApplyCellWidth();
}

function ImageEditorCellHeightButtonDown::onClick(%this)
{
    ImageEditorCellHeight.setText(ImageEditorCellHeight.getText() - 1);
    
    // Update the gui
    ImageEditorApplyCellHeight();
}

function ImageEditorCellHeightButtonUp::onClick(%this)
{
    ImageEditorCellHeight.setText(ImageEditorCellHeight.getText() + 1);
    
    // Update the gui
    ImageEditorApplyCellHeight();
}

function ImageEditorAddTagButton::onClick(%this)
{
    ImageEditor.addTag(ImageEditorTagList.getText());
}

function ImageBuilderGui::removeTag(%this, %tag)
{
    ImageEditor.removeTag(%tag);
}

function ImageBuilderSourceFileMouseEventControl::onMouseEnter(%this)
{
    ImageEditorFileBrowserButton.NormalImageCache = ImageEditorFileBrowserButton.NormalImage;
    ImageEditorFileBrowserButton.setNormalImage(ImageEditorFileBrowserButton.HoverImage);
}

function ImageBuilderSourceFileMouseEventControl::onMouseLeave(%this)
{
    ImageEditorFileBrowserButton.setNormalImage(ImageEditorFileBrowserButton.NormalImageCache);
}

function ImageBuilderSourceFileMouseEventControl::onMouseDown(%this)
{
    ImageEditorFileBrowserButton.setNormalImage(ImageEditorFileBrowserButton.DownImage);
}

function ImageBuilderSourceFileMouseEventControl::onMouseUp(%this)
{
    ImageEditor.imageFileBrowser();
    ImageEditorFileBrowserButton.setNormalImage(ImageEditorFileBrowserButton.HoverImage);
}

function ImageEditorZoomButton::onClick(%this)
{
    Canvas.pushDialog(ImagePreviewGui);
    ImagePreviewGui.displayImage(ImageEditor.selectedImage, ImageEditor.selectedImage.getImageSize());  
}

function ImageBuilderImageLocation::onWake(%this)
{
    %this.initialize($AssetLocationTextEditMessageString);
}

function ImageEditorTxtImageName::onWake(%this)
{
    %this.initialize($AssetNameTextEditMessageString);
}

function ImageEditorLinkPropertiesCheckBox::onWake(%this)
{
    %this.setValue(1); 
    ImageEditor.linkProperties = %this.getValue();  
}

function ImageEditorLinkPropertiesCheckBox::onAction(%this)
{ 
    ImageEditor.linkProperties = %this.getValue();  
}

function ImageEditorTxtImageName::onValidate(%this)
{
    ImageEditorImageName();
}

function ImageEditorTxtImageName::onKeyPressed(%this)
{
    // Disallow symbols and spaces
    %name = stripChars(%this.getValue(), "-+*/%$&§=()[].?\"#,;!~<>|°^{} ");
    %this.setText(%name);
    
    %this.update();
    ImageEditorSaveButton.update(); 
} 

function ImageEditorSaveButton::update(%this)
{
    %active = true;    
    
    // Don't allow saving if the name is blank
    if (ImageEditorTxtImageName.isEmpty()) 
        %active = false;
        
    // Don't allow saving if the image location is blank
    if (ImageBuilderImageLocation.isEmpty()) 
        %active = false;
      
    %this.setActive(%active);
}