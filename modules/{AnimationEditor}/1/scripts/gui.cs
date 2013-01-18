//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------



/// void(AnimationBuilderGui this, GuiControl control, Point2F position)
/// Catch all in case there's something that catches drag and drops beneath us.
/// @param this The gui.
/// @param control The control that was dropped.
/// @param position The position the control was dropped at.
function AnimationBuilderGui::onControlDropped( %this, %control, %position)
{
    if ( ABStoryboardWindow.pointInControl( %position.x, %position.y ) )
        ABStoryboardWindow.onControlDropped( %control, %position );
    if ( %control.spriteClass $= "ABStoryboardPreviewSprite" )
        AnimationBuilder.removeFrame( %control.frameNumber );
}

function AnimationBuilderGui::setSelectedAsset(%this, %assetID)
{
   AnimationBuilder.newAnimation(%assetID, true);

   if ($AssetAutoTag !$= "")
   {
      AnimationBuilder.applyTag($AssetAutoTag);
      
      $AssetAutoTag = "";
   }
}

function AnimationBuilderGui::removeTag(%this, %tag)
{
    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagManifest.untag(AnimationBuilder.animationId, %tag);
    ABTagContainer.removeTagItem(%tag);
    AnimationBuilder.updateGui();
}

function AnimationBuilderGui::onWake(%this)
{
    AnimationBuilder.updateGui();
}

function ABStoryboardWindow::onControlDropped(%this, %control, %position)
{
    %dropPosition = Vector2Sub(%position, %this.getGlobalPosition());
    %worldX = getWord(%dropPosition, 0);
    %sizeX = %this.colSize + %this.colSpacing;

    %index = mFloor((%worldX / %sizeX) + 0.5);
    AnimationBuilder.insertFrame( %control.Frame, %index );

    if ( %control.spriteClass $= "ABStoryboardPreviewSprite" )
        AnimationBuilder.removeFrame( %control.frameNumber );
}

function ABAddTagButton::onClick(%this)
{
    AnimationBuilder.addTag(ABTagList.getText());
}

function ABSpeedDownBtn::onClick(%this)
{
    %speed = ABFPSField.getText();
    %frames = mCeil(%speed);
    %frames--;
    ABFPSField.setText(%frames);
    AnimationBuilder.validateFPS();
}

function ABSpeedUpBtn::onClick(%this)
{
    %speed = ABFPSField.getText();
    %frames = mCeil(%speed);
    %frames++;
    ABFPSField.setText(%frames);
    AnimationBuilder.validateFPS();
}

function ABChangeImageClick::onMouseEnter(%this)
{
    ABChangeImageButton.setNormalImage(ABChangeImageButton.HoverImage);
}

function ABChangeImageClick::onMouseLeave(%this)
{
    ABChangeImageButton.setNormalImage(ABChangeImageButton.NormalImageCache);
}

function ABChangeImageClick::onMouseDown(%this)
{
    ABChangeImageButton.setNormalImage(ABChangeImageButton.DownImage);
}

function ABChangeImageClick::onMouseUp(%this)
{
    ABChangeImageButton.setNormalImage(ABChangeImageButton.HoverImage);
    ABChangeImageButton.onClick();
}

function ABChangeImageButton::onClick(%this)
{
    AnimationBuilder.changeImage();
}

function AnimBuilderHelpButton::onClick(%this)
{
    gotoWebPage("http://docs.3stepstudio.com/" @ $LoadedTemplate.Name @ "/animation/");
}

function ABNameField::onWake(%this)
{
    %this.initialize($ABAssetNameTextEditMessageString);
}

function ABImageField::onWake(%this)
{
    %this.initialize($ABAssetLocationTextEditMessageString);
}

function ABNameField::onValidate(%this)
{
    AnimationBuilder.validateName();
}

function ABNameField::onKeyPressed(%this)
{
    // Disallow symbols and spaces
    %this.animName = stripChars(%this.getValue(), "-+*/%$&§=()[].?\"#,;!~<>|°^{} ");
    %this.setText(%this.animName);
    
    %this.update();
    ABSaveButton.update(); 
} 

function ABSaveButton::update(%this)
{
    %active = true;    
    
    // Don't allow saving if the name is blank
    if (ABNameField.isEmpty()) 
        %active = false;
        
    // Don't allow saving if the image location is blank
    if (ABImageField.isEmpty()) 
        %active = false;
      
    %this.setActive(%active);
}
