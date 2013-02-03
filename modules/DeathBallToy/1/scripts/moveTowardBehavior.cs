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

if (!isObject(MoveTowardBehavior))
{
    %template = new BehaviorTemplate(MoveTowardBehavior);

    %template.friendlyName = "Move Toward";
    %template.behaviorType = "AI";
    %template.description  = "Set the object to move toward another object";

    %template.addBehaviorField(target, "The object to move toward", object, "", sceneObject);
    %template.addBehaviorField(speed, "The speed to move toward the object at (world units per second)", float, 2.0);
}

function MoveTowardBehavior::onAddToScene(%this)
{
    %this.schedule(500, updateMoveToward);
}

function MoveTowardBehavior::updateMoveToward(%this)
{
    if (!isObject(%this.target) || !%this.owner.alive)
        return;

    %this.owner.moveTo(%this.target.position, %this.speed);
   
    %this.schedule(200, updateMoveToward);
}
