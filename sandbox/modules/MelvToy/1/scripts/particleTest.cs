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
    %emitter.EmitterType = "point";
    //%emitter.IntenseParticles = true;
    %emitter.Image = "MelvToy:MiniTileMapImage";
    %emitter.FixedAspect = true;
    %emitter.RandomImageFrame = true;

    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 10 );

    %emitter.selectField( "Speed" );    
        %emitter.setSingleDataKey( 1 );

    //%emitter.selectField( "SpeedVariation" );    
        //%emitter.setSingleDataKey( 4 );


    %emitter.selectField( "Quantity" );    
        %emitter.setSingleDataKey( 1000 );
    
    %emitter.selectField( "SizeXLife" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.2, 3 );
        %emitter.addDataKey( 0.9, 2 );
        %emitter.addDataKey( 1, 0 );

    %emitter.selectField( "Spin" );    
        %emitter.setSingleDataKey( 360 );

    //%emitter.selectField( "SpinVariation" );
        //%emitter.setSingleDataKey( 360 );
        
    //%emitter.selectField( "RandomMotion" );
        //%emitter.setSingleDataKey( 200 );

    //%emitter.selectField( "FixedForce" );
        //%emitter.setSingleDataKey( 100 );
        //%emitter.FixedForceAngle = 0;
            
    %emitter.selectField( "AlphaChannel" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.1, 1 );
        %emitter.addDataKey( 0.9, 1 );
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
    SandboxScene.addToScene( %particlePlayer );
    %particlePlayer.setSize( 40 );
    %particlePlayer.Particle = "MelvToy:" @ %particleAssetName;
    %particlePlayer.play();
    
    //%particlePlayerFilePath = expandPath( "^MelvToy/particlePlayer.taml" );
    //TamlWrite( %particlePlayer, %particlePlayerFilePath );
    //TamlRead( %particlePlayerFilePath );
        
	
	//quit();
}