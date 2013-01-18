//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//--------------------------------
// Sound Tool Help
//--------------------------------
function SoundEditorHelpButton::onClick(%this)
{
   gotoWebPage("http://docs.3stepstudio.com/" @ $LoadedTemplate.Name @ "/soundimporter/");
}

// --------------------------------------------------------------------
$musicAudioType = 1;
$effectsAudioType = 2;
// --------------------------------------------------------------------

function launchSoundImporter( %soundProfile )
{
    SoundEditor.soundAssetID = "";    
    SoundEditor.previewSoundHandle = 0;

    // Is this a sound profile?
    %soundAsset = AssetDatabase.acquireAsset(%soundProfile);
    if ( !isObject(%soundAsset) )
    {
        // No, so create a new one.
        SoundEditor.isNewProfile = true;
        SoundEditor.soundAssetID = SoundEditor.createPrivateAsset("", "NewSound");
    }
    else
    {
        SoundEditor.isNewProfile = false;
        SoundEditor.soundAssetID = %soundProfile;
        SoundEditor.createProxyProfile(%soundAsset.AssetName);
    }

    SoundImporterSoundName.initialize($SIAssetNameTextEditMessageString);
    SoundImporterFilename.initialize($SIAssetLocationTextEditMessageString);

    if (SoundEditor.proxyProfile.name !$= "")
        SoundImporterSoundName.setText( SoundEditor.proxyProfile.name );

    // Update importer GUI.   
    SoundEditor.UpdateImporterGui();

    // Reset tag list.
    SoundImporterTagList.refresh(0);
    SoundImporterTagContainer.deleteContents(true);

    // Populdate tag container.   
    SoundImporterTagContainer.populateTagList(SoundEditor.soundAssetID);

    // Auto-apply Tag if we are Filtered 
    if (AssetLibrary.tagFilter !$= "Any")
        SoundEditor.applyTag(AssetLibrary.tagFilter);

    // Show importer.
    Canvas.pushDialog( SoundImporterGui );  
}

// --------------------------------------------------------------------

function cancelSoundImporter()
{
    // Stop any preview sound.
    Si_SoundStopButton.onClick();

    if (SoundEditor.isNewProfile)
        cleanTempAssets();

    // Delete proxy sound profile.
    if ( isObject(SoundEditor.proxyProfile) )
        SoundEditor.proxyProfile.delete();   

    // Remove GUI.
    Canvas.popDialog( SoundImporterGui );
}

// --------------------------------------------------------------------

function SoundImporterGui::onSleep(%this)
{
    Si_SoundStopButton.onClick();
}

function SoundEditor::createProxyProfile(%this, %proxyName)
{
    %soundAsset = AssetDatabase.acquireAsset(%this.soundAssetID);
    %proxyProfile = new ScriptObject()
    {   
        name = %proxyName;
        filename = %soundAsset.AudioFile;
        Looping = %soundAsset.Looping;
        volume = (%soundAsset.Volume > 0 ? %soundAsset.Volume : 1);
        volumeChannel = (%soundAsset.VolumeChannel > 0 ? %soundAsset.VolumeChannel : $effectsAudioType);
        streaming = %soundAsset.Streaming;
    };
    AssetDatabase.releaseAsset(%this.soundAssetID);
    %this.proxyProfile = %proxyProfile;
}

function SoundEditor::addTag(%this, %tag)
{
    if (%tag $= "")
        return;

    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagManifest.getAssetTagCount(%this.soundAssetID);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %tempTag = %assetTagManifest.getAssetTag(%this.soundAssetID, %i);
        if (%tag $= %tempTag)
        {
            echo(" @@@ asset already has tag " @ %tag);
            return;
        }
    }
    %assetTagManifest.tag(%this.soundAssetID, %tag);
    SoundImporterTagContainer.addTagItem(%tag);
    SoundImporterTagContainer.populateTagList(%this.soundAssetID);
}

