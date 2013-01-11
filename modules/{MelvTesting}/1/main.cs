//OpenRemoteDebugger( 1, 1010, "abc" );

function startMelvTesting()
{
       
    setScreenMode( 1024, 768, 32, false );
    //setScreenMode( 2560, 1600, 32, true );

	$NoImageRenderProxy = new RenderProxy()
	{
		ImageMap = "{PhysicsLauncherAssets}:defaultImageImageMap";
		//Animation = "{PhysicsLauncherAssets}:PL_GorillaFlightProjectileAnim";
	};
	
	testScreen();

    Canvas.BackgroundColor = "DimGray";
    Canvas.UseBackgroundColor = true;

    //testScene.BackgroundColor = "HotPink";
    //testScene.SetBackgroundColor( "HotPink" );
    //testScene.UseBackgroundColor = true;

	//testQueryAssetName();
	//testQueryAssetType();
	//testAssetAcquisitionAndRelease();
	
	//%sprite1 = new Sprite();
	//testScene.addToScene( %sprite1 );
	//%sprite1.setSize( 50, 50 );
	//%sprite1.ImageMap = "{MelvTesting}:MiniTileMapImage";
	//%sprite1.Frame = 0;
	//%sprite1.setBlendColor( "Red" );
	//%sprite1.setAngularVelocity( -90 );
	//%sprite1.setLinearVelocity( 10, 0 );
	//%sprite1.setSceneLayer( 31 );
	//%sprite1.setSceneLayerDepth( 0 );
	
	//TamlWrite( %sprite1, "sprite1.xml" );
	//%readSprite = TamlRead( "sprite1.xml" );	
//
	//%sprite2 = new Sprite();
	//testScene.addToScene( %sprite2 );
	//%sprite2.setPosition( 4, 4 );
	//%sprite2.setSize( 15, 15 );
	//%sprite2.ImageMap = "{MelvTesting}:isotiles2";
	//%sprite2.Frame = 2;
	//%sprite2.setSceneLayer( 31 );
	//%sprite2.setSceneLayerDepth( 1 );
//
	//%sprite3 = new Sprite();
	//testScene.addToScene( %sprite3 );
	//%sprite3.setPosition( 4, -2 );
	//%sprite3.setSize( 15, 15 );
	//%sprite3.ImageMap = "{MelvTesting}:isotiles2";
	//%sprite3.Frame = 4;
	//%sprite3.setSceneLayer( 31 );
	//%sprite3.setSceneLayerDepth( 15 );
	
	//%sprite3.setSceneLayerDepthForward();
	//%sprite3.setSceneLayerDepthForward();


    //testScene.layerSortMode31 = "-Y Axis";
    //testScene.layerSortMode30 = "-Y Axis";

    //testScene.layerSortMode31 = "z axis";
    //testScene.layerSortMode30 = "z axis";
    
    //testScene.setDebugOn( 0, 2, 3 );
    //testScene.setDebugOn( 0 );
    
    
	%composite = new CompositeSprite();
	testScene.addToScene( %composite );
	%composite.BatchIsolated = "true";

    %composite.setDefaultSpriteStride( 2, 2 );
    %composite.setDefaultSpriteSize( 2 );
    
    %composite.setBatchLayout( "rect" );
    
    //%composite.setAngle( 30.0001 );
	
	%frame = 0;
	
    //for ( %y = 0; %y <= 0; %y++ )
    for ( %y = -50; %y <= 50; %y++ )
	{
	    for ( %x = -50; %x <= 50; %x++ )
        {
            %composite.addSprite( %x SPC %y );
            %composite.setSpriteImage( "{MelvTesting}:MiniTileMapImage", getRandom(0,15) );
            //%composite.setSpriteAngle( 45 );
            //%composite.setSpriteBlendColor( "HotPink" );
            //%composite.setSpriteVisible( getRandom(1,10) < 5 );
            //%dataObject = new ScriptObject();
            //%dataObject.Breakable = true;
            //%dataObject.BreakLife = 3;
            //%composite.setDataObject( %dataObject );
            
            //%frame++;
            //if ( %frame == 16 ) %frame = 0;
            
        }
	}
	
	schedule( 10000, 0, profileDump );
	
	//
	//testSceneWindow2D.setCurrentCameraPosition( 0, 0 );
	//testScene.setDebugSceneObject( %composite );
	%composite.setAngularVelocity( 72 );
	//%composite.setLinearVelocity( -200, 0 );
	
	//TamlWrite( %composite, "composite.xml" );
	//%newComposite1 = TamlRead( "composite.xml" );
	//testScene.addToScene( %newComposite1 );

	//TamlWrite( %composite, "composite.bin", "binary" );
	//%newComposite2 = TamlRead( "composite.bin", "binary" );
	//testScene.addToScene( %newComposite2 );
	
    //%assetTags = AssetDatabase.getAssetTags();    
    //%assetTags.createtag( "Test" );
    //%assetTags.createtag( "Image" );
    //%assetTags.createtag( "Animation" );
    
    //%assetTags.tag( "{MelvTesting}:MiniTileMapAnim", "Test" );
    //%assetTags.tag( "{MelvTesting}:MiniTileMapImage", "Test" );    
    //%assetTags.tag( "{MelvTesting}:CuriosityImage", "Test" );    
    //%assetTags.tag( "{MelvTesting}:MiniTileMapImage", "Image" );    
    //%assetTags.tag( "{MelvTesting}:CuriosityImage", "Image" );    
    //%assetTags.tag( "{MelvTesting}:MiniTileMapAnim", "Animation" );
    
    //AssetDatabase.saveAssetTags();
	//testQueryAssetTag();
	//AssetDatabase.deleteAsset( "{MelvTesting}:MiniTileMapImage", true, true );

    //AssetDatabase.IgnoreAutoUnload = true;
    // Scroller.
    //%scroller = new Scroller();
    //%scroller.setSize( 100, 75 );
    //%scroller.scrollX = -30;
    //%scroller.scrollY = 15;
    //%scroller.repeatX = 4;
    //%scroller.repeatY = 3;
    //%scroller.setScrollPositionX( 20 );
    //%scroller.imageMap = "{PhysicsLauncherAssets}:PL_GorillaProjectileImageMap";
    //%scroller.imageMap = "{MelvTesting}:ChoppyImage";
    //%scroller.frame = 1;
    //%scroller.imageMap = "";
    //%scroller.Animation = "{PhysicsLauncherAssets}:MiniTileMapAnim";
    //%scroller.frame = 80;
    //%scroller.imageMap = "{PhysicsLauncherAssets}:CreditsImageImageMap";
    
	//%scroller.blendMode = true;
	//%scroller.blendColor = "HotPink";
    
    //AssetDatabase.IgnoreAutoUnload = false;
    
    //testScene.addToScene( %scroller );        

    //TamlWrite( testScene, "testScene.taml" );
    //%scene = TamlRead( "testScene.taml" );
    //TamlWrite( %scene, "testSceneOut.taml" );
    //quit();

	//%assetTagManifest = AssetDatabase.getAssetTags();

	//%assetTagManifest.createTag( "Backgrounds" );
	//%assetTagManifest.tag( "{PhysicsLauncherAssets}:Level1BackgroundImageMap", "Effects" );
	//%assetTagManifest.untag( "{PhysicsLauncherAssets}:Level1BackgroundImageMap", "Effects" );
	//%assetTagManifest.deleteTag( "Projectiles" );
	//%assetId = "{PhysicsLauncherAssets}:PL_GorillaFlightProjectileAnim";
	//%tagCount = %assetTagManifest.getAssetTagCount(%assetId);
	//for ( %index = 0; %index < %tagCount; %index++ )
	//{
	//    echo( %assetTagManifest.getAssetTag(%assetId, %index) );
	//}

	//AssetDatabase.dumpDeclaredAssets();


	//%asset = AssetDatabase.acquireAsset( "{PhysicsLauncherAssets}:PL_GorillaProjectileImageMap" );
	//%assetId = %asset.getAssetId();
	//testAssetDependsOn( %assetId );
	//testAssetIsDependedOn( %assetId );

	//%assetSnapshot = new AssetSnapshot();
	//AssetDatabase.getAssetSnapshot( %assetSnapshot, %assetId );
	//%asset.assetName = "Should not be able to do this!";
	//%asset.AssetDescription = "New asset description:" SPC getRandom();
	//AssetDatabase.setAssetSnapshot( %assetSnapshot, %assetId );
	//%asset.refreshAsset();
	//AssetDatabase.refreshAllAssets(true);


    //AssetDatabase.purgeAssets();

	//%asset.refreshAsset();

	//AssetDatabase.renameDeclaredAsset( "{PhysicsLauncherAssets}:Background9ImageMap", "{PhysicsLauncherAssets}:Background1ImageMap" );

	//AssetDatabase.saveAssetTags();

	//%scene = TamlRead( "^Modules/{PhysicsLauncher}/1/data/levels/world1level1.scene.taml" );
	//testSceneWindow2D.setScene( %scene );

	//testSpriteCtrl.delete();
	//$NoImageRenderProxy.delete();
	
	//alxPlay( "{PhysicsLauncherAssets}:PL_LaunchSound" );
	//alxPlay( "{PhysicsLauncherAssets}:PL_TitleSound" );


	//%asset = AssetDatabase.acquireAsset( "{PhysicsLauncherAssets}:PL_TitleSound" );
	
	//%asset.AudioFile="#PL_TitleAudio.wav";
    //echo( ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" SPC %asset.AudioFile );	
	
	//%asset.refreshAsset();
	//refreshTextureManager();
	
}

