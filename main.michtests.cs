//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Set log mode.
setLogMode(2);

// Controls whether the execution or script files or compiled DSOs are echoed to the console or not.
// Being able to turn this off means far less spam in the console during typical development.
setScriptExecEcho( false );

// Controls whether all script execution is traced (echoed) to the console or not.
trace( false );

// Controls whether global 
$pref::T2D::imageAssetGlobalFilterMode = "Smooth";

// Disable the vertical sync.
$pref::Video::disableVerticalSync = 1;

// The name of the company. Used to form the path to save preferences. Defaults to GarageGames
// if not specified.
// The name of the game. Used to form the path to save preferences. Defaults to C++ engine define TORQUE_GAME_NAME
// if not specified.
// Appending version string to avoid conflicts with existing versions and other versions.
setCompanyAndProduct("GarageGames", "3StepStudio" @ getThreeStepStudioVersion());

// Set module database information echo.
ModuleDatabase.EchoInfo = false;

// Set asset database information echo.
AssetDatabase.EchoInfo = true;

// Is a module merge available?
if ( ModuleDatabase.isModuleMergeAvailable() )
{
    // Yes, so merge modules.
    if ( ModuleDatabase.mergeModules( "modules", true, false ) == false )
    {
        error( "A serious error occurred merging modules!" );
        quit();
    }
}

// Scan modules.
ModuleDatabase.scanModules( "modules" );

//-----------------------------------------------------------------------------

function resetCanvas()
{
    if (isObject(Canvas))
        Canvas.repaint();
}

//-----------------------------------------------------------------------------

function createDefaultProfile()
{
    if(!isObject(GuiDefaultProfile)) new GuiControlProfile (GuiDefaultProfile)
    {
        tab = false;
        canKeyFocus = false;
        hasBitmapArray = false;
        mouseOverSelected = false;

        // fill color
        opaque = false;
        fillColor = "211 211 211 64";
        fillColorHL = "244 244 244 64";
        fillColorNA = "244 244 244 64";
        
        // font
        fontType = $platform $= "windows" ? "lucida console" : "monaco";
        fontSize = 12;
        fontColor = "255 255 255 255";

        // border color
        border = 1;
        borderColor   = "100 100 100 255";
        borderColorHL = "128 128 128";
        borderColorNA = "64 64 64";

    };
}

//-----------------------------------------------------------------------------

function createDefaultCanvas()
{
    if (!createCanvas("Torque 2D"))
    {
        error("Canvas creation failed. Shutting down.");
        quit();
    }
    
    echo("- Created canvas successfully");
    
    if (!setScreenMode( 1024, 768, 32, false ))
    {
        error("Could not set screen mode");
        quit();
    }
}

//-----------------------------------------------------------------------------

function createDefaultRenderProxy()
{
    // Create default image.
    %defaultImage = new ImageAsset()
    {
        ImageFile = expandPath( "./modules/{MelvTesting}/1/assets/images/defaultImage.png" );
    };
    %defaultAssetId = AssetDatabase.addPrivateAsset( %defaultImage );

    // Create no-image render proxy.
    $NoImageRenderProxy = new RenderProxy()
    {
        Image = %defaultAssetId;
    };    
}

//-----------------------------------------------------------------------------

function createTestScene()
{
    new SceneWindow(testSceneWindow2D)
    { 
        Profile = "GuiDefaultProfile";
        useMouseEvents = "1";
        Active = "1";
    };
    
    if (!isObject(testSceneWindow2D))
    {
       error("Failed to create SceneWindow");
       quit();
    }

    new Scene(testScene);
    
    if (!isObject(testScene))
    {
        error("Failed to create Scene");
        quit();
    }
    
    testSceneWindow2D.setScene( testScene );
    Canvas.setContent(testSceneWindow2D);
}

//-----------------------------------------------------------------------------

function createLargeSprite()
{
    %asset = new ImageAsset()
    {
        ImageFile = expandPath("./modules/{MelvTesting}/1/assets/images/Curiosity.jpg");
    };
    
    %assetId = AssetDatabase.addPrivateAsset( %asset );
    
    %sprite = new Sprite(Curiosity)
    {
        Image = %assetId;
        frame = 0;
        size = "20 20";
    };
    
    testScene.addToScene( %sprite );
}

//-----------------------------------------------------------------------------

function createAnimatedSprite()
{
    %asset = new ImageAsset()
    {
        ImageFile = expandPath("./modules/{MelvTesting}/1/assets/images/MiniTileMap.png");
        CellCountX = 4;
        CellCountY = 4;
        CellWidth = 128;
        CellHeight = 128;
    };
    
    %imageAssetID = AssetDatabase.addPrivateAsset( %asset );
    
    %animation = new AnimationAsset()
    {
        Image = %imageAssetID;
        AnimationFrames = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15";
        AnimationTime = 16;
    };

    %animationID = AssetDatabase.addPrivateAsset( %animation );
       
    %sprite = new Sprite(TileAnimation)
    {
        Animation = %animationID;
        size = "20 20";
    };
    testScene.addToScene( %sprite );
}

//-----------------------------------------------------------------------------