function SoundEditor::copyTags(%this, %sourceAsset, %targetAsset)
{
    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagManifest.getAssetTagCount(%sourceAsset);
    %targetTags = %assetTagManifest.getAssetTagCount(%targetAsset);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %tag = %assetTagManifest.getAssetTag(%sourceAsset, %i);
        for (%j = 0; %j <= %targetTags; %j++)
        {
            if (%targetTags > 0)
            {
                %tempTargetTag = %assetTagManifest.getAssetTag(%targetAsset, %j);
                if (%tempTargetTag $= %tag)
                {
                    echo(" @@@ asset already has tag " @ %tag);
                    continue;
                }
            }
            else
                %assetTagManifest.tag(%targetAsset, %tag);
        }
    }
}

function SoundEditor::clearTags(%this, %assetID)
{
    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagCount = %assetTagManifest.getAssetTagCount(%assetID);
    for (%i = 0; %i < %assetTagCount; %i++)
    {
        %tag = %assetTagManifest.getAssetTag(%sourceAsset, %i);
        %assetTagManifest.untag(%assetID, %tag);
    }
}

function SoundEditor::applyTag(%this, %tag)
{
}

function SoundEditor::removeTag(%this, %tag)
{
    %assetTagManifest = AssetDatabase.getAssetTags();
    %assetTagManifest.untag(%this.soundAssetID, %tag);
    SoundImporterTagContainer.removeTagItem(%tag);
    %this.UpdateImporterGui();
}

function SoundEditor::deleteTagFromList(%this, %tag)
{
}

// --------------------------------------------------------------------

function SoundEditor::SoundImporterValidateSoundName( %this )
{  
    %oldName = SoundImporterSoundName.getText();

    // User typed something, but ended up using the original name
    if(%oldName $= %this.proxyProfile.name)
        return;

    if (%oldname $= "")
        %newName = %this.getValidSoundName("NewSound");
    else
        %newName = %this.getValidSoundName(%oldName);

    // Set name.
    %this.proxyProfile.name = %newName;

    // Update name.
    if (%this.proxyProfile.name !$= "")
        SoundImporterSoundName.setText( %this.proxyProfile.name );
}

// --------------------------------------------------------------------

function SoundEditor::UpdateImporterGui( %this )
{
    SoundImporterTagContainer.populateTagList(%this.soundAssetID);
    // Fetch proxy profile.
    %proxyProfile = %this.proxyProfile;

    %proxyAsset = AssetDatabase.acquireAsset(%this.soundAssetID);
    
    if (%proxyAsset !$= "")
    {
        // Update filename.
        if (%proxyAsset.AudioFile !$= "")
            SoundImporterFilename.setText( %proxyAsset.audioFile );

        // Update asset name.
        if (%proxyProfile.name !$= "")
            SoundImporterSoundName.setText( %proxyProfile.name );

        // Update "looping".
        SoundImporterLooping.setValue( %proxyAsset.Looping );

        // Update "music".
        %isMusic = (%proxyAsset.VolumeChannel == $musicAudioType);
        SoundImporterChannel.setValue(%isMusic);
    }
    
    AssetDatabase.releaseAsset(%this.soundAssetID);
    
    Si_SaveButton.update(); 
}

// --------------------------------------------------------------------

function SoundEditor::soundFileBrowser( %this )
{
    // Fetch proxy profile.
    %proxyProfile = %this.proxyProfile;

    // Create file dialog.
    %fileDialog = new OpenFileDialog()
    {
        Filters = $T2D::SoundSpec;
        DefaultFile = "";
        ChangePath = false;
        MustExist = true;
        MultipleFiles = false;
    };

    // Update filename.        
    if( %fileDialog.Execute() )
    {
        %fileName = %fileDialog.FileName;

        %proxyProfile.filename = %fileName;

        %proxyAsset = AssetDatabase.acquireAsset(%this.soundAssetID);
        %proxyAsset.AudioFile = %proxyProfile.filename;
        AssetDatabase.releaseAsset(%this.soundAssetID);
    }
    else
    {
        if (%proxyProfile.filename !$= "")
            SoundImporterFilename.initialize($SIAssetLocationTextEditMessageString);
        else
            SoundImporterFilename.setText(%proxyProfile.filename);
    }
    %fileDialog.delete();

    // Update importer GUI.   
    %this.UpdateImporterGui();
}

// --------------------------------------------------------------------

function SoundEditor::createPrivateAsset(%this, %fileName, %assetName)
{
    %privateAsset = new AudioAsset()
    {
        AudioFile = %fileName;
        Looping = false;
        Volume = 1.0;
        VolumeChannel = $effectsAudioType;
        Streaming = false;
    };
    
    %this.createProxyProfile(%this.getValidSoundName("NewSound"));
    %newAssetID = AssetDatabase.addPrivateAsset(%privateAsset);
    return %newAssetID;
}