function profileDump()
{
	profilerDumpToFile( "dump.log" );
	quit();    
}

function testScreen()
{
    new SceneWindow(testSceneWindow2D);
    new Scene(testScene);
    testSceneWindow2D.setScene( testScene );
    Canvas.setContent(testSceneWindow2D);   
    
    
    if(!isObject(MelvDebugProfile)) new GuiControlProfile (MelvDebugProfile)
    {
        tab = false;
        canKeyFocus = false;
        hasBitmapArray = false;
        mouseOverSelected = false;

        // fill color
        opaque = false;
        fillColor = "0 0 0 128";
        
        // font
        fontType = $platform $= "windows" ? "lucida console" : "monaco";
        fontSize = 14;
        fontColor = "255 255 255 255";

        // border color
        border = 1;
        borderColor   = "100 100 100 255";
        borderColorHL = "128 128 128";
        borderColorNA = "64 64 64";

    }; 
    
    testSceneWindow2D.Profile = MelvDebugProfile;  
    
    //new GuiSceneObjectCtrl(testT2DViewer);    
    //testT2DViewer.position = "0 0";
    //testT2DViewer.extent = "400 400";
    //Canvas.setContent(testT2DViewer);
    
    //new guiSpriteCtrl(testSpriteCtrl);
    //Canvas.setContent(testSpriteCtrl);
    
    //new GuiImageButtonCtrl(testImageButtonCtrl);
    //Canvas.setContent(testImageButtonCtrl);    
}

