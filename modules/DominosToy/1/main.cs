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

function DominosToy::create( %this )
{
    // Set the sandbox drag mode availability.
    Sandbox.allowManipulation( pan );
    Sandbox.allowManipulation( pull );

    // Set the manipulation mode.
    Sandbox.useManipulation( pull );

    // Sets how large the ground will be
    DominosToy.GroundWidth = 80;

    // Reset the toy initially.
    DominosToy.reset();
}

//-----------------------------------------------------------------------------

function DominosToy::destroy( %this )
{   
       
}

//-----------------------------------------------------------------------------

function DominosToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();

    // Zoom the camera in
    SandboxWindow.setCurrentCameraArea("-10 0 15 15");

    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption(false);

    // Set the scene gravity.
    SandboxScene.setGravity(0, -9.8);

    // Create the ground
    %ground = new Scroller();
    %ground.setBodyType("static");
    %ground.Image = "ToyAssets:woodGround";
    %ground.setSize(DominosToy.GroundWidth, 2);
    %ground.setRepeatX(DominosToy.GroundWidth / 12);
    %ground.setPosition(0, 0);
    %ground.createEdgeCollisionShape(DominosToy.GroundWidth/-2, 1, DominosToy.GroundWidth/2, 1);
    SandboxScene.add(%ground);

    // Create the static fixtures the dominos collide against
    %this.createPlatforms();

    // Create the dominos
    %this.createDominos();

    // Create the lever platform
    %this.createLever();

    // Create the trap at the end of the gadget
    %this.createTrap();

    // Create the swinging box that starts the dominos
    %this.createSwinger();
}

//-----------------------------------------------------------------------------

function DominosToy::createPlatforms()
{
    // Create top platform
    %obj = new Sprite();
    %obj.setBodyType("static");
    %obj.Image = "ToyAssets:woodGround";
    %obj.setSize(4, 0.25);
    %obj.setPosition(-0.5, 5);
    %obj.createPolygonBoxCollisionShape( 4, 0.25 );
    SandboxScene.add(%obj);

    // Create slanted platform
    %obj = new Sprite();
    %obj.setBodyType("static");
    %obj.Image = "ToyAssets:woodGround";
    %obj.setSize(7, 0.25);
    %obj.setPosition(1, 3.5);
    %obj.setAngle(15);
    %obj.createPolygonBoxCollisionShape( 7, 0.25 );

    SandboxScene.add(%obj);

    // Create vertical platform
    %obj = new Sprite();
    %obj.setBodyType("static");
    %obj.Image = "ToyAssets:woodGround";
    %obj.setSize(1, 0.25);
    %obj.setPosition(-3.25, 2.5);
    %obj.setAngle(90);
    %obj.createPolygonBoxCollisionShape( 1, 0.25 );

    SandboxScene.add(%obj);
}

//-----------------------------------------------------------------------------

function DominosToy::createDominos()
{
    for (%i = 0; %i < 10; %i++)
    {
        %obj = new Sprite();

        %obj.Image = "ToyAssets:woodGround";
        %obj.setSize(0.1, 0.5);
        %obj.setDefaultFriction(0.1);
        %xPos = (0.35 * %i) - 2;
        %obj.setPosition(%xPos, 5.75);

        %obj.createPolygonBoxCollisionShape( 0.1, 0.5 );
        SandboxScene.add(%obj);
    }
}

//-----------------------------------------------------------------------------

function DominosToy::createLever()
{

}

//-----------------------------------------------------------------------------

function DominosToy::createTrap()
{

}

//-----------------------------------------------------------------------------

function DominosToy::createSwinger()
{

}