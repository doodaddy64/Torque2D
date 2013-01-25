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

    //%imageFont = new ImageFont();
    //SandboxScene.add( %imageFont );
    //%imageFont.image = "Sandbox:font";
    //%imageFont.text = "0123456";
}

//-----------------------------------------------------------------------------

function destroyMichToy( %scopeSet )
{
}
