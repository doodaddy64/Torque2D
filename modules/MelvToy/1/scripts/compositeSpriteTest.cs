//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runCompositeSpriteTest()
{
    // Fetch the stock color count.
    %stockColorCount = getStockColorCount();
        
	%composite = new CompositeSprite();
	SandboxScene.add( %composite );
	
    %composite.setDefaultSpriteStride( 25, 25 );
    %composite.setDefaultSpriteSize( 20 );    
    %composite.BatchLayout = "iso";
	%composite.BatchIsolated = "true";    
	%composite.BatchSortMode = "-Y";
    //%composite.setAngularVelocity( -10 );
    %composite.DefaultSpriteSize = 8;
    %composite.DefaultSpriteStride = "4 2";
	
    for ( %y = -50; %y <= 50; %y++ )
	{
	    for ( %x = -50; %x <= 50; %x++ )
        {
            if ( getRandom(0,100) < 30 )
                continue;
                
            %composite.addSprite( %x SPC %y );
            %composite.setSpriteImage( "MelvToy:isoTiles2", getRandom(0,4) );
            //%composite.setSpriteName( "My Name is" SPC %x SPC %y );
            //%composite.setSpriteAngle( getRandom(0,360) );
            //%composite.setSpriteRenderGroup( "melv" );
            //%composite.setSpriteBlendColor( getStockColorName(getRandom(0,%stockColorCount-1)) );
        }
	}
	
	//TamlWrite( %composite, "^MelvToy/composite.xml" );
	//%newComposite = TamlRead( "^MelvToy/composite.xml" );
	//SandboxScene.add( %newComposite );
}

//-----------------------------------------------------------------------------

function CompositeSprite::onCustomLayout( %this, %args )
{
    %x = getWord( %args, 0 );
    %y = getWord( %args, 1 );
    
    return (%x * 10) SPC (%y * 10);
}