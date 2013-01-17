//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runParticleTest()
{
    %effectAsset = new ParticleAsset();
    %effectAsset.assetName = "TestParticle";      
    
    %effectAsset.Lifetime = 10;
    %effectAsset.LifeMode = "cycle";    
    
    %effectAsset.selectField( "SizeXScale" );
        %effectAsset.addDataKey( 0, 1 );
        %effectAsset.addDataKey( 0.1, 2 );
        %effectAsset.addDataKey( 8.9, 5 );
        %effectAsset.addDataKey( 10, 1 );    
        
    %effectAsset.selectField( "SizeYScale" );
        %effectAsset.addDataKey( 0, 1 );
        %effectAsset.addDataKey( 1, 5 );
        %effectAsset.addDataKey( 9, 3 );
        %effectAsset.addDataKey( 10, 1 );
        
    %effectAsset.deselectField();
           
    %emitter = %effectAsset.createEmitter();
    %emitter.setEmitterName( "Sputter" );
    %emitter.EmitterType = "Area";
    %emitter.Image = "MelvToy:MiniTileMapImage";
    
    %emitter.selectField( "AlphaChannel" );
        %emitter.setRepeatTime( 2 );
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
    
    %particlePlayer = new ParticlePlayer();
    
    
	
	quit();
}