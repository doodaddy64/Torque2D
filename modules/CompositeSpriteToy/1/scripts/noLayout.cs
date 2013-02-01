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

function CompositeSpriteToy::createNoLayout( %this )
{
    // Create the composite sprite.
	%composite = new CompositeSprite();
	   
	// Set the batch layout mode.  We must do this before we add any sprites.
    %composite.SetBatchLayout( "off" );
	
    // Add some sprites.
	for( %n = 0; %n < 50; %n++ )
	{
        // Add a sprite with no logical position.
        %composite.addSprite();
        
        // Set the sprites location position to a random location.
        %composite.setSpriteLocalPosition( getRandom(-40,40), getRandom(-30,30) );
        
        // Set a random angle.
        %composite.setSpriteAngle( getRandom(0,360) );
        
        // Set a random size.
        %composite.setSpriteSize( getRandom(5,10), getRandom(5,10) );

        // Set the sprite image with a random frame.
        // We could also use an animation here.
        %composite.setSpriteImage( "ToyAssets:Tiles", getRandom(0,15) );            
        
        // Set the sprite spinning to make it more interesting.
        %composite.setAngularVelocity( CompositeSpriteToy.AngularVelocity );
	}  
	
	// Add to the scene.
	SandboxScene.add( %composite );
	
	// Set the composite sprite toy.
	CompositeSpriteToy.CompositeSprite = %composite;
}
