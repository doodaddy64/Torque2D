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

function SceneLayerToy::create( %this )
{
    // Reset the toy.
    SceneLayerToy.reset();
}


//-----------------------------------------------------------------------------

function SceneLayerToy::destroy( %this )
{
}

//-----------------------------------------------------------------------------

function SceneLayerToy::reset( %this )
{
    // Clear the scene.
    SandboxScene.clear();

    // Add a sprite to all scene layers.
    for( %layer = 0; %layer <= 31; %layer++ )
    {
        // Create the sprite.
        %object = new Sprite();

        // Set the position so each layer is in a different Y position.
        %object.SetPosition( -32 + (%layer * 2), -32 + (%layer * 2) );
        
        // Set the size.
        %object.Size = "20 10";
        
        // Set the scene layer.
        %object.SceneLayer = %layer;
        
        // Set the image.
        %object.Image = "ToyAssets:Blocks";
        
        // Set the frame to be the layer number.
        %object.Frame = %layer;
        
        // Set the object randomly spinning to make it more interesting.
        %object.AngularVelocity = getRandom(-180,180);
        
        // Add the object to the scene.
        SandboxScene.add( %object );
    }  
}

