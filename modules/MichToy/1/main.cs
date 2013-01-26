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

    $framePeriod = new ImageFont();
    $framePeriod.Position = "-50 35";
    $framePeriod.Image = "Sandbox:font";
    $framePeriod.TextAlignment = "Left";
    $framePeriod.Text = "FramePeriod";
    $framePeriod.FontSize = 3;
    SandboxScene.add( $framePeriod );

    $frameCount = new ImageFont();
    $frameCount.Position = "-50 30";
    $frameCount.Image = "Sandbox:font";
    $frameCount.TextAlignment = "Left";
    $frameCount.Text = "FrameCount";
    $frameCount.FontSize = 3;
    SandboxScene.add( $frameCount );

    // Schedule to update the metrics.
    $updateMichToyMetricsSchedule = schedule( 100, 0, updateMichToyMetrics );   
}

//-----------------------------------------------------------------------------

function updateMichToyMetrics()
{
    // Reset the event schedule.
    $updateMichToyMetricsSchedule = "";
    
    $framePeriod.Text = "FPS = " @ $fps::framePeriod;
    $frameCount.Text = "FrameCount = " @ $fps::frameCount;
    
    // Schedule to update the metrics.
    $updateMichToyMetricsSchedule = schedule( 100, 0, updateMichToyMetrics );     
}

//-----------------------------------------------------------------------------

function destroyMichToy( %scopeSet )
{   
    // Cancel any pending events.
    if ( isEventPending($updateMichToyMetricsSchedule) )
    {
        cancel($updateMichToyMetricsSchedule);
        $updateMichToyMetricsSchedule = "";
    }    
    
    $imageFont = "";    
}
