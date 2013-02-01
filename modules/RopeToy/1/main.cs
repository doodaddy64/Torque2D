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

function RopeToy::create( %this )
{
    // Set the sandbox drag mode availability.
    Sandbox.allowManipulation( pan );
    Sandbox.allowManipulation( pull );
    
    // Set the manipulation mode.
    Sandbox.useManipulation( pull );   
    
    RopeToy.GroundWidth = 80;
    RopeToy.maxLinks = 16;

    // Reset the toy initially.
    RopeToy.reset();
}

//-----------------------------------------------------------------------------

function RopeToy::destroy( %this )
{   
       
}

//-----------------------------------------------------------------------------

function RopeToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Zoom the camera in    
    SandboxWindow.setCurrentCameraArea("-15 -15 15 15");
    
    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption(false);
    
    // Set the scene gravity.
    SandboxScene.setGravity(0, -9.8);
    
    // Create the ground
    %ground = new Scroller();
    %ground.setBodyType("static");
    %ground.Image = "ToyAssets:woodGround";
    %ground.setSize(RopeToy.GroundWidth, 2);
    %ground.setRepeatX(RopeToy.GroundWidth / 12);   
    %ground.setPosition(0, -10);
    %ground.createEdgeCollisionShape(RopeToy.GroundWidth/-2, 1, RopeToy.GroundWidth/2, 1);
    SandboxScene.add(%ground);
    
    // Create the chain
    %this.createRope(0, 2);
}

//-----------------------------------------------------------------------------

function RopeToy::setMaxLinks(%this, %value)
{
    %this.maxLinks = %value;
}

//-----------------------------------------------------------------------------

function RopeToy::createRope(%this, %posX, %posY)
{
    // Swinging box
    %box = new Sprite();
    %box.setUseInputEvents(true);
    %box.setImage( "ToyAssets:crate" );
    %box.setPosition( %crateX, %crateY );
    %box.setSize( 1.5 );
    %box.setDefaultFriction( 1.0 );
    %box.createPolygonBoxCollisionShape( 1.5, 1.5 );
    SandboxScene.add( %box );

    %linkWidth = 0.25;
    %linkHeight = %linkWidth * 2;
    %halfLinkHeight = %linkHeight * 0.4;

    %rootObj = new Sprite();
    %rootObj.setBodyType( "static" );
    %rootObj.setImage( "ToyAssets:cable" );
    %rootObj.setPosition( %posX, %posY );
    %rootObj.setSize( %linkWidth, %linkHeight );
    %rootObj.setCollisionSuppress();
    SandboxScene.add( %rootObj );

    %lastLinkObj = %rootObj;

    for ( %n = 1; %n <= %this.maxLinks; %n++ )
    {
        %obj = new Sprite();
        %obj.setImage( "ToyAssets:cable" );
        %obj.setPosition( %posX, %posY - (%n*%linkHeight) );
        %obj.setSize( %linkWidth, %linkHeight );
        %obj.setDefaultDensity( 0.4 );
        %obj.setDefaultFriction( 0.2 );
        %obj.createPolygonBoxCollisionShape( %linkWidth, %linkHeight );
        %obj.setAngularDamping( 0.1 );
        %obj.setLinearDamping( 0.1 );
        %obj.setUseInputEvents(true);
        SandboxScene.add( %obj );   

        //SandboxScene.createRopeJoint( %lastLinkObj, %obj, 0, -%halfLinkHeight, 0, %halfLinkHeight, 0.01, false );
        SandboxScene.createRevoluteJoint( %lastLinkObj, %obj, 0, -%halfLinkHeight, 0, %halfLinkHeight, false );

        %lastLinkObj = %obj;
    }

    SandboxScene.createRevoluteJoint( %lastLinkObj, %box, 0, -%halfLinkHeight, 0, %halfLinkHeight, false );
    SandboxScene.createRopeJoint(%box, %rootObj, 0, 0, 0, 0, 7, false);
    %lastLinkObj.setAwake(false);
}