// --------------------------------------------------------------------

function SoundEditor::createSoundAsset(%this, %name)
{
    %fileName = expandPath("^{UserAssets}/audio/" @ fileName(%soundAssetID.AudioFile));

    %proxyAsset = AssetDatabase.acquireAsset(%this.soundAssetID);

    %soundAsset = new AudioAsset();
    %soundAsset.AssetName = %name;
    %soundAsset.AudioFile = %fileName;
    %soundAsset.Looping = %proxyAsset.Looping;
    %soundAsset.Volume = %proxyAsset.Volume;
    %soundAsset.VolumeChannel = %proxyAsset.VolumeChannel;
    %soundAsset.Streaming = %proxyAsset.Streaming;

    AssetDatabase.releaseAsset(%this.soundAssetID);

    TamlWrite(%soundAsset, expandPath("^{UserAssets}/audio/" @ %soundAsset.AssetName @ ".asset.taml"));

    %userAssetModule = ModuleDatabase.findModule("{UserAssets}", 1);
    AssetDatabase.addSingleDeclaredAsset(%userAssetModule, expandPath("^{UserAssets}/audio/" @ %soundAsset.AssetName @ ".asset.taml"));
    %newAssetID = "{UserAssets}:" @ %soundAsset.AssetName;

    return %newAssetID;
}

// --------------------------------------------------------------------

function SoundEditor::getValidSoundName(%this, %nameBase)
{
    %number = 0;
    %tempName = %nameBase;
    %query = new AssetQuery();
    AssetDatabase.findAssetName(%query, %tempName);
    while ( %query.getCount() > 0 )
    {
        %tempName = %nameBase @ %number++;
        AssetDatabase.findAssetName(%query, %tempName);
    }
    return %tempName;
}

// --------------------------------------------------------------------

function SoundEditor::updateLooping( %this )
{
    %this.SoundImporterValidateSoundName();

    // Fetch proxy profile.
    %proxyProfile = %this.proxyProfile;

    // Update profile description.
    %proxyProfile.Looping = SoundImporterLooping.getValue();

    %proxyAsset = AssetDatabase.acquireAsset(%this.soundAssetID);
    %proxyAsset.Looping = %proxyProfile.Looping;
    AssetDatabase.releaseAsset(%this.soundAssetID);

    // Update importer GUI.   
    %this.UpdateImporterGui();   
}

function SoundEditor::updateChannel( %this )
{
    %this.SoundImporterValidateSoundName();

    // Fetch proxy profile.
    %proxyProfile = %this.proxyProfile;

    // Update profile description.
    %isMusic = SoundImporterChannel.getValue();
    if (%isMusic)
        %proxyProfile.VolumeChannel = $musicAudioType;
    else
        %proxyProfile.VolumeChannel = $effectsAudioType;

    %proxyAsset = AssetDatabase.acquireAsset(%this.soundAssetID);
    %proxyAsset.VolumeChannel = %proxyProfile.VolumeChannel;
    AssetDatabase.releaseAsset(%this.soundAssetID);

    // Update importer GUI.
    %this.UpdateImporterGUI();
}

// --------------------------------------------------------------------

function SoundEditor::updateProxyProfile( %this )
{
    if (!%this.proxyProfile)
        %this.proxyProfile = new AudioAsset();
    if (%this.soundAssetID !$= "")
    {
        %temp = AssetDatabase.acquireAsset(%this.soundAssetID);
        %this.proxyProfile.name = %temp.AssetName;
        %this.proxyProfile.VolumeChannel = %temp.VolumeChannel;
        %this.proxyProfile.Looping = %temp.Looping;
        %this.proxyProfile.Volume = %temp.Volume;
        %this.proxyProfile.Streaming = %temp.Streaming;
        %this.proxyProfile.filename = %temp.AudioFile;
        AssetDatabase.releaseAsset(%this.soundAssetID);
    }
    else
    {
        %this.proxyProfile.name = SoundImporterSoundName.getText();
        %this.proxyProfile.filename = collapsePath(SoundImporterFilename.getText());
    }
    %this.updateChannel();
    %this.updateLooping();
}

