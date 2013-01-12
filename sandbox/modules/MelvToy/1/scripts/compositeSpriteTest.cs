//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function runCompositeSpriteTest( %scopeSet )
{
	%composite = new CompositeSprite();
	SandboxScene.addToScene( %composite );
	
    %composite.setDefaultSpriteStride( 5, 5 );
    %composite.setDefaultSpriteSize( 4 );    
    %composite.BatchLayout = "rect";
	%composite.BatchIsolated = "true";    
    %composite.setAngularVelocity( -90 );
	
    for ( %y = -5; %y <= 5; %y++ )
	{
	    for ( %x = -5; %x <= 5; %x++ )
        {
            %composite.addSprite( %x SPC %y );
            %composite.setSpriteImage( "MelvToy:MiniTileMapImage", getRandom(0,15) );
            %composite.setSpriteName( "My Name is" SPC %x SPC %y );
            %composite.setSpriteAngle( getRandom(0,360) );
        }
	}
}

//-----------------------------------------------------------------------------

function CompositeSprite::onCustomLayout( %this, %args )
{
    %x = getWord( %args, 0 );
    %y = getWord( %args, 1 );
    
    return (%x * 10) SPC (%y * 10);
}