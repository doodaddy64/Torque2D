//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------



$PhysicsLauncherTools::CameraWidthInMeters = 12; 
$PhysicsLauncherTools::WorldObjectAndProjectileSceneLayer = 15;
$PhysicsLauncherTools::LauncherCollisionObjectSceneLayer = 16;


function PhysicsLauncherTools::writePrefabs()
{
    // Write out templateObjectSet to taml
    %prefabFile = expandPath("^PhysicsLauncherTemplate/managed/prefabs.taml");

    if (isFile(%prefabFile))
        TamlWrite($PrefabSet, %prefabFile);
}

function PhysicsLauncherTools::deleteSceneContents(%scene)
{
    // Iterate.
    while (%scene.getCount() > 0)
    {
        %object = %scene.getObject(0);
        
        // Handle deleting the launcher group
        if (%object.getClassName() $= "SceneObjectGroup")
        {
            %object.deleteContents();
            %scene.remove(%object);
        }

        %object.delete();
    }
}

function PhysicsLauncherTools::purgePrefabFromAllLevels(%prefabName)
{
    %path = expandPath("^PhysicsLauncherTemplate/data/levels"); 
    %fileSpec = "/*.scene.taml";   
    %pattern = %path @ %fileSpec;
    
    %file = findFirstFile(%pattern);

    while(%file !$= "")
    {
        PhysicsLauncherTools::purgePrefab(%file, %prefabName);

        %file = findNextFile(%pattern);
    }
}

function PhysicsLauncherTools::purgePrefab(%levelFile, %prefabName)
{
    // Read level file
    %scene = TamlRead(%levelFile);
    
    //for (%i = 0; %i < %scene.getSceneObjectCount(); %i++)
    for (%i = %scene.getSceneObjectCount() - 1; %i >= 0; %i--)
    {
        %obj = %scene.getSceneObject(%i);
        
        if (%obj.getPrefab() $= %prefabName)
            %obj.delete();
    }
    
    // Write level file
    TamlWrite(%scene, %levelFile);
    
    // Remove the level scene and its contents from memory
    PhysicsLauncherTools::deleteSceneContents(%scene);
    %scene.delete();
}

/// <summary>
/// Sets the mass of the object to the specified value
/// by setting the density of all the collision shapes
/// on the object.
/// </summary>
/// <param name="mass">Float mass to set on the object.</param>
function PhysicsLauncherTools::setObjectMass(%object, %mass)
{
    %totalArea = 0;
    
    // Sum up the areas for all collision shapes
    for (%i = 0; %i < %object.getCollisionShapeCount(); %i++)
    {
        %totalArea += %object.getCollisionShapeArea(%i);
    }
    
    // Calculate the density
    %density = %mass / %totalArea;

    // Set the density
    %object.setDefaultDensity(%density);    

    // Set the density on each collision shape
    for (%i = 0; %i < %object.getCollisionShapeCount(); %i++)
    {
        %object.setCollisionShapeDensity(%i, %density);
    }
}

/// <summary>
/// Gets the mass of the object.
/// </summary>
/// <return>The mass of the object.</return>
function PhysicsLauncherTools::getObjectMass(%object)
{
    %totalArea = 0;
    
    // Sum up the areas for all collision shapes
    for (%i = 0; %i < %object.getCollisionShapeCount(); %i++)
    {
        %totalArea += %object.getCollisionShapeArea(%i);
    }
    
    // Calculate the density
    %density = %object.getDefaultDensity();
    %mass = %density * %totalArea;
    return %mass;
}

function PhysicsLauncherTools::createObjectFromTemplate(%template, %className, %tool, %type)
{
    eval("%obj = new " @ %className @ "();");
    %obj.setPrefab(%template.template.getName());
    %obj.setInternalName(%template.getInternalName());
    
    %tool.setFieldValue(%type, %obj);
}

function PhysicsLauncherTools::copyCollisionShapes(%sourceObject, %modifiedObject)
{
    %sourceObject.copyAllCollisionShapes( %modifiedObject );
}

