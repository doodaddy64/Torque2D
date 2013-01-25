//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createMichToy( %scopeSet )
{
    %sprite = new Sprite();
    SandboxScene.add( %sprite );
    
    %sprite.Image = "MichToy:DefaultImage";
    %sprite.Size = 60;
    %sprite.AngularVelocity = 90;
}

//-----------------------------------------------------------------------------

function destroyMichToy( %scopeSet )
{
}
