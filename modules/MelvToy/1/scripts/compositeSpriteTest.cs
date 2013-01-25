//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runCompositeSpriteTest()
{
    //createRectSprite();
    createIsoSprite();
}

function createRectSprite()
{
    // Fetch the stock color count.
    %stockColorCount = getStockColorCount();
        
	%composite = new CompositeSprite();
	SandboxScene.add( %composite );
	
    %composite.setDefaultSpriteStride( 10, 10 );
    %composite.setDefaultSpriteSize( 10 );    
    %composite.BatchLayout = "off";
	%composite.BatchIsolated = "true";    
	%composite.BatchSortMode = "-Y";
    //%composite.setAngularVelocity( -10 );
	
    for ( %y = -10; %y <= 10; %y++ )
	{
	    for ( %x = -10; %x <= 10; %x++ )
        {
                
            %composite.addSprite( getRandom(-50,50) SPC getRandom(-37,37) );
            %composite.setSpriteImage( "MelvToy:MiniTileMapImage", getRandom(0,15) );
            //%composite.setSpriteName( "My Name is" SPC %x SPC %y );
            %composite.setSpriteAngle( getRandom(0,360) );
            %composite.setSpriteSize( getRandom(5,10), getRandom(5,10) );
            //%composite.setSpriteRenderGroup( "melv" );
            //%composite.setSpriteBlendColor( getStockColorName(getRandom(0,%stockColorCount-1)) );
        }
	}
	
	//TamlWrite( %composite, "^MelvToy/composite.xml" );
	//%newComposite = TamlRead( "^MelvToy/composite.xml" );
	//SandboxScene.add( %newComposite );    
}

//-----------------------------------------------------------------------------

function createIsoSprite()
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
	
    for ( %y = -5; %y <= 5; %y++ )
	{
	    for ( %x = -5; %x <= 5; %x++ )
        {
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