function PhysicsLauncherTools::scaleCollisionShapes(%object, %scaleX, %scaleY)
{
    // Copy all the collision shapes.
    %objectTarget = new SceneObject();
    
    // Fetch the collision shape count.
    %count = %object.getCollisionShapeCount(); 
         
    // Set scaled collision shapes
    for (%i = 0; %i < %count; %i++)
    {
        // Scale the shape format string
        %shapeType = %object.getCollisionShapeType( %i );
        switch$(%shapeType) 
        {
            case "circle":
                PhysicsLauncherTools::scaleCircleCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY);
            
            case "polygon":
                PhysicsLauncherTools::scalePolygonCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY);
            
            //case "edge":
                PhysicsLauncherTools::scaleEdgeCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY);
            //
            //case "chain":  
                PhysicsLauncherTools::scaleChainCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY);
            
            default: 
                warn ("PhysicsLauncherTools::scaleCollisionShapes -- invalid shape type: " @ %shapeType);
                return;
        }       
    } 
    
    // Copy the new scale objects.
    %objectCopy.copyAllCollisionShapes( %object );
    
    // Remove the temporary copy.
    %objectCopy.delete();
}

function PhysicsLauncherTools::scaleCircleCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY)
{
    if (getWordCount(%formatString) != 8) 
    {
        warn ("PhysicsLauncherTools::scaleCircleCollisionShapeString -- invalid format string.");
        return %formatString;   
    }
    
    %scaledRadius = %object.getCircleCollisionShapeRadius(%i) * mGetMin(%scaleX, %scaleY);

    %localPosition = %object.getCircleCollisionShapeLocalPosition(%i);
    %scaledLocalPositionX = getWord(%localPosition, 0) * %scaleX;
    %scaledLocalPositionY = getWord(%localPosition, 1) * %scaleY;
    
    return %objectTarget.createCircleCollisionShape( %scaledRadius, %scaledLocalPositionX, %scaledLocalPositionY );
}

function PhysicsLauncherTools::scalePolygonCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY)
{
    %localPointCount = %object.getPolygonCollisionShapePointCount(%i);
    for (%i = 0; %i < %localPointCount; %i++)
    {
        %localPoint = %object.getPolygonCollisionShapeLocalPoint(%i);
        %scaledLocalPointX = getWord(%localPoint, 0) * %scaleX;
        %scaledLocalPointY = getWord(%localPoint, 1) * %scaleY;
                
        %localPoints = %localPoints SPC %scaledLocalPointX SPC %scaledLocalPointY;
    }
    
    return %objectTarget.createPolygonCollisionShape( %localPoints );
}

function PhysicsLauncherTools::scaleEdgeCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY)
{
    //TODO
}

function PhysicsLauncherTools::scaleChainCollisionShapeString(%object, %objectTarget, %i, %scaleX, %scaleY)
{
    //TODO
}

function PhysicsLauncherTools::setGuiControlAspectRatio(%guiControl, %aspectRatio)
{
    %position = %guiControl.getPosition(); 
    %extent = %guiControl.getExtent();   
    
    if (%aspectRatio > 1) //landscape
    {
        %newExtentX = %extent.x;
        %newExtentY = %extent.y / %aspectRatio;
        
        %newPositionX = %position.x;
        %newPositionY = %position.y + ((%extent.y - %newExtentY) /2 );
    }
    else if (%aspectRatio < 1) //portrait
    {
        %newExtentX = %extent.x * %aspectRatio;
        %newExtentY = %extent.y;
        
        %newPositionX = %position.x + ((%extent.x - %newExtentX) / 2 );
        %newPositionY = %position.y;
    }
    else
    {
        return;   
    }
    
    %guiControl.setPosition(%newPositionX, %newPositionY);
    %guiControl.setExtent(%newExtentX, %newExtentY);
}

function PhysicsLauncherTools::validateIntField(%field, %min, %max)
{
    %fieldValue = %field.getText();    
    
    if (%fieldValue > %max)
        %fieldValue = %max;
    if (%fieldValue < %min)
        %fieldValue = %min;
        
    // Check to make sure it is a number
    if (!(%fieldValue < %max) && !(%fieldValue > %min))
        %fieldValue = 0;
        
    %field.setText(%fieldValue);
}

