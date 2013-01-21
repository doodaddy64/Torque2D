//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runParticleTest()
{
    %particleAssetName = "TestParticle";
    
    %effectAsset = new ParticleAsset();
    %effectAsset.assetName = %particleAssetName;   
    
    %effectAsset.LifeMode = "infinite";    
    //%effectAsset.Lifetime = 1;        
           
    %emitter = %effectAsset.createEmitter();
    %emitter.EmitterName = "Sputter";
    %emitter.EmitterType = "linex";
    %emitter.IntenseParticles = false;
    %emitter.Image = "MelvToy:Particles1";
    %emitter.Frame = 0;
    %emitter.FixedAspect = true;
    %emitter.RandomImageFrame = true;
    %emitter.OldestInFront = true;
    %emitter.AttachPositionToEmitter = true;
    %emitter.AttachRotationToEmitter = false;

    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 1 );

    %emitter.selectField( "Speed" );    
        %emitter.setSingleDataKey( 1 );

    %emitter.selectField( "SpeedVariation" );    
        %emitter.setSingleDataKey( 0 );


    %emitter.selectField( "Quantity" );    
        %emitter.setSingleDataKey( 500 );
    
    %emitter.selectField( "SizeXLife" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.1, 2 );
        %emitter.addDataKey( 0.5, 1 );
        %emitter.addDataKey( 0.9, 0.1 );
        %emitter.addDataKey( 1, 0 );

    //%emitter.selectField( "Spin" );    
        //%emitter.setSingleDataKey( 360 );

    //%emitter.selectField( "SpinVariation" );
        //%emitter.setSingleDataKey( 360 );
        
    %emitter.selectField( "RandomMotion" );
        %emitter.setSingleDataKey( 80 );

    %emitter.selectField( "FixedForce" );
        %emitter.setSingleDataKey( 50 );
        %emitter.FixedForceAngle = -90;

    %emitter.selectField( "AlphaChannel" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.1, 1 );
        %emitter.addDataKey( 0.9, 1 );
        %emitter.addDataKey( 1, 1 );    
        
    %emitter.deselectField();  
    

    %assetFilePath = expandPath( "^MelvToy/particle.asset.taml" );
    
	TamlWrite( %effectAsset, %assetFilePath );
	
    if ( !AssetDatabase.addSingleDeclaredAsset( ModuleDatabase.findLoadedModule( "MelvToy" ), %assetFilePath ) )
    {
        error( "Could not load the asset file:" SPC %assetFilePath );
    }

    //TamlRead( %assetFilePath );   
    
    %particlePlayer = new ParticlePlayer();
    SandboxScene.addToScene( %particlePlayer );
    %particlePlayer.setSize( 90, 1 );
    %particlePlayer.ParticleInterpolation = false;
    %particlePlayer.CameraIdleDistance = 50;
    %particlePlayer.Particle = "MelvToy:" @ %particleAssetName;
    %particlePlayer.play();
    //%particlePlayer.setLinearVelocity( 10, 0 );
    %particlePlayer.setAngularVelocity( -180 );
    
    //SandboxScene.setDebugSceneObject( %particlePlayer );
    
    //%particlePlayerFilePath = expandPath( "^MelvToy/particlePlayer.taml" );
    //TamlWrite( %particlePlayer, %particlePlayerFilePath );
    //TamlRead( %particlePlayerFilePath );
    
    
    //schedule( 5000, 0, changeParticleAsset );
	
	//quit();
}

function changeParticleAsset()
{
   %asset = AssetDatabase.acquireAsset( "MelvToy:TestParticle" );
   %emitter = %asset.findEmitter( "Sputter" );
   %emitter.Frame = 1;
}
