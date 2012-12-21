//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function createDefaultProfile()
{
    if(!isObject(GuiDefaultProfile)) new GuiControlProfile (GuiDefaultProfile)
    {
        tab = false;
        canKeyFocus = false;
        hasBitmapArray = false;
        mouseOverSelected = false;

        // fill color
        opaque = false;
        fillColor = "211 211 211 64";
        fillColorHL = "244 244 244 64";
        fillColorNA = "244 244 244 64";
        
        // font
        fontType = $platform $= "windows" ? "lucida console" : "monaco";
        fontSize = 12;
        fontColor = "255 255 255 255";

        // border color
        border = 1;
        borderColor   = "100 100 100 255";
        borderColorHL = "128 128 128";
        borderColorNA = "64 64 64";

    };
}

//-----------------------------------------------------------------------------

function createDefaultCanvas()
{
    if (!createCanvas("Torque 2D"))
    {
        error("Canvas creation failed. Shutting down.");
        quit();
    }
    
    echo("- Created canvas successfully");
    
    if (!setScreenMode( 1024, 768, 32, false ))
    {
        error("Could not set screen mode");
        quit();
    }
}

//-----------------------------------------------------------------------------

function escapeQuit(%val)
{
    quit();
}

//-----------------------------------------------------------------------------

function createTestBindings()
{
    if (!isObject(moveMap))
        new ActionMap(moveMap);
    
    moveMap.bind(keyboard, escape, "escapeQuit");
}

//-----------------------------------------------------------------------------

function resetCanvas()
{
    if (isObject(Canvas))
        Canvas.repaint();
}