function createBox2DScene()
{
    testScene.setDebugOn( 0, 5 );
    testScene.setGravity( 0, -9.8 );
       
    %asset = new ImageAsset()
    {
        ImageFile = expandPath("./modules/{MelvTesting}/1/assets/images/MiniTileMap.png");
    };
    
    %assetId = AssetDatabase.addPrivateAsset( %asset );
           
    %animation = new AnimationAsset()
    {
        Image = %imageAssetID;
        AnimationFrames = "0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15";
        AnimationTime = 16;
    };

    $TestAnimationID = AssetDatabase.addPrivateAsset( %animation );
    

    %worldBounds = new SceneObject();
    %worldBounds.setBodyType( static );
    %worldBounds.createPolygonBoxCollisionShape( 100, 4, 0, -35.5 );
    %worldBounds.createPolygonBoxCollisionShape( 4, 70, -48, 0 );
    %worldBounds.createPolygonBoxCollisionShape( 4, 70, 48, 0 );
    testScene.addToScene( %worldBounds );    
    
    for( %n = 0; %n < 10; %n++ )
    {
        %sprite = new Sprite()
        {
            //Animation = $TestAnimationID;
            Image = %assetId;
            position = getRandom( -40, 40 ) SPC getRandom( 100, 40 );
            size = "5 5";
            angle = getRandom(0,360);
        };
        %sprite.createPolygonBoxCollisionShape( %sprite.size );
        testScene.addToScene( %sprite );
    }
}

//-----------------------------------------------------------------------------

function escapeQuit(%val)
{
    quit();
}

//-----------------------------------------------------------------------------

function createTestBindings()
{
    new ActionMap(moveMap);
    
    moveMap.bind(keyboard, escape, "escapeQuit");
    
    moveMap.push();
}

//-----------------------------------------------------------------------------

function pickFile()
{
    //$T2D::ImageSpec = "All Supported Graphics (*.jpg;*.jpeg;*.png;)|*.png;*.jpg;*.jpeg|";

    %assetFileSpec = "Asset Files (*.taml)|*.taml|";
    
    echo("Opening a file");
    
    %dlg = new OpenFileDialog()
    {
        Filters = %assetFileSpec;
        ChangePath = false;
        MustExist = true;
        MultipleFiles = true;
    };
    
    if (!isObject(%dlg))
    {
        error("Could not create OpenFileDialog");
        return;
    }

    echo("Executing OpenFileDialog");

    if (%dlg.Execute())
    {
        echo("Successfully picked a file");
        
        %file = %dlg.files[0];
        %fileOnlyName = fileName(%file);
        %fileOnlyBase = fileBase(%file);
        %fileOnlyExtension = fileExt(%file);
        
        echo("File: " @ %file);
        echo("Name: " @ %fileOnlyName);
        echo("Base: " @ %fileOnlyBase);
        echo("Extension: " @ %fileOnlyExtension);
        
        %fileExists = isFile(%file);
        
        if (%fileExists)
        {
            echo("File does exist. Let's save it to another location");
            saveFile(%file);
        }
        else
        {
            echo("File does not exist");
        }
    }
    else
    {
        error("Could not pick a file");
    }
}

function saveFile(%originalFile)
{
    %assetFileSpec = "Asset Files (*.taml)|*.taml|";
   
    %object = TamlRead(%originalFile);
     
   %dlg = new SaveFileDialog()
   {
      Filters           = %assetFileSpec;
      DefaultPath       = filePath(%originalFile);
      DefaultFile       = fileName(%originalFile);
      ChangePath        = true;
      OverwritePrompt   = true;
   };
   
    if (%dlg.execute())
    {
        %file = %dlg.FileName;
        
        echo("Saving file to: " @ %file);
        
        TamlWrite(%object, %file);
        
        if (isFile(%file))
        {
            echo("File finished saving properly");
        }
    }
}

function SceneWindow::onTouchDown(%this, %touchID, %worldPos)
{
    echo("Click on: " @ %worldPos);

    //pickFile();
    
    createBlockAtPoint(%worldPos);
    
    //dragSprite(%worldPos);
}

function SceneWindow::onTouchDragged(%this, %touchID, %worldPos)
{
    updateDraggingSprite(%worldPos);
}

function SceneWindow::onTouchUp(%this, %touchID, %worldPos)
{
    releaseDraggingSprite();
}

function createBlockAtPoint(%worldPos)
{
    %sprite = new Sprite()
    {
        Animation = $TestAnimationID;
        position = %worldPos;
        size = "5 5";
        angle = getRandom(0,360);
    };
    
    %sprite.createPolygonBoxCollisionShape( %sprite.size );
    testScene.addToScene( %sprite );
}

function dragSprite(%startingPoint)
{
    %objList = testScene.pickPoint(%startingPoint);
    %objCount = getWordCount(%objList);
    %sprite = "";
    
    for(%i = 0; %i < %objCount; %i++)
    {
        %object = getWord(%objList, %i);
        
        if (%object.isMemberOfClass("Sprite"))
        {    
            %sprite = %object;
            break;
        }
    }
    
    $currentJointID = testScene.createTargetJoint(%sprite, %startingPoint, 1000);
    
    echo("New joint: " @ $currentJointID);
}

function updateDraggingSprite(%worldPos)
{
    echo(currentJointID);
    
    if ($currentJointID !$= "")
    {
        echo("Found joint: " @ %currentJointID);
        testScene.setTargetJointTarget($currentJointID, %worldPos);
    }
}

function releaseDraggingSprite()
{
    if ($currentJointID !$= "")
    {
        echo("Deleting target joint: " @ $currentJointID);
        testScene.deleteJoint($currentJointID);
    }
}

//-----------------------------------------------------------------------------

function startMichTesting()
{
    echo("@@@ Starting Mich tests");

    createDefaultProfile();
    createDefaultCanvas();
    createDefaultRenderProxy();
    
    createTestScene();
    
    //createLargeSprite();
    //createAnimatedSprite();
    createBox2DScene();
    
    createTestBindings();
}

//-----------------------------------------------------------------------------

startMichTesting();
