//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createMelvToy( %scopeSet )
{
    // Load scripts.
    exec( "./scripts/compositeSpriteTest.cs" );
    exec( "./scripts/particleTest.cs" );
    
    // Run test.
    //runCompositeSpriteTest();
    runParticleTest();
    
    //createRangeControl( "IsoWidth", "Width", "0 0", 0, 1000, "" );
}

//-----------------------------------------------------------------------------

function destroyMelvToy( %scopeSet )
{
}
