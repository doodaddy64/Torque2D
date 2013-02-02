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

// Create this behavior only if it does not already exist
if (!isObject(BallBehavior))
{
   // Create this behavior from the blank BehaviorTemplate
   // Name it BallBehavior
   %template = new BehaviorTemplate(BallBehavior);
   
   %template.addBehaviorField(acceleration, "Base acceleration (world units per second)", float, 10.0);
   %template.addBehaviorField(turnSpeed, "Velocity of turning (degrees per second)", float, 70.0);
   %template.addBehaviorField(damping, "Amount to damp movement (percentage of velocity per second)", float, 1.0);
   
   %template.addBehaviorField(RollHorizontal, "Motion input for rolling the ball vertically", motion, "accelerometer gravityx");
   %template.addBehaviorField(RollVertical, "Motion input for rolling the ball vertically", motion, "accelerometer gravityy");
   
   %template.addBehaviorField(upKey, "Motion input for rolling the ball vertically", keybind, "keyboard up");
   %template.addBehaviorField(downKey, "Motion input for rolling the ball vertically", keybind, "keyboard down");
   %template.addBehaviorField(leftKey, "Motion input for rolling the ball vertically", keybind, "keyboard left");
   %template.addBehaviorField(rightKey, "Motion input for rolling the ball vertically", keybind, "keyboard right");
}

//-----------------------------------------------------------------------------

function BallBehavior::initialize(%this)
{
   %this.owner.pauseAnimation(1);
   %this.owner.setDamping(%this.damping);

   %this.rollSchedule = %this.schedule(100, updateRollAnimation);
}

//-----------------------------------------------------------------------------

function BallBehavior::updateRollAnimation(%this)
{
    %velocity = %this.owner.getLinearVelocity();

    %scaledVelocity = (mAbs(%velocity.x) + mAbs(%velocity.y)) / 50;
    %flooredVelocity = mFloatLength(%scaledVelocity, 1);
    %scaledAnimTime = %currentAnimTime * %flooredVelocity;

    %this.owner.setSpeedFactor(%scaledAnimTime);

    %this.rollSchedule = %this.schedule(100, updateDesktopRoll);
}

//-----------------------------------------------------------------------------

function BallBehavior::onBehaviorRemove(%this)
{
    if ( isEventPending(%this.rollSchedule) )
        cancel(%this.rollSchedule);
}