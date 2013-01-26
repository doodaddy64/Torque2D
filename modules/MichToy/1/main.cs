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

    %imageFont = new ImageFont();
    SandboxScene.add( %imageFont );
    %imageFont.image = "Sandbox:font";
    %imageFont.fontSize = 3;
    %imageFont.text = "This is a test!";
    
}

//-----------------------------------------------------------------------------

function destroyMichToy( %scopeSet )
{
}
