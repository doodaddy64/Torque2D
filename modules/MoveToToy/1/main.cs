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

function MoveToToy::create( %this )
{
    // Activate the package.
    activatePackage( MoveToToyPackage );    

    // Initialize the toys settings.
    MoveToToy.moveTime = 1000;

    // Add the custom controls.
    addIntegerOption("Move time", 10, 1000, "setMoveTime", MoveToToy.moveTime, true);

    // Reset the toy initially.
    MoveToToy.reset();        
}

//-----------------------------------------------------------------------------

function MoveToToy::destroy( %this )
{
    // Deactivate the package.
    deactivatePackage( MoveToToyPackage );
}

//-----------------------------------------------------------------------------

function MoveToToy::reset( %this )
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Create the sprite.
    %object = new Sprite(MoveToSprite);
    
    // Set the static image.
    %object.Image = "ToyAssets:Crosshair2";
    
    // Set a useful size.
    %object.Size = 20;
    
    // Set the sprite rotating to make it more interesting.
    %object.AngularVelocity = 180;
    
    // Add to the scene.
    SandboxScene.add( %object );
}

//-----------------------------------------------------------------------------

function MoveToToy::setMoveTime( %this, %value )
{
    %this.moveTime = %value;
}

//-----------------------------------------------------------------------------

package MoveToToyPackage
{

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Move to the touched position.
    MoveToSprite.moveTo( %worldPos, MoveToToy.moveTime );
}
    
};