function SoundEditor::updateSoundAsset( %this )
{
    %previewProfile = AssetDatabase.acquireAsset(%this.soundAssetID);
    %proxyProfile = %this.proxyProfile;

    // Configure preview profile.
    if (%previewProfile.AssetName !$= %proxyProfile.name)
    {
        %newID = strreplace(%this.soundAssetID, %previewProfile.AssetName, %proxyProfile.name);
        AssetDatabase.renameDeclaredAsset(%this.soundAssetID, %newID);
    }
    %previewProfile.AssetName = %proxyProfile.name;
    %previewProfile.AudioFile = %proxyProfile.filename;
    %previewProfile.Looping = %proxyProfile.Looping;
    %previewProfile.VolumeChannel = %proxyProfile.VolumeChannel;
    %previewProfile.Volume = %proxyProfile.volume;
    %previewProfile.Streaming = %proxyProfile.streaming;
    AssetDatabase.releaseAsset(%this.soundAssetID);
}

// --------------------------------------------------------------------

function SoundEditor::saveSounds( %this )
{   
    // Fetch proxy / profile.
    %proxyProfile = %this.proxyProfile;
    %tempAssetName = %this.proxyProfile.AssetName;
    
    // Stop any preview sound.
    Si_SoundStopButton.onClick();

    %proxyProfile.name = SoundImporterSoundName.getText();

    // Calculate target filename.
    %targetFilename = expandPath("^{UserAssets}/audio/" @ fileName(%proxyProfile.filename) );

    // Is the file in the correct target location?
    if ( %targetFilename !$= %proxyProfile.filename )
    {
        // No, so copy.
        if ( !pathCopy( %proxyProfile.filename, %targetFilename, true ) )
        {
            warn( " @@@ Target file '" @ %targetFilename @ " already exists.  File not copied." );
        }

        // Update sound profile filename.
        %proxyProfile.AudioFile = SoundImporterStripExtension( %targetFilename );
    }

    // Update the profile.
    if ( %this.isNewProfile )
    {
        %soundAsset = %this.createSoundAsset(%proxyProfile.name);

        %this.copyTags(%this.soundAssetID, %soundAsset);
        %this.clearTags(%this.soundAssetID);

        AssetDatabase.removeSingleDeclaredAsset(%this.soundAssetID);
    }
    else
    {
        %this.updateSoundAsset();
        %this.soundAssetID = "";
    }
    AssetDatabase.saveAssetTags();

    // Delete proxy sound profile.
    if ( isObject(SoundEditor.proxyProfile) )
        SoundEditor.proxyProfile.delete();

    // Remove GUI.   
    Canvas.popDialog( SoundImporterGui );

    AssetLibrary.schedule(100, "updateGui");
}

function cleanTempAssets(%tempAssetName)
{
    AssetDatabase.removeSingleDeclaredAsset( SoundEditor.soundAssetID );
}

// --------------------------------------------------------------------
function Si_ChangeFileClick::onMouseEnter(%this)
{
    SoundImporterFileBrowse.setNormalImage(ABChangeImageButton.HoverImage);
}

function Si_ChangeFileClick::onMouseLeave(%this)
{
    SoundImporterFileBrowse.setNormalImage(ABChangeImageButton.NormalImageCache);
}

function Si_ChangeFileClick::onMouseDown(%this)
{
    SoundImporterFileBrowse.setNormalImage(ABChangeImageButton.DownImage);
}

function Si_ChangeFileClick::onMouseUp(%this)
{
    SoundEditor.soundFileBrowser();
    SoundImporterFileBrowse.setNormalImage(ABChangeImageButton.HoverImage);
}


function SoundImporterAddTagButton::onClick(%this)
{
    SoundEditor.addTag(SoundImporterTagList.getText());
}

