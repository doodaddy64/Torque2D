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

function PyramidToy::create( %this )
{
    // Set the sandbox drag mode availability.
    Sandbox.allowManipulation( pan );
    Sandbox.allowManipulation( pull );
    
    // Set the manipulation mode.
    Sandbox.useManipulation( pull );
    
    // Configure the toy.
    PyramidToy.BrickCount = 15;
    PyramidToy.GroundWidth = 150;
    
    // Set the camera.
    SandboxWindow.setCurrentCameraArea("-15 -15 15 15");
    
    // Se the gravity.
    SandboxScene.setGravity( 0, -15 );
        
    // Set the sandbox drag mode.
    Sandbox.useManipulation( "pull" ); 
    
    // Reset the toy.
    PyramidToy.reset();
}

//-----------------------------------------------------------------------------

function PyramidToy::destroy( %this )
{
}

//-----------------------------------------------------------------------------

function PyramidToy::reset( %this )
{
    // Clear the scene.
    SandboxScene.clear();
        
    // Create the pyramid ground.
    %this.createPyramidGround();
    
    // Create the pyramid.
    %this.createPyramid(-15, -8, PyramidToy.BrickCount);    
}

//-----------------------------------------------------------------------------

function PyramidToy::createPyramidGround( %this )
{
    %ground = new Scroller();
    %ground.setBodyType( "static" );
    %ground.Image = "ToyAssets:woodGround";
    %ground.setSize(PyramidToy.GroundWidth, 2);
    %ground.setRepeatX( PyramidToy.GroundWidth / 12 );   
    %ground.setPosition(0, -10);
    %ground.createEdgeCollisionShape( PyramidToy.GroundWidth/-2, 1, PyramidToy.GroundWidth/2, 1 );
    SandboxScene.add( %ground );
}

//-----------------------------------------------------------------------------

function PyramidToy::createPyramid( %this, %posX, %posY, %brickBaseCount )
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
            %crateX = %stackX + (%stackIndex*1.5);
            %crateY = %stackY;

            %obj = new Sprite();
            %obj.setUseInputEvents(true);
            %obj.setImage( "ToyAssets:crate" );
            %obj.setPosition( %crateX, %crateY );
            %obj.setSize( 1.5 );
            %obj.setDefaultFriction( 1.0 );
            %obj.createPolygonBoxCollisionShape( 1.5, 1.5 );
            SandboxScene.add( %obj );          
        }
    }
}
