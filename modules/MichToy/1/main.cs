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

    $imageFont = new ImageFont();
    SandboxScene.add( $imageFont );
    $imageFont.image = "Sandbox:font";
    $imageFont.fontSize = 3;
    $imageFont.text = "This is a test!";
    
    // Schedule to create a ball.
    $updateFPSSchedule = schedule( 100, 0, updateMichToyFPS );   
}

//-----------------------------------------------------------------------------

function updateMichToyFPS()
{
    // Reset the event schedule.
    $createTumblerBallSchedule = "";
    
    $imageFont.text = $fps::framePeriod;
    
    // Schedule to create a ball.
    $createTumblerBallSchedule = schedule( 100, 0, updateMichToyFPS );    
}

//-----------------------------------------------------------------------------

function destroyMichToy( %scopeSet )
{   
    // Cancel any pending events.
    if ( isEventPending($createTumblerBallSchedule) )
    {
        cancel($createTumblerBallSchedule);
        $createTumblerBallSchedule = "";
    }    
    
    $imageFont = "";    
}
