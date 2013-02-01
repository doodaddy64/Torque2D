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

function BridgeToy::create(%this)
{
    // Set the sandbox drag mode availability.
    Sandbox.allowManipulation( pan );
    Sandbox.allowManipulation( pull );
    
    // Set the manipulation mode.
    Sandbox.useManipulation( pull );
    
    BridgeToy.GroundWidth = 150;
    BridgeToy.maxProps = 3;
    
    addNumericOption("Number of props", 0, 10, 1, "setMaxProps", BridgeToy.maxProps, true);
    
    // Reset the toy initially.
    BridgeToy.reset();
}

//-----------------------------------------------------------------------------

function BridgeToy::destroy(%this)
{   
       
}

//-----------------------------------------------------------------------------

function BridgeToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Zoom the camera in    
    SandboxWindow.setCurrentCameraZoom(2);
    
    // Prefer the collision option off as it severely affects the performance.
    setCollisionOption(false);
    
    // Set the scene gravity.
    SandboxScene.setGravity(0, -20);
    
    // Create the ground
    %ground = new Scroller();
    %ground.setBodyType("static");
    %ground.Image = "ToyAssets:woodGround";
    %ground.setSize(BridgeToy.GroundWidth, 2);
    %ground.setRepeatX(BridgeToy.GroundWidth / 12);   
    %ground.setPosition(0, -10);
    %ground.createEdgeCollisionShape(BridgeToy.GroundWidth/-2, 1, BridgeToy.GroundWidth/2, 1);
    SandboxScene.add(%ground);
    
    // Create the left side of the bridge
    %this.createBase(-12, -6.5, -90);
    
    // Create the right side of the bridge
    %this.createBase(11.5, -6.5, -90);
    
    // Create the links for the bridge
    %this.createBridge(-11.5, -4.1, 45);
    
    // Add some props
    %this.createProps();
}

//-----------------------------------------------------------------------------

function BridgeToy::createBase(%this, %posX, %posY, %angle)
{
    %image = "ToyAssets:woodGround";

    %obj = new Sprite();   
    %obj.setBodyType("static");
    %obj.setImage("ToyAssets:woodGround");
    %obj.setSize(5, 0.8);   
    %obj.createPolygonBoxCollisionShape(5, 0.8);
    %obj.setPosition(%posX, %posY);
    %obj.setAngle(%angle);    
    %obj.setAwake(false);
    %obj.setDefaultFriction(1.0);
    
    SandboxScene.add(%obj);
}

//-----------------------------------------------------------------------------

function BridgeToy::createBridge(%this, %posX, %posY, %linkCount)
{
    %linkWidth = 0.5;
    %linkHeight = %linkWidth * 0.5;
    %halfLinkWidth = %linkWidth * 0.5;

    %rootObj = new Sprite();
    %rootObj.setBodyType("static");
    %rootObj.setImage("ToyAssets:cable");
    %rootObj.setPosition(%posX, %posY);
    %rootObj.setSize(%linkWidth, %linkHeight);
    %rootObj.setCollisionSuppress();
    SandboxScene.add(%rootObj);

    %lastLinkObj = %rootObj;

    for (%n = 1; %n <= %linkCount; %n++)
    {
        %obj = new Sprite();

        %obj.setImage("ToyAssets:cable");
        %obj.setPosition(%posX + (%n*%linkWidth), %posY);
        %obj.setSize(%linkWidth, %linkHeight);
        
        if (%n == %linkCount)
        {
            %obj.setBodyType("static");
            %obj.setCollisionSuppress();
        }
        else
        {      
            %obj.setDefaultDensity(1);
            %obj.setDefaultFriction(1.0);
            %obj.createPolygonBoxCollisionShape(%linkWidth, %linkHeight);
            %obj.setAngularDamping(1.0);
            %obj.setLinearDamping(1.0);
        }
        
        SandboxScene.add(%obj);   

        SandboxScene.createRevoluteJoint(%lastLinkObj, %obj, %halfLinkWidth, 0, -%halfLinkWidth, 0);
        %joint = SandboxScene.createMotorJoint(%lastLinkObj, %obj);
        SandboxScene.setMotorJointMaxForce(%joint, 500);
        %obj.setAwake(false);
        %lastLinkObj.setAwake(false);
        
        %lastLinkObj = %obj;
    }
}

//-----------------------------------------------------------------------------

function BridgeToy::setMaxProps(%this, %value)
{
    %this.maxProps = %value;
}

function BridgeToy::createProps(%this)
{
    for (%image = 0; %i < %this.maxProps; %i++)
    {
        %randomPosition = getRandom(-10, 10) SPC getRandom(-2, 8);
        %randomBrick = getRandom(1, 5);
        
        %image = "ToyAssets:brick_0" @ %randomBrick;
        
        %obj = new Sprite();   
        
        %obj.setImage(%image);
        %obj.setPosition(%randomPosition);
        %obj.setSize(1, 0.5);
        %obj.setDefaultFriction(1.0);
        %obj.createPolygonBoxCollisionShape(1, 0.5);
        %obj.setAwake(true);
        SandboxScene.add(%obj);
    }
}