function testQueryAssetName()
{
    %assetName = "PL_DefaultProjectile";
    %assetQuery = new AssetQuery();
    AssetDatabase.findAssetName( %assetQuery, %assetName );
    
    %assetCount = %assetQuery.Count;

    echo( "Asset Query: Search for asset name:" SPC %assetName SPC "found" SPC %assetCount SPC "asset(s):" );
    
    for( %index = 0; %index < %assetCount; %index++ )
    {
        %assetId = %assetQuery.getAsset( %index );
        
        echo( "> #" @ %index @ ":" SPC %assetId );
    }
    
    %assetQuery.delete();
}

function testQueryAssetType()
{
    %assetType = "ImageAsset";
    //%assetType = "AnimationAsset";
    %assetQuery = new AssetQuery();
    AssetDatabase.findAssetType( %assetQuery, %assetType );

    %assetBase = new AssetQuery();
    %assetBase.set( %assetQuery );    
    
    %assetCount = %assetBase.Count;

    echo( "Asset Query: Search for asset type:" SPC %assetType SPC "found" SPC %assetCount SPC "asset(s):" );
    
    for( %index = 0; %index < %assetCount; %index++ )
    {
        %assetId = %assetBase.getAsset( %index );
        
        echo( "> #" @ %index @ ":" SPC %assetId );
    }    
    
    %assetQuery.delete();
    %assetBase.delete();
}

function testQueryAssetTag()
{
    %assetTag1 = "test";
    %assetTag2 = "animation";
    %assetQuery = new AssetQuery();
    AssetDatabase.findTaggedAssets( %assetQuery, %assetTag1 );
    AssetDatabase.findTaggedAssets( %assetQuery, %assetTag2, true );
    
    %assetCount = %assetQuery.Count;

    echo( "*********************************************************************************************************************************************************" );
    echo( "Asset Query: Search for asset tag found" SPC %assetCount SPC "asset(s):" );
    
    for( %index = 0; %index < %assetCount; %index++ )
    {
        %assetId = %assetQuery.getAsset( %index );
        
        echo( "> #" @ %index @ ":" SPC %assetId );
    }

    echo( "*********************************************************************************************************************************************************" );
    
    %assetQuery.delete();
}

