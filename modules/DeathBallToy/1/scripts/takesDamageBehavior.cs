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

if (!isObject(TakesDamageBehavior))
{
    %template = new BehaviorTemplate(TakesDamageBehavior);

    %template.friendlyName = "Takes Damage";
    %template.behaviorType = "Game";
    %template.description  = "Set the object to take damage from DealsDamage objects that collide with it";

    %template.addBehaviorField(health, "The amount of health the object has", int, 100);
    %template.addBehaviorField(explodeEffect, "The particle effect to play on death", asset, "", ParticleAsset);
    %template.addBehaviorField(spawnEffect, "The particle effect to play on spawn", asset, "", ParticleAsset);
    %template.addBehaviorField(deathAnim, "The object's death animation, alternative to explodeEffect", asset, "", AnimationAsset);
    %template.addBehaviorField(deleteOnDeath, "Delete the owner upon dieing", bool, false);
}

function TakesDamageBehavior::initialize(%this)
{
    %this.startHealth = %this.health;
   
    %this.spawn();
}

function TakesDamageBehavior::takeDamage(%this, %amount, %assailant)
{
    %this.health -= %amount;
   
    if (%this.health <= 0)
    {
        // This only works on animated sprites
        if (%this.deathAnim !$= "")
            %this.owner.setAnimation(%this.deathAnim);
      
        if (%this.explodeEffect !$= "")
            %this.explode();
            
        %this.kill();
    }
}

function TakesDamageBehavior::kill(%this)
{
    %this.owner.setCollisionSuppress();
    %this.owner.active = false;
    %this.owner.alive = false;
    
    if (%this.deleteOnDeath)
        %this.owner.safeDelete();
}

function TakesDamageBehavior::spawn(%this)
{
    %this.health = %this.startHealth;

    %this.owner.setCollisionSuppress(false);
    %this.owner.visible = true;
    %this.owner.alive = true;

    if ( isObject(%this.spawnEffect) )
    {
        %particlePlayer = new ParticlePlayer();
        %particlePlayer.BodyType = static;
        %particlePlayer.SetPosition( %this.owner.getPosition() );
        %particlePlayer.SceneLayer = %this.owner.getSceneLayer();
        %particlePlayer.ParticleInterpolation = true;
        %particlePlayer.Particle = %this.spawnEffect;
        SandboxScene.add( %particlePlayer );
    }
}

function TakesDamageBehavior::explode(%this)
{
    %particlePlayer = new ParticlePlayer();
    %particlePlayer.BodyType = static;
    %particlePlayer.SetPosition( %this.owner.getPosition() );
    %particlePlayer.SceneLayer = %this.owner.getSceneLayer();
    %particlePlayer.ParticleInterpolation = true;
    %particlePlayer.Particle = %this.explodeEffect;
    SandboxScene.add( %particlePlayer ); 
}
