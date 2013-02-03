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

if (!isObject(FaceObjectBehavior))
{
    %template = new BehaviorTemplate(FaceObjectBehavior);

    %template.friendlyName = "Face Object";
    %template.behaviorType = "AI";
    %template.description  = "Set the object to face another object";

    %template.addBehaviorField(object, "The object to face", object, "", sceneObject);
    %template.addBehaviorField(turnSpeed, "The speed to rotate at (degrees per second). Use 0 to snap", float, 0.0);
    %template.addBehaviorField(rotationOffset, "The rotation offset (degrees)", float, 0.0);
}

function FaceObjectBehavior::onAddToScene(%this)
{
    %this.schedule(200, updateFace);
}

function FaceObjectBehavior::updateFace(%this)
{
    if (!isObject(%this.object) || !%this.owner.active)
        return;
   
    %vector = t2dVectorSub(%this.object.position, %this.owner.position);
    %targetRotation = mRadToDeg(mAtan(%vector.y, %vector.x)) + 90 + %this.rotationOffset;
   
    if (%this.turnSpeed == 0)
        %this.owner.setRotation(%targetRotation);
    else
        %this.owner.rotateTo(%targetRotation, %this.turnSpeed, true, false, true, 0.1);
      
    %this.schedule(200, updateFace);
}
