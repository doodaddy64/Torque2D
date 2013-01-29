//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createSimpleSpriteToy( %scopeSet )
{
    // Create a simple "static" sprite toy.
    createSimpleStaticSpriteToy();   
    
    // Create a simple "animated" sprite toy.
    createSimpleAnimatedSpriteToy();
}

//-----------------------------------------------------------------------------

function createSimpleStaticSpriteToy()
{    
    // Create the sprite.
    %object = new Sprite();
    
    // Always try to configure the sprite prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "-25 0";
        
    // If the size is to be square then we can simply pass a single value.
    // This applies to any 'Vector2' engine type.
    %object.Size = 40;
    
    // Set the sprite to use an image.  This is known as "static" image mode.
    %object.Image = "ToyAssets:Tiles";
    
    // We don't really need to do this as the frame is set to zero by default.
    %object.Frame = 0;
        
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}

//-----------------------------------------------------------------------------

function createSimpleAnimatedSpriteToy()
{
    // Create the sprite.
    %object = new Sprite();
    
    // Always try to configure the sprite prior to adding it to a scene for best performance.

    // Set the position.
    %object.Position = "25 0";
        
    // If the size is to be square then we can simply pass a single value.
    // This applies to any 'Vector2' engine type.
    %object.Size = 40;
    
    // Set the sprite to use an animation.  This is known as "animated" image mode.
    %object.Animation = "ToyAssets:TileAnimation";
       
    // Add the sprite to the scene.
    SandboxScene.add( %object );    
}

//-----------------------------------------------------------------------------

function destroySimpleSpriteToy( %scopeSet )
{
}
