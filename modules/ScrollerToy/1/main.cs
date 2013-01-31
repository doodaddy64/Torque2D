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

function ScrollerToy::create( %this )
{
    // Activate the package.
    activatePackage( ScrollerToyPackage );  
    
    // Reset the toy.
    ScrollerToy.reset();
}

//-----------------------------------------------------------------------------

function ScrollerToy::destroy( %this )
{
    // Deactivate the package.
    deactivatePackage( ScrollerToyPackage );    
}

//-----------------------------------------------------------------------------

function ScrollerToy::reset( %this )
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Create some scrollers.
    %this.createBackgroundScroller();   
    %this.createFarScroller();
    %this.createNearScroller();
}

//-----------------------------------------------------------------------------

function ScrollerToy::createBackgroundScroller( %this )
{    
    // Create the scroller.
    %object = new Scroller();
    
    // Note this scroller for the touch controls.
    ScrollerToy.BackgroundScroller = %object;
    
    // Always try to configure a scene-object prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "0 0";

    // Set the size.        
    %object.Size = "100 75";

    // Set to the furthest background layer.
    %object.SceneLayer = 31;
    
    // Set the scroller to use an animation!
    %object.Animation = "ToyAssets:TileAnimation";
    
    // Set the scroller moving in the X axis.
    %object.ScrollX = 10;
    
    // Set the scroller repeat the animation across the background.
    %object.RepeatX = 2;
    %object.RepeatY = 2;
        
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}


//-----------------------------------------------------------------------------

function ScrollerToy::createFarScroller( %this )
{    
    // Create the scroller.
    %object = new Scroller();
    
    // Note this scroller for the touch controls.
    ScrollerToy.FarScroller = %object;
    
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
    %object.ScrollX = 25;
    
    // Set the scroller to only show half of the static image in the X axis.
    %object.RepeatX = 0.5;
        
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}

//-----------------------------------------------------------------------------

function ScrollerToy::createNearScroller( %this )
{    
    // Create the scroller.
    %object = new Scroller();

    // Note this scroller for the touch controls.
    ScrollerToy.NearScroller = %object;    
    
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

package ScrollerToyPackage
{

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{   
    // Set the scrollers speed to be the distance from the background scrollers origin.
    // Also use the sign to control the direction of scrolling.
    %scrollerSpeed = getWord(%worldPos,0) - ScrollerToy.BackgroundScroller.getPositionX();

    // Set the scroller speeds.
    ScrollerToy.BackgroundScroller.ScrollX = %scrollerSpeed * 0.5;
    ScrollerToy.FarScroller.ScrollX = %scrollerSpeed;
    ScrollerToy.NearScroller.ScrollX = %scrollerSpeed * 1.5;
}
    
};

