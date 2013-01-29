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

function createSimpleScrollerToy( %scopeSet )
{
    // Create some simple scrollers.
    createSimpleScrollerFar();
    createSimpleScrollerNear();
}

//-----------------------------------------------------------------------------

function createSimpleScrollerFar()
{    
    // Create the scroller.
    %object = new Scroller();
    
    // Always try to configure a scene-object prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "0 -20";

    // Set the size.        
    %object.Size = "100 35";

    // Set to the furthest background layer.
    %object.SceneLayer = 31;
    
    // Set the scroller to use a static image.
    %object.Image = "ToyAssets:LandscapeFar";
    
    // We don't really need to do this as the frame is set to zero by default.
    %object.Frame = 0;

    // Set the scroller moving in the X axis.
    %object.ScrollX = 30;
    
    // Set the scroller to only show half of the static image in the X axis.
    %object.RepeatX = 0.5;
        
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}

//-----------------------------------------------------------------------------

function createSimpleScrollerNear()
{    
    // Create the scroller.
    %object = new Scroller();
    
    // Always try to configure a scene-object prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "0 -30";

    // Set the size.        
    %object.Size = "100 20";
    
    // Set to the furthest background layer.
    %object.SceneLayer = 31;
    
    // Set the scroller to use a static image.
    %object.Image = "ToyAssets:LandscapeNear";
    
    // We don't really need to do this as the frame is set to zero by default.
    %object.Frame = 0;
    
    // Set the scroller moving in the X axis.
    %object.ScrollX = 40;

    // Set the scroller to only show half of the static image in the X axis.
    %object.RepeatX = 0.5;
        
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}
//-----------------------------------------------------------------------------

function destroySimpleScrollerToy( %scopeSet )
{
}
