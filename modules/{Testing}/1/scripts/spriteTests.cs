//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function createTestScene()
{
    $SpriteTestSceneWindow = new SceneWindow()
    { 
        Profile = "GuiDefaultProfile";
        useMouseEvents = "1";
        Active = "1";
    };
    
    if (!isObject($SpriteTestSceneWindow))
    {
       error("Failed to create SceneWindow");
       quit();
    }

    $SpriteTestScene = new Scene();
    
    if (!isObject($SpriteTestScene))
    {
        error("Failed to create Scene");
        quit();
    }
    
    $SpriteTestSceneWindow.setScene( $SpriteTestScene );
    Canvas.setContent($SpriteTestSceneWindow);
}

//-----------------------------------------------------------------------------

function createDefaultRenderProxy()
{
    // Create default image.
    %defaultImage = new ImageAsset()
    {
        ImageFile = expandPath( "^{Testing}/assets/images/defaultImage.png" );
    };
    %defaultAssetId = AssetDatabase.addPrivateAsset( %defaultImage );

    // Create no-image render proxy.
    $NoImageRenderProxy = new RenderProxy()
    {
        Image = %defaultAssetId;
    };    
}

//-----------------------------------------------------------------------------

function createLargeSprite()
{    
    %sprite = new Sprite(Curiosity)
    {
        Image = "{Testing}:CuriosityImage";
        frame = 0;
        size = "20 20";
    };
    
    $SpriteTestScene.addToScene( %sprite );
}

//-----------------------------------------------------------------------------

function createAnimatedSprite()
{    
    %sprite = new Sprite(TileAnimation)
    {
        Animation = "{Testing}:MiniTileMapAnim";
        size = "20 20";
    };

    $SpriteTestScene.addToScene( %sprite );
}

//-----------------------------------------------------------------------------

function createBox2DScene()
{
    $SpriteTestScene.setGravity( 0, -9.8 );

    %worldBounds = new SceneObject();
    %worldBounds.setBodyType( static );
    %worldBounds.createPolygonBoxCollisionShape( 100, 4, 0, -35.5 );
    %worldBounds.createPolygonBoxCollisionShape( 4, 70, -48, 0 );
    %worldBounds.createPolygonBoxCollisionShape( 4, 70, 48, 0 );
    
    $SpriteTestScene.addToScene( %worldBounds );    
    
    for( %n = 0; %n < 10; %n++ )
    {
        %sprite = new Sprite()
        {
            //Animation = "{Testing}:MiniTileMapAnim";
            Image = "{Testing}:CuriosityImage";
            position = getRandom( -40, 40 ) SPC getRandom( 100, 40 );
            size = "5 5";
            angle = getRandom(0,360);
        };
        
        %sprite.createPolygonBoxCollisionShape( %sprite.size );
        $SpriteTestScene.addToScene( %sprite );
    }
}

//-----------------------------------------------------------------------------

function SceneWindow::onTouchDown(%this, %touchID, %worldPos)
{
    echo("Click on: " @ %worldPos);

    createBlockAtPoint(%worldPos);
}

//-----------------------------------------------------------------------------

function createBlockAtPoint(%worldPos)
{
    %sprite = new Sprite()
    {
        Animation = "{Testing}:MiniTileMapAnim";
        position = %worldPos;
        size = "5 5";
        angle = getRandom(0,360);
    };
    
    %sprite.createPolygonBoxCollisionShape( %sprite.size );
    $SpriteTestScene.addToScene( %sprite );
}

//-----------------------------------------------------------------------------

function bindSpriteKeys()
{
    if (!isObject(moveMap))
        new ActionMap(moveMap);
        
    moveMap.bind(keyboard, space, "showDebugInfo");
}

//-----------------------------------------------------------------------------

function showDebugInfo(%val)
{
    if (%val)
    {    
        if (!$DebugBannerOn)
            $SpriteTestScene.setDebugOn( 0, 5 );
        else
            $SpriteTestScene.setDebugOff( 0, 5 );

        $DebugBannerOn = !$DebugBannerOn;
    }
}

//-----------------------------------------------------------------------------

function runSpriteTests()
{
    createDefaultRenderProxy();

    createTestScene();
    
    createBox2DScene();
    
    bindSpriteKeys();
}

//-----------------------------------------------------------------------------

function endSpriteTests()
{
    $SpriteTestSceneWindow.delete();
}