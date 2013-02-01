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

function CompositeSpriteToy::createIsoLayout( %this )
{       
    // Create the composite sprite.   
	%composite = new CompositeSprite();
	
    // Set the default sprite stride.
    // This is used in isometrics layout mode to scale the specified logical position arguments.	
    %composite.setDefaultSpriteStride( 4, 2 );
    
    // Set the default sprite size used.
    %composite.setDefaultSpriteSize( 8 );    
    
	// Set the batch layout mode.  We must do this before we add any sprites.
    %composite.SetBatchLayout( "iso" );

    // Set the batch to render in an isolated way.  This allows the composite sprite to specify
    // the sort mode for the sprites.  All the standard scene-layer sort modes are available.    
	%composite.SetBatchIsolated( "true" );

    // The batch is in "isolated" mode so we can specify a render sort mode.
	%composite.SetBatchSortMode( "-Y" );
    	
	// Add some sprites.
    for ( %y = -5; %y <= 5; %y++ )
	{
	    for ( %x = -5; %x <= 5; %x++ )
        {
            // Add a sprite with the specified logical position.
	        // In isometric layout this two-part position is scaled by the default sprite-stride.            
            %composite.addSprite( %x SPC %y );
            
            // Set the sprite image with a random frame.
            // We could also use an animation here.            
            %composite.setSpriteImage( "CompositeSpriteToy:isoTiles", getRandom(0,4) );
        }
	}
	
	// Add to the scene.
	SandboxScene.add( %composite );
	
	// Set the composite sprite toy.
	CompositeSpriteToy.CompositeSprite = %composite;	
}