function testAssetDependsOn( %assetId )
{
    %assetQuery = new AssetQuery();
    
    AssetDatabase.findAssetDependsOn( %assetQuery, %assetId );
    
    %assetCount = %assetQuery.Count;

    echo( "Asset Query: Search Asset Id" SPC %assetId SPC "depends-on found" SPC %assetCount SPC "asset(s):" );
    
    for( %index = 0; %index < %assetCount; %index++ )
    {
        %assetId = %assetQuery.getAsset( %index );
        
        echo( "> #" @ %index @ ":" SPC %assetId );
        
        testAssetDependsOn( %assetId );       
    }
    
    %assetQuery.delete();   
}

function testAssetIsDependedOn( %assetId )
{
    %assetQuery = new AssetQuery();
    
    AssetDatabase.findAssetIsDependedOn( %assetQuery, %assetId );
    
    %assetCount = %assetQuery.Count;

    echo( "Asset Query: Search Asset Id" SPC %assetId SPC "is-depended-on found" SPC %assetCount SPC "asset(s):" );
    
    for( %index = 0; %index < %assetCount; %index++ )
    {
        %assetId = %assetQuery.getAsset( %index );
                      
        echo( "> #" @ %index @ ":" SPC %assetId );
        
        testAssetIsDependedOn( %assetId );        
    }
    
    %assetQuery.delete();   
}

function testAssetAcquisitionAndRelease()
{
    //%asset = AssetDatabase.acquireAsset( "{PhysicsLauncherAssets}:BackgroundOneImageMap" );
    //AssetDatabase.releaseAsset( %asset.getAssetId() );

    //AssetDatabase.acquireAsset( "{PhysicsLauncherAssets}:PL_GorillaFlightProjectileAnim" );

    // Static Sprite#1
    //%sprite1 = new Sprite();
    //%sprite1.imageMap = "{PhysicsLauncherAssets}:Background1ImageMap";
    //%sprite1.setImageMap( "{PhysicsLauncherAssets}:Background1ImageMap" );
    //%sprite1.setSize( 100, 75 );
    //echo( "SPRITE >>>>>>>>>>>>>>>>>" SPC %sprite1.imageMap );
    //testScene.addToScene( %sprite1 );    

    // Static Sprite#1
    //%sprite2 = new Sprite();
    //%sprite2.imageMap = "{PhysicsLauncherAssets}:Background2ImageMap";
    //%sprite2.setSize( 100, 75 );
    //testScene.addToScene( %sprite2 );

    //AssetDatabase.acquireAsset( "{PhysicsLauncherAssets}:PL_DefaultProjectileBlue1ImageMap" );
    
    // Animated Sprite.    
    //%sprite3 = new Sprite();
    //%sprite3.playAnimation("{PhysicsLauncherAssets}:PL_GorillaFlightProjectileAnim");
    //%sprite3.setSize( 20, 20 );
    //testScene.addToScene( %sprite3 );

    //TamlWrite( %sprite, "sprite.taml" );
    //%sprite.delete();    
    //%newSprite = TamlRead( "sprite.taml" );
    //%newSprite.delete();

    //%sprite1 = new Sprite();
    //%sprite1.imageMap = "{PhysicsLauncherAssets}:Background1ImageMap";    
    //testT2DViewer.setSceneObject( %sprite1 );
    
    //testSpriteCtrl.Image = "{PhysicsLauncherAssets}:PL_GorillaProjectileImageMap";
    //testSpriteCtrl.Frame = 5;
    //testSpriteCtrl.Animation = "{PhysicsLauncherAssets}:PL_GorillaFlightProjectileAnim";
    //echo( ">>>>>>>>>>>>>>>>>>>>>>" SPC testSpriteCtrl.Animation );
    //testSpriteCtrl.setAnimation( "{PhysicsLauncherAssets}:PL_GorillaFlightProjectileAnim" );
    //testImageButtonCtrl.NormalImage = "{PhysicsLauncherAssets}:PL_DefaultProjectileBlue1ImageMap";
    //testImageButtonCtrl.HoverImage = "{PhysicsLauncherAssets}:PL_DefaultProjectileBlue2ImageMap";
    //testImageButtonCtrl.DownImage = "{PhysicsLauncherAssets}:PL_DefaultProjectileBlue3ImageMap";
    //testImageButtonCtrl.InactiveImage = "{PhysicsLauncherAssets}:PL_DefaultProjectileBlue4ImageMap";

        
    
    //TamlWrite( testSpriteCtrl, "spritectrl.taml" );
    
}