function Si_SoundStopButton::onClick( %this )
{
    // Stop sound if already playing.
    if ( alxIsPlaying( SoundEditor.previewSoundHandle ) )
        alxStop( SoundEditor.previewSoundHandle );

    if (SoundEditor.playPreviewSchedule)
        cancel(SoundEditor.playPreviewSchedule);
   
    SoundEditor.previewSoundHandle = 0;
    SoundEditor.playPreviewSchedule = "";
    Si_SoundIconDisplay.stop();

    Si_SoundIconDisplay.setImage("{EditorAssets}:speaker_OffImage");
    Si_SoundPlayButton.NormalImage = "{EditorAssets}:smallPlayImage";
    Si_SoundPlayButton.HoverImage = "{EditorAssets}:smallPlay_hImage";
    Si_SoundPlayButton.DownImage = "{EditorAssets}:smallPlay_dImage";
    Si_SoundPlayButton.InactiveImage = "{EditorAssets}:smallPlay_iImage";
}

function Si_SoundPlayButton::onClick( %this )
{
    // pause sound if already playing.
    if ( alxIsPlaying( SoundEditor.previewSoundHandle ) )
    {
        alxPause( SoundEditor.previewSoundHandle );
        Si_SoundPlayButton.NormalImage = "{EditorAssets}:smallPlayImage";
        Si_SoundPlayButton.HoverImage = "{EditorAssets}:smallPlay_hImage";
        Si_SoundPlayButton.DownImage = "{EditorAssets}:smallPlay_dImage";
        Si_SoundPlayButton.InactiveImage = "{EditorAssets}:smallPlay_iImage";
        return;
    }

    // Fetch proxy profile.
    %proxyProfile = SoundEditor.proxyProfile;
    if (!isObject(%proxyProfile) || %proxyProfile.filename $= "")
        return;

    // set the "playing" image first to give visual feedback that we are trying
    // to perform the requested action.  This is especially important with larger
    // audio files because the copy-then-load proceedure can be long.
    Si_SoundIconDisplay.update();
    Si_SoundPlayButton.NormalImage = "{EditorAssets}:smallPauseImage";
    Si_SoundPlayButton.HoverImage = "{EditorAssets}:smallPause_hImage";
    Si_SoundPlayButton.DownImage = "{EditorAssets}:smallPause_dImage";
    Si_SoundPlayButton.InactiveImage = "{EditorAssets}:smallPause_iImage";

    // Create preview profile.
    SoundEditor.updateSoundAsset();
    %sound = AssetDatabase.acquireAsset(SoundEditor.soundAssetID);
    if (!%sound.Looping)
    {
        %time = alxGetAudioLength(SoundEditor.soundAssetID) - 32;
        SoundEditor.playPreviewSchedule = Si_SoundStopButton.schedule(%time, onClick);
    }
    AssetDatabase.releaseAsset(SoundEditor.soundAssetID);
    
    // Play the preview sound.
    if ( SoundEditor.previewSoundHandle != 0 )
        alxUnPause( SoundEditor.previewSoundHandle );
    else if ( SoundEditor.soundAssetID !$= "" && SoundEditor.previewSoundHandle == 0 )
        SoundEditor.previewSoundHandle = alxPlay( SoundEditor.soundAssetID );
}

function SoundImporterSoundName::onKeyPressed(%this)
{
    // Disallow symbols and spaces
    %name = stripChars(%this.getValue(), "-+*/%$&§=()[].?\"#,;!~<>|°^{} ");
    %this.setText(%name);
    
    %this.update();
    Si_SaveButton.update(); 
} 

function Si_SaveButton::update(%this)
{
    %active = true;    
    
    // Don't allow saving if the name is blank
    if (SoundImporterSoundName.isEmpty()) 
        %active = false;
        
    // Don't allow saving if the image location is blank
    if (SoundImporterFilename.isEmpty()) 
        %active = false;
      
    %this.setActive(%active);
}

function Si_SoundIconDisplay::update(%this)
{
    if (SoundEditor.proxyProfile.Looping)
    {
        if (%this.state > 3)
            %this.state = 1;
        if (%this.state < 1)
            %this.state = 1;
        %this.setImage("{EditorAssets}:speaker_On0" @ %this.state @ "Image");
        %this.state++;
        %this.updateSchedule = %this.schedule(750, update);
    }
    else
    {
        if (%this.Image $= "{EditorAssets}:speaker_On03Image")
            %this.setImage("{EditorAssets}:speaker_OffImage");
        else
            %this.setImage("{EditorAssets}:speaker_On03Image");
    }
}

function Si_SoundIconDisplay::stop(%this)
{
    %this.state = 1;
    cancel(%this.updateSchedule);
}