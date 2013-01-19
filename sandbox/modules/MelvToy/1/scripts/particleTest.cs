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
    %emitter.setEmitterName( "Sputter" );
    %emitter.EmitterType = "point";
    %emitter.Image = "MelvToy:MiniTileMapImage";
    %emitter.FixedAspect = true;

    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 1 );

    %emitter.selectField( "Quantity" );    
        %emitter.setSingleDataKey( 100 );
    
    %emitter.selectField( "SizeXLife" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.5, 5 );
        %emitter.addDataKey( 1, 0 );
            
    %emitter.selectField( "AlphaChannel" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.5, 1 );
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