//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runParticleTest()
{
    %particleAssetName = "Exhaust";
    
    %effectAsset = new ParticleAsset();
    %effectAsset.assetName = %particleAssetName;   
    
    %effectAsset.LifeMode = "infinite";    
    //%effectAsset.Lifetime = 1;        

    %effectAsset.selectField( "EmissionArc" );    
        %effectAsset.setSingleDataKey( 15 );

    %effectAsset.selectField( "EmissionAngle" );    
        %effectAsset.setSingleDataKey( 180 );

           
    %emitter = %effectAsset.createEmitter();
    %emitter.EmitterName = "smoke";
    %emitter.EmitterType = "point";
    %emitter.IntenseParticles = false;
    %emitter.Image = "MelvToy:Particles4";
    %emitter.Frame = 0;
    %emitter.FixedAspect = true;
    %emitter.RandomImageFrame = false;
    %emitter.OldestInFront = true;
    %emitter.AttachPositionToEmitter = false;
    %emitter.AttachRotationToEmitter = false;

    %emitter.selectField( "Quantity" );    
        %emitter.setSingleDataKey( 20 );

    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 2 );

    %emitter.selectField( "LifetimeVariation" ); 
        %emitter.setSingleDataKey( 1 );

    %emitter.selectField( "Speed" );    
        %emitter.setSingleDataKey( 1.5 );

    %emitter.selectField( "SpeedVariation" );    
        %emitter.setSingleDataKey( 1 );

    %emitter.selectField( "SizeXVariation" );
        %emitter.setSingleDataKey( 2 );
        
    %emitter.selectField( "SizeXLife" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.2, 1 );
        %emitter.addDataKey( 0.5, 2 );
        %emitter.addDataKey( 0.9, 1 );
        %emitter.addDataKey( 1, 0 );

    //%emitter.selectField( "Spin" );    
        //%emitter.setSingleDataKey( 360 );

    //%emitter.selectField( "SpinVariation" );
        //%emitter.setSingleDataKey( 360 );
        
    //%emitter.selectField( "RandomMotion" );
        //%emitter.setSingleDataKey( 80 );

    %emitter.selectField( "FixedForce" );
        %emitter.setSingleDataKey( 1 );
        %emitter.FixedForceAngle = 90;

    %emitter.selectField( "FixedForceVariation" );
        %emitter.setSingleDataKey( 1 );

    %emitter.selectField( "AlphaChannel" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.3, 0.3 );
        %emitter.addDataKey( 0.8, 0.3 );
        %emitter.addDataKey( 1, 0 );    
        
    %emitter.deselectField();  
    

    %assetFilePath = expandPath( "^MelvToy/particle.asset.taml" );
    
	TamlWrite( %effectAsset, %assetFilePath );
	
    if ( !AssetDatabase.addSingleDeclaredAsset( ModuleDatabase.findLoadedModule( "MelvToy" ), %assetFilePath ) )
    {
        error( "Could not load the asset file:" SPC %assetFilePath );
    }

    //TamlRead( %assetFilePath );   
    
    %particlePlayer = new ParticlePlayer();
    SandboxScene.add( %particlePlayer );
    %particlePlayer.setSize( 90, 65 );
    %particlePlayer.ParticleInterpolation = false;
    //%particlePlayer.CameraIdleDistance = 50;
    %particlePlayer.Particle = "MelvToy:" @ %particleAssetName;
    %particlePlayer.play();
    //%particlePlayer.visible = false;
    //%particlePlayer.setLinearVelocity( 10, 0 );
    //%particlePlayer.setAngularVelocity( -180 );
    
    //SandboxScene.setDebugSceneObject( %particlePlayer );
    
    //%particlePlayerFilePath = expandPath( "^MelvToy/particlePlayer.taml" );
    //TamlWrite( %particlePlayer, %particlePlayerFilePath );
    //TamlRead( %particlePlayerFilePath );
    
    
    //schedule( 5000, 0, changeParticleAsset );
    
    SandboxScene.setBatchingEnabled( true );
    
    //TamlWrite( SandboxScene, "scene.taml" );
    //%newScene = TamlRead( "scene.taml" );
	
	//quit();
}

function changeParticleAsset()
{
   %asset = AssetDatabase.acquireAsset( "MelvToy:TestParticle" );
   %emitter = %asset.findEmitter( "Sputter" );
   %emitter.Frame = 1;
}
