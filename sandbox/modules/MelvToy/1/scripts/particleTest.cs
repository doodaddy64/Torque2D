//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runParticleTest()
{
    %asset = new ParticleAsset();
    %asset.Lifetime = 10;
    %asset.LifeMode = "cycle";
    
    %asset.selectField( "SizeXScale" );
        %asset.addDataKey( 0, 1 );
        %asset.addDataKey( 5, 2 );
        %asset.addDataKey( 9, 5 );
        %asset.addDataKey( 10, 1 );    
        
    %asset.selectField( "SizeYScale" );
        %asset.addDataKey( 0, 1 );
        %asset.addDataKey( 1, 5 );
        %asset.addDataKey( 9, 3 );
        %asset.addDataKey( 10, 1 );    
        
    %asset.deselectField();
    
    %emitter = %asset.createEmitter();
    %emitter.setEmitterName( "nova" );
    
    
	TamlWrite( %asset, "^MelvToy/particle.asset.taml" );
	%newAsset = TamlRead( "^MelvToy/particle.asset.taml" );
	quit();
}