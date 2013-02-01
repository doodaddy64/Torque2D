//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

function MelvToy::compositeSpriteTest( %this )
{
    MelvToy.createOffSprite();
    //MelvToy.createRectSprite();
    //MelvToy.createIsoSprite();
    //MelvToy.createCustomSprite();
    
	//TamlWrite( SandboxScene.CompositeSprite, "^MelvToy/composite.xml" );
	//SandboxScene.CompositeSprite.delete();
	//%newComposite = TamlRead( "^MelvToy/composite.xml" );
	//SandboxScene.add( %newComposite );     
}

//-----------------------------------------------------------------------------

function MelvToy::createOffSprite( %this )
{
	%composite = new CompositeSprite();
	
    %composite.setDefaultSpriteStride( 10, 10 );
    %composite.setDefaultSpriteSize( 10 );    
    %composite.BatchLayout = "off";
	%composite.BatchIsolated = "true";    
	
    for ( %y = -10; %y <= 10; %y++ )
	{
	    for ( %x = -10; %x <= 10; %x++ )
        {            
            %composite.addSprite();
            %composite.setSpriteLocalPosition( getRandom(-50,50), getRandom(-37,37) );
            %composite.setSpriteImage( "ToyAssets:Tiles", getRandom(0,15) );
            %composite.setSpriteAngle( getRandom(0,360) );
            %composite.setSpriteSize( getRandom(5,10), getRandom(5,10) );
        }
	}  
	SandboxScene.add( %composite );
	SandboxScene.CompositeSprite = %composite;
}

//-----------------------------------------------------------------------------

function MelvToy::createRectSprite( %this )
{
	%composite = new CompositeSprite();
	
    %composite.setDefaultSpriteStride( 10, 10 );
    %composite.setDefaultSpriteSize( 10 );    
    %composite.BatchLayout = "rect";
	%composite.BatchIsolated = "true";    
	
    for ( %y = -10; %y <= 10; %y++ )
	{
	    for ( %x = -10; %x <= 10; %x++ )
        {                
            %composite.addSprite( %x SPC %y );
            %composite.setSpriteImage( "ToyAssets:Tiles", getRandom(0,15) );
            %composite.setSpriteAngle( getRandom(0,360) );
            %composite.setSpriteSize( getRandom(5,10), getRandom(5,10) );
        }
	}
	SandboxScene.add( %composite );
	SandboxScene.CompositeSprite = %composite;
}

//-----------------------------------------------------------------------------

function MelvToy::createIsoSprite( %this )
{       
	%composite = new CompositeSprite();
	
    %composite.setDefaultSpriteStride( 25, 25 );
    %composite.setDefaultSpriteSize( 20 );    
    %composite.BatchLayout = "iso";
	%composite.BatchIsolated = "true";    
	%composite.BatchSortMode = "-Y";
    %composite.DefaultSpriteSize = 8;
    %composite.DefaultSpriteStride = "4 2";
	
    for ( %y = -5; %y <= 5; %y++ )
	{
	    for ( %x = -5; %x <= 5; %x++ )
        {
            %composite.addSprite( %x SPC %y );
            %composite.setSpriteImage( "MelvToy:isoTiles2", getRandom(0,4) );
        }
	}
	
	SandboxScene.add( %composite );
	SandboxScene.CompositeSprite = %composite;
}

//-----------------------------------------------------------------------------

function MelvToy::createCustomSprite( %this )
{
	%composite = new CompositeSprite();
	
    %composite.setDefaultSpriteStride( 10, 10 );
    %composite.setDefaultSpriteSize( 10 );    
    %composite.BatchLayout = "custom";
	%composite.BatchIsolated = "true";    
	
    for ( %y = -10; %y <= 10; %y++ )
	{
	    for ( %x = -10; %x <= 10; %x++ )
        {                
            %composite.addSprite(%x SPC %y);
            %composite.setSpriteImage( "ToyAssets:Tiles", getRandom(0,15) );
            %composite.setSpriteAngle( getRandom(0,360) );
            %composite.setSpriteSize( getRandom(5,10), getRandom(5,10) );
        }
	}
	SandboxScene.add( %composite );
	SandboxScene.CompositeSprite = %composite;
}

//-----------------------------------------------------------------------------

function CompositeSprite::onCustomLayout( %this, %args )
{    
    if ( getWordCount(%args) == 0 )
        return;

    %x = getWord( %args, 0 );
    %y = getWord( %args, 1 );
    
    return (%x * 10) SPC (%y * 10);
}