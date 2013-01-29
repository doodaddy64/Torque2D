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

function createRotateToToy( %scopeSet )
{
    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", false );
    setSandboxDragModeAvailable( "zoom", false );
    
    // Set drag mode to off.
    setSandboxDragMode( "off" );

    // Create the test sprite.
    createRotateToSprite();
    
    // Activate the package.
    activatePackage( RotateToToyPackage );    
}

//-----------------------------------------------------------------------------

function createRotateToSprite()
{
    $RotateToSprite = new Sprite();
    $RotateToSprite.Image = "ToyAssets:Ship";
    $RotateToSprite.Size = 60;
    SandboxScene.add( $RotateToSprite );
}

//-----------------------------------------------------------------------------

function destroyRotateToToy( %scopeSet )
{
    // Deactivate the package.
    deactivatePackage( RotateToToyPackage );
}

//-----------------------------------------------------------------------------

package RotateToToyPackage
{

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Calculate the angle to the mouse.
    %origin = $RotateToSprite.getPosition();
    %angle = -mRadToDeg( mAtan( getWord(%worldPos,0)-getWord(%origin,0), getWord(%worldPos,1)-getWord(%origin,1) ) );
    
    // Move to the touched position.
    $RotateToSprite.RotateTo( %angle );
}
    
};
