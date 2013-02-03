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

if (!isObject(SpawnAreaBehavior))
{
    %template = new BehaviorTemplate(SpawnAreaBehavior);

    %template.friendlyName = "Spawn Area";
    %template.behaviorType = "AI";
    %template.description  = "Spawns objects inside the area of this object";

    %template.addBehaviorField(object, "The object to clone", object, "", sceneObject);
    %template.addBehaviorField(count, "The number of objects to clone (-1 for infinite)", int, 50);
    %template.addBehaviorField(spawnTime, "The time between spawns (seconds)", float, 2.0);
    %template.addBehaviorField(spawnVariance, "The variance in the spawn time (seconds)", float, 1.0);

    %spawnLocations = "Area" TAB "Edges" TAB "Center" TAB "Top" TAB "Bottom" TAB "Left" TAB "Right";
    %template.addBehaviorField(spawnLocation, "The are in which objects can be spawned", enum, "Area", %spawnLocations);

    %template.addBehaviorField(autoSpawn, "Automatically start/stop spawning", bool, true);
}

function SpawnAreaBehavior::onAddToScene(%this, %scenegraph)
{
    %this.spawnCount = 0;
   
    if (%this.count && %this.autoSpawn)
        %this.spawnEvent = %this.schedule(%this.spawnTime, "spawn");
}

function SpawnAreaBehavior::startSpawns(%this)
{
    if (%this.autoSpawn)
        return;
      
    if (%this.count)
        %this.spawnEvent = %this.schedule(%this.spawnTime, "spawn");
}

function SpawnAreaBehavior::spawn(%this)
{
    if (!isObject(%this.object))
        return;
   
    %clone = 0;
   
    for (%i = 0; %i < %this.spawnCount; %i++)
    {
        if (!%this.pool[%i].isEnabled())
        {
            %clone = %this.pool[%i];
            %clone.Enabled = true;
         
            break;
        }
    }
   
    if (!%clone)
    {
        %this.pool[%this.spawnCount] = %this.object.cloneWithBehaviors();
        %clone = %this.pool[%this.spawnCount];
        %this.spawnCount++;
    }
   
    %xPos = 0;
    %yPos = 0;
    %spawnLocation = %this.spawnLocation;
    %edges = "Top" TAB "Bottom" TAB "Left" TAB "Right";

    if (%spawnLocation $= "Edges")
        %spawnLocation = getField(%edges, getRandom(0, 3));
   
    switch$ (%spawnLocation)
    {
        case "Area":
            %xPos = getRandom(getWord(%this.owner.getAreaMin(), 0), getWord(%this.owner.getAreaMax(), 0));
            %yPos = getRandom(getWord(%this.owner.getAreaMin(), 1), getWord(%this.owner.getAreaMax(), 1));
            
        case "Center":
            %xPos = %this.owner.position.x;
            %yPos = %this.owner.position.y;
            
        case "Top":
            %xPos = getRandom(getWord(%this.owner.getAreaMin(), 0), getWord(%this.owner.getAreaMax(), 0));
            %yPos = getWord(%this.owner.getAreaMin(), 1);
        
        case "Bottom":
            %xPos = getRandom(getWord(%this.owner.getAreaMin(), 0), getWord(%this.owner.getAreaMax(), 0));
            %yPos = getWord(%this.owner.getAreaMax(), 1);
        
        case "Left":
            %xPos = getWord(%this.owner.getAreaMin(), 0);
            %yPos = getRandom(getWord(%this.owner.getAreaMin(), 1), getWord(%this.owner.getAreaMax(), 1));
        
        case "Right":
            %xPos = getWord(%this.owner.getAreaMax(), 0);
            %yPos = getRandom(getWord(%this.owner.getAreaMin(), 1), getWord(%this.owner.getAreaMax(), 1));
   }
   
    %clone.position = %xPos SPC %yPos;
   
    %clone.active = true;

    if (%this.spawnCount < %this.count)
    {
        %minTime = (%this.spawnTime - %this.spawnVariance) * 1000;
        %maxTime = (%this.spawnTime + %this.spawnVariance) * 1000;
        %spawnTime = getRandom(%minTime, %maxTime);
        %this.spawnEvent = %this.schedule(%spawnTime, "spawn");
    }
}

function SpawnAreaBehavior::stopSpawns(%this)
{
    if (%this.autoSpawn)
        return;
      
    cancel(%this.spawnEvent);
}