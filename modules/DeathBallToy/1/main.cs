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
    // DealsDamageBehavior
    // TakesDamageBehavior
    // MoveTowardBehavior
    // FaceObjectBehavior
    // SpawnAreaBehavior

    // Activate the package.
    activatePackage( DeathBallToyPackage );

    // Initialize the toys settings.
    DeathBallToy.rotateTime = 100;
    DeathBallToy.moveTime = 3000;
    DeathBallToy.soldierSpeed = 10;
    DeathBallToy.spawnPointCount = 4;
    DeathballToy.spawnAmount = 40;
    
    // Add the custom controls.
    addNumericOption("Deathball turn speed", 10, 100, 10, "setRotateTime", DeathBallToy.rotateTime, false);
    addNumericOption("Deathball move speed", 10, 3000, 10, "setMoveTime", DeathBallToy.moveTime, false);
    addNumericOption("Number of spawnpoints", 4, 4, 1, "setSpawnPointCount", DeathBallToy.spawnPointCount, true);
    addNumericOption("Soldier count", 40, 100, 10, "setSpawnAmount", DeathBallToy.spawnAmount, true);
    addNumericOption("Soldier speed", 1, 10, 1, "setSoldierSpeed", DeathBallToy.soldierSpeed, false);
    
    // Reset the toy initially.
    DeathBallToy.reset();
}

//-----------------------------------------------------------------------------

function DeathBallToy::destroy( %this )
{
    // Cancel any pending events.
    DeathBallToy::cancelPendingEvents();

    // Deactivate the package.
    deactivatePackage( DeathBallToyPackage );
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

function DeathBallToy::setRotateTime(%this, %value)
{
    %this.rotateTime = %value;
}

//-----------------------------------------------------------------------------

function DeathBallToy::setMoveTime(%this, %value)
{
    %this.moveTime = %value;
}

//-----------------------------------------------------------------------------

function DeathBallToy::setSpawnPointCount(%this, %value)
{
    %this.spawnPointCount = %value;
}

//-----------------------------------------------------------------------------

function DeathBallToy::setSpawnAmount(%this, %value)
{
    %this.spawnAmount = %value;
}

//-----------------------------------------------------------------------------

function DeathBallToy::setSoldierSpeed(%this, %value)
{
    %this.soldierSpeed = %value;
}

//-----------------------------------------------------------------------------

function DeathBallToy::cancelPendingEvents()
{
    // Finish if there are not pending events.
    if ( !isEventPending(DeathBall.rollSchedule) )
        return;

    cancel(DeathBall.rollSchedule);
    DeathBall.rollSchedule = "";
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

    %currentAnimTime = Deathball.getAnimationTime();
    echo("currentAnimTime: " @ %currentAnimTime);

    //%db.pauseAnimation(1);

    Deathball.rollSchedule = Deathball.schedule(100, "updateRollAnimation");

    SandboxScene.add(%db);

    SandboxWindow.mount( Deathball, "0 0", 0, true, false );
}

function Deathball::updateRollAnimation(%this)
{
    %this.rollSchedule = "";

    %velocity = %this.getLinearVelocity();

    %currentAnimTime = %this.getAnimationTime();
    %scaledVelocity = (mAbs(getWord(%velocity, 0))) + mAbs(getWord(%velocity, 1)) / 50;
    %flooredVelocity = mFloatLength(%scaledVelocity, 1);
    %scaledAnimTime = %currentAnimTime * %flooredVelocity;

    %this.setAnimationTimeScale(%scaledAnimTime);

    %this.rollSchedule = %this.schedule(100, updateRollAnimation);
}

package DeathBallToyPackage
{

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    %origin = Deathball.getPosition();
    %angle = -mRadToDeg( mAtan( getWord(%worldPos,0)-getWord(%origin,0), getWord(%worldPos,1)-getWord(%origin,1) ) );

    //Rotate to the touched angle.
    Deathball.RotateTo( %angle, DeathBallToy.rotateTime );

    // Move to the touched position.
    Deathball.moveTo( %worldPos, DeathBallToy.moveTime );
}

};