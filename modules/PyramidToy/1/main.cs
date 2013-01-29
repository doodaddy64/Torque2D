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

function createPyramidToy( %scopeSet )
{
    $PyramidBrickCount = 15;
    $GroundWidth = 150;
    
    SandboxWindow.setCurrentCameraZoom( 2 );
    SandboxScene.setGravity( 0, -15 );
    
    createPyramidGround();
    createToyPyramid(-15, -8, $PyramidBrickCount);
    
    // Set the sandbox drag mode.
    setSandboxDragMode( "pull" ); 
}

//-----------------------------------------------------------------------------

function destroyPyramidToy( %scopeSet )
{
}

//-----------------------------------------------------------------------------

function createPyramidGround()
{
    %ground = new Scroller();
    %ground.setBodyType( "static" );
    %ground.Image = "ToyAssets:woodGround";
    %ground.setSize($GroundWidth, 2);
    %ground.setRepeatX( $GroundWidth / 12 );   
    %ground.setPosition(0, -10);
    %ground.createEdgeCollisionShape( $GroundWidth/-2, 1, $GroundWidth/2, 1 );
    SandboxScene.add( %ground );
}

//-----------------------------------------------------------------------------

function createToyPyramid( %posX, %posY, %brickBaseCount)
{
   if ( %brickBaseCount < 2 )
   {
      echo( "Invalid pyramid brick base count of" SPC %brickBaseCount );
      return;
   }
   
   for( %stack = 0; %stack < %brickBaseCount; %stack++ )
   {
      %stackIndexCount = %brickBaseCount - (%stack*2);
      %stackX = %posX + ( %stack * 1.5 );
      %stackY = %posY + ( %stack * 1.5 );
      for ( %stackIndex = 0; %stackIndex < %stackIndexCount; %stackIndex++ )
      {
         createCrate( %stackX + (%stackIndex*1.5), %stackY);
      }
   }
}

//-----------------------------------------------------------------------------

function createCrate( %posX, %posY)
{
   %imageMap = "ToyAssets:crate";
   %obj = new Sprite();
   %obj.setUseInputEvents(true);
   %obj.setImage( %imageMap );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 1.5 );
   %obj.setDefaultFriction( 1.0 );
   %obj.createPolygonBoxCollisionShape( 1.5, 1.5 );
   SandboxScene.add( %obj );
}
