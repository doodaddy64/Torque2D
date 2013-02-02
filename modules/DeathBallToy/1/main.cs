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

function DeathBallToy::create( %this )
{
    // Reset the toy initially.
    DeathBallToy.reset();
}

//-----------------------------------------------------------------------------

function DeathBallToy::destroy( %this )
{   
       
}

//-----------------------------------------------------------------------------

function DeathBallToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();

    // Set the gravity.
    SandboxScene.setGravity(0, 0);

    // Add backgrounds
    %this.createDesertBackgrounds();

    // Add deathball
    %this.spawnDeathball("0 0");

    // Start spawning soldiers

    // GET 'EM!
}

//-----------------------------------------------------------------------------

function DeathBallToy::createDesertBackgrounds(%this)
{
    // Create the composite sprite.
    %composite = new CompositeSprite();

    %composite.setSceneLayer(3);

    // Set the batch layout mode.  We must do this before we add any sprites.
    %composite.SetBatchLayout( "off" );

    // Add upper left desert
    %composite.addSprite();
    %composite.setSpriteLocalPosition(0, 0);
    %composite.setSpriteSize(100, 75);
    %composite.setSpriteImage( "DeathBallToy:upperLeftDesert");

    // Add upper right desert
    %composite.addSprite();
    %composite.setSpriteLocalPosition(100, 0);
    %composite.setSpriteSize(100, 75);
    %composite.setSpriteImage( "DeathBallToy:upperRightDesert");

    // Add lower left desert
    %composite.addSprite();
    %composite.setSpriteLocalPosition(0, -75);
    %composite.setSpriteSize(100, 75);
    %composite.setSpriteImage( "DeathBallToy:lowerLeftDesert");

    // Add lower right desert
    %composite.addSprite();
    %composite.setSpriteLocalPosition(100, -75);
    %composite.setSpriteSize(100, 75);
    %composite.setSpriteImage( "DeathBallToy:lowerRightDesert");

    // Add to the scene.
    SandboxScene.add( %composite );
}

//-----------------------------------------------------------------------------
function DeathBallToy::spawnDeathball(%this, %position)
{
    %db = new Sprite(Deathball)
    {
        Animation = "DeathBallToy:dbForwardAnim";
        position = %position;
        size = "20 20";
        SceneLayer = "1";
        SceneGroup = "14";
        minSpeed = "5";
        maxSpeed = "15";
        CollisionCallback = true;
    };

    SandboxScene.add(%db);
}

function Deathball::updateRollAnimation(%this)
{
    %velocity = %this.owner.getLinearVelocity();

    %scaledVelocity = (mAbs(%velocity.x) + mAbs(%velocity.y)) / 50;
    %flooredVelocity = mFloatLength(%scaledVelocity, 1);
    %scaledAnimTime = %currentAnimTime * %flooredVelocity;

    %this.owner.setSpeedFactor(%scaledAnimTime);

    %this.rollSchedule = %this.schedule(100, updateDesktopRoll);
}