function PhysicsLauncherTools::getBehaviorFromInstanceName(%object, %instanceName)
{
    for (%i = 0; %i < %object.getBehaviorCount(); %i++)
    {
        %behavior = %object.getBehaviorByIndex(%i);
        if ((%behavior.instanceName $= %instanceName))
            return %behavior;
    }
    
    return "";
}

function PhysicsLauncherTools::getBehaviorFromConnection(%object, %outputBehaviorTemplateName, %inputBehaviorTemplateName, %outputName, %inputName)
{
    // Get output behavior
    %outputBehavior = "";
    for (%i = 0; %i < %object.getBehaviorCount(); %i++)
    {
        %b = %object.getBehaviorByIndex(%i);
        if ((%b.template.getName() $= %outputBehaviorTemplateName))
        {
            %outputBehavior = %b;
            break;   
        }
    }
    
    if (%outputBehavior $= "")
    {
        warn("PhysicsLauncherTools::getBehaviorFromConnection -- unable to find outputBehavior");
        return;   
    }    

    %inputBehavior = "";  
    for (%i = 0; %i < %object.getBehaviorConnectionCount(%outputBehavior, %outputName); %i++)
    {
        %behaviorConnectionString = %object.getBehaviorConnection(%outputBehavior, %outputName, %i);
        
        %behaviorConnectionString = strreplace(%behaviorConnectionString, ",", " ");        
        
        %behaviorConnectionInputBehavior = getWord(%behaviorConnectionString, 1);
        %behaviorConnectionInputName = getWord(%behaviorConnectionString, 3);
        
        if ((%behaviorConnectionInputName $= %inputName) && (%behaviorConnectionInputBehavior.template.getName() $= %inputBehaviorTemplateName))
        {
            %inputBehavior = getWord(%behaviorConnectionString, 1);
            break;
        }
    }
    
    return %inputBehavior;
}

function PhysicsLauncherTools::onFieldUpButton(%field)
{
    %value = %field.getText();
    %value += 1;
    %field.setText(%value);
    
    %field.onValidate();
}

function PhysicsLauncherTools::onFieldDownButton(%field)
{
    %value = %field.getText();
    %value -= 1;
    %field.setText(%value);
    
    %field.onValidate();
}

//
// Sound button utility functions
//

function PhysicsLauncherTools::audioButtonInitialize(%button)
{
    %button.soundHandle = "";
    %button.soundScheduleHandle = "";   
    
    %button.NormalImage = "{EditorAssets}:smallPlayImageMap";
    %button.HoverImage = "{EditorAssets}:smallPlay_hImageMap";
    %button.DownImage = "{EditorAssets}:smallPlay_dImageMap";
    %button.InactiveImage = "{EditorAssets}:smallPlay_iImageMap";
}

function PhysicsLauncherTools::audioButtonClicked(%button, %soundAsset)
{
    if (alxIsPlaying(%button.soundHandle))
    {
        PhysicsLauncherTools::audioButtonStop(%button, %soundAsset);
    }
    else
    {
        PhysicsLauncherTools::audioButtonStart(%button, %soundAsset);
    }
}

function PhysicsLauncherTools::audioButtonStop(%button, %soundAsset)
{
    // Cancel any pending schedule
    if(isEventPending(%button.soundScheduleHandle))
        cancel(%button.soundScheduleHandle); 
    
    alxStop(%button.soundHandle);
    %button.setNormalImage("{EditorAssets}:smallPlayImageMap");
    %button.setHoverImage("{EditorAssets}:smallPlay_hImageMap");
    %button.setDownImage("{EditorAssets}:smallPlay_dImageMap");
    %button.setInactiveImage("{EditorAssets}:smallPlay_iImageMap");
}

function PhysicsLauncherTools::audioButtonStart(%button, %soundAsset)
{
    if (AssetDatabase.isDeclaredAsset(%soundAsset))
    {    
        %button.soundHandle = alxPlay(%soundAsset);
        %button.setNormalImage("{EditorAssets}:smallStopImageMap");
        %button.setHoverImage("{EditorAssets}:smallStop_hImageMap");
        %button.setDownImage("{EditorAssets}:smallStop_dImageMap");
        %button.setInactiveImage("{EditorAssets}:smallStop_iImageMap");
        
        // Schedule the stop
        %time = alxGetAudioLength(%soundAsset);
        %button.soundScheduleHandle = schedule(%time, %button, "PhysicsLauncherTools::audioButtonStop", %button, %soundAsset);
    }
    
    // Handle stoping other sound play buttons that were pressed before this one
    if (isObject($PhysicsLauncherTools::LastSoundButton) && $PhysicsLauncherTools::LastSoundButton !$= %button)
    {
        if (alxIsPlaying($PhysicsLauncherTools::LastSoundButton.soundHandle))
            PhysicsLauncherTools::audioButtonStop($PhysicsLauncherTools::LastSoundButton, "");
    }
    
    $PhysicsLauncherTools::LastSoundButton = %button;
}

