//-----------------------------------------------------------------------------
// Three Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

//------------------------------------------------------------------------------
// initializeCanvas
// Constructs and initializes the default canvas window.
//------------------------------------------------------------------------------
$canvasCreated = false;
function initializeCanvas(%windowName)
{
    // Don't duplicate the canvas.
    if($canvasCreated)
    {
        error("Cannot instantiate more than one canvas!");
        return;
    }

    videoSetGammaCorrection($pref::OpenGL::gammaCorrection);

    if (!createCanvas(%windowName))
    {
        error("Canvas creation failed. Shutting down.");
        quit();
    }

    $pref::iOS::ScreenDepth = 32;

    if ($pref::iOS::DeviceType $= "")
        %res = $pref::Video::defaultResolution;
    else
        %res = iOSResolutionFromSetting($pref::iOS::DeviceType, $pref::iOS::ScreenOrientation);

    if ($platform $= "windows" || $platform $= "macos")
        setScreenMode( GetWord( %res , 0 ), GetWord( %res, 1 ), $pref::iOS::ScreenDepth, $pref::Video::fullScreen);
    else
        setScreenMode( GetWord( %res , 0 ), GetWord( %res, 1 ), $pref::iOS::ScreenDepth, false);

    $canvasCreated = true;
}

//------------------------------------------------------------------------------
// resetCanvas
// Forces the canvas to redraw itself.
//------------------------------------------------------------------------------
function resetCanvas()
{
    if (isObject(Canvas))
        Canvas.repaint();
}

new GuiCursor(DefaultCursor)
{
    hotSpot = "4 4";
    renderOffset = "0 0";
    bitmapName = "^{GameCore}/gui/images/defaultCursor";
};