//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runParticleTest()
{
    %asset = new ParticleAsset();
    
	TamlWrite( %asset, "^MelvToy/particle.asset.taml" );
	%newAsset = TamlRead( "^MelvToy/particle.asset.taml" );
	quit();
}