function PhysicsLauncherTools::audioButtonPairClicked(%button, %soundAsset)
{
    if (alxIsPlaying(%button.soundHandle))
    {
        PhysicsLauncherTools::audioButtonPairPause(%button);
    }
    else
    {
        PhysicsLauncherTools::audioButtonPairStart(%button, %soundAsset);
    }
}

function PhysicsLauncherTools::audioButtonPairStop(%button)
{
    // Cancel any pending schedule
    if(isEventPending(%button.soundScheduleHandle))
        cancel(%button.soundScheduleHandle); 
    
    alxStop(%button.soundHandle);
    %button.setNormalImage("{EditorAssets}:smallPlayImageMap");
    %button.setHoverImage("{EditorAssets}:smallPlay_hImageMap");
    %button.setDownImage("{EditorAssets}:smallPlay_dImageMap");
    %button.setInactiveImage("{EditorAssets}:smallPlay_iImageMap");
}

function PhysicsLauncherTools::audioButtonPairPause(%button)
{
    alxPause(%button.soundHandle);
    %button.setNormalImage("{EditorAssets}:smallPlayImageMap");
    %button.setHoverImage("{EditorAssets}:smallPlay_hImageMap");
    %button.setDownImage("{EditorAssets}:smallPlay_dImageMap");
    %button.setInactiveImage("{EditorAssets}:smallPlay_iImageMap");
}

function PhysicsLauncherTools::audioButtonPairStart(%button, %soundAsset)
{
    if (AssetDatabase.isDeclaredAsset(%soundAsset))
    {    
        %button.soundHandle = alxPlay(%soundAsset);
        %button.setNormalImage("{EditorAssets}:smallPauseImageMap");
        %button.setHoverImage("{EditorAssets}:smallPause_hImageMap");
        %button.setDownImage("{EditorAssets}:smallPause_dImageMap");
        %button.setInactiveImage("{EditorAssets}:smallPause_iImageMap");
        
        // Schedule the stop
        %time = alxGetAudioLength(%soundAsset);
        %button.soundScheduleHandle = schedule(%time, %button, "PhysicsLauncherTools::audioButtonPairStop", %button, %soundAsset);
    }
    
    // Handle stoping other sound play buttons that were pressed before this one
    if (isObject($PhysicsLauncherTools::LastSoundButton) && $PhysicsLauncherTools::LastSoundButton !$= %button)
    {
        if (alxIsPlaying($PhysicsLauncherTools::LastSoundButton.soundHandle))
            PhysicsLauncherTools::audioButtonStop($PhysicsLauncherTools::LastSoundButton, "");
    }
    
    $PhysicsLauncherTools::LastSoundButton = %button;
}

function PhysicsLauncherTools::renameCurrentLevelFile(%oldFileName, %newFileName, %level)
{
    // Check if the new name is different than the old one
    if (%oldFileName !$= %newFileName)
    {
        // Delete the old file
        echo("Removing file: " @ %oldFileName);
        %levelFile = expandPath("^PhysicsLauncherTemplate/data/levels/" @ %oldFileName @ ".scene.taml");
        %success = fileDelete(%levelFile);
        if (%success)
            echo("File " @ %oldFileName @ " removed");
        else
        {
            echo("Unable to delete file " @ %oldFileName);
            return false;
        }

        %newFile = "^PhysicsLauncherTemplate/data/levels/" @ %newFileName @ ".scene.taml";
        %success = TamlWrite(%level, %newFile);
        if (!%success)
        {
            echo(" @@@ Unable to write new file " @ %newFileName);
            return false;
        }

        return true;
    }
    else
    {
        return false;
    }
}