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

function createAquariumToy( %scopeSet )
{
    exec("./scripts/aquarium.cs");

    %scopeSet.createFishScheduleId = "";
    %scopeSet.maxFish = 10;
    %scopeSet.currentFish = 0;
    %scopeSet.selectedAnimation = "AquariumToy:angelfish1Anim";

//    addIntegerOption("Number of fish", "10 40", "35 25", true, "setMaxFish", %scopeSet.maxFish);
//    addButtonOption("Reset?", "10 75", "50 25", false, "reset");
//    addSelectionOption(getFishAnimationList(), "Fish Animation", "10 110", "165 25", false, "setSelectedAnimation");
//    addButtonOption("Spawn fish", "10 145", "70 25", false, "spawnOneFish");

    addRangeOption( "Test Range", "10 40", "100 25", "0 10", "10", false, setFishRange, 1);

    // Reset the toy initially.
    //%scopeSet.reset();
}

function AquariumToy::setFishRange(%this, %value)
{
    echo("@@@ Range: " @ %value);
}
//-----------------------------------------------------------------------------

function AquariumToy::setSelectedAnimation(%this, %value)
{
    echo("New animation: " @ %value);
    %this.selectedAnimation = %value;
}

//-----------------------------------------------------------------------------

function AquariumToy::spawnOneFish(%this)
{
    %position = getRandom(-55, 55) SPC getRandom(-20, 20);

    %fishSize = getFishSize(%this.selectedAnimation);

    %fish = new Sprite()
    {
        Animation = %this.selectedAnimation;
        class = "FishClass";
        position = %position;
        size = %fishSize;
        SceneLayer = "2";
        SceneGroup = "14";
        minSpeed = "5";
        maxSpeed = "15";
        CollisionCallback = true;
    };

    %fish.createPolygonBoxCollisionShape( 15, 15);
    %fish.setCollisionGroups( 15 );
    %fish.setDefaultDensity( 1 );
    %fish.setDefaultFriction( 1.0 );
    SandboxScene.add( %fish );
}

//-----------------------------------------------------------------------------

function AquariumToy::reset(%this)
{
    // Clear the scene.
    SandboxScene.clear();

    SandboxScene.setGravity(0, 0);

    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", false );
    setSandboxDragModeAvailable( "zoom", false );

    buildAquarium();
    createAquariumEffects();

    // Reset the ball count.
    %this.currentFish = 0;

    // Cancel any pending events.
    AquariumToy::cancelPendingEvents();

    // Schedule to create a ball.
    %this.createFishScheduleId = %this.schedule( 100, "spawnFish" );
}

//-----------------------------------------------------------------------------

function AquariumToy::setMaxFish(%this, %value)
{
    %this.maxFish = %value;
}

//-----------------------------------------------------------------------------

function AquariumToy::cancelPendingEvents(%this)
{
    // Finish if there are not pending events.
    if ( !isEventPending(%this.createFishScheduleId) )
        return;

    // Cancel it.
    cancel(%this.createFishScheduleId);
    %this.createFishScheduleId = "";
}

//-----------------------------------------------------------------------------

function AquariumToy::spawnFish(%this)
{
    // Reset the event schedule.
    %this.createFishScheduleId = "";

    %position = getRandom(-55, 55) SPC getRandom(-20, 20);
    %index = getRandom(0, 5);
    %anim = getWord(getFishAnimationList(), %index);

    %fishInfo = getFishSize(%anim);

    %fish = new Sprite()
    {
        Animation = %anim;
        class = "FishClass";
        position = %position;
        size = %fishInfo;
        SceneLayer = "2";
        SceneGroup = "14";
        minSpeed = "5";
        maxSpeed = "15";
        CollisionCallback = true;
    };

    %fish.createPolygonBoxCollisionShape( 15, 15);
    %fish.setCollisionGroups( 15 );
    %fish.setDefaultDensity( 1 );
    %fish.setDefaultFriction( 1.0 );
    SandboxScene.add( %fish );

    %this.currentFish++;

    // Schedule to spawn a fish.
    if ( %this.currentFish < %this.maxFish)
        %this.createFishScheduleId = %this.schedule( 100, "spawnFish" );
}

//-----------------------------------------------------------------------------

function destroyAquariumToy( %scopeSet )
{
    // Cancel any pending events.
    AquariumToy::cancelPendingEvents();
}