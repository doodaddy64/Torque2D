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

//------------------------------------------------------------------------------
// iOSResolutionFromSetting
// Helper function that grabs resolution strings based on device type
//------------------------------------------------------------------------------
function iOSResolutionFromSetting( %deviceType, %deviceScreenOrientation )
{
    // A helper function to get a string based resolution from the settings given.
    %x = 0;
    %y = 0;
    
    %scaleFactor = $pref::iOS::RetinaEnabled ? 2 : 1;

    switch(%deviceType)
    {
        case $iOS::constant::iPhone:
            if(%deviceScreenOrientation == $iOS::constant::Landscape)
            {
                %x =  $iOS::constant::iPhoneWidth * %scaleFactor;
                %y =  $iOS::constant::iPhoneHeight * %scaleFactor;
            }
            else
            {
                %x =  $iOS::constant::iPhoneHeight * %scaleFactor;
                %y =  $iOS::constant::iPhoneWidth * %scaleFactor;
            }

        case $iOS::constant::iPad:
            if(%deviceScreenOrientation == $iOS::constant::Landscape)
            {
                %x =  $iOS::constant::iPadWidth * %scaleFactor;
                %y =  $iOS::constant::iPadHeight * %scaleFactor;
            }
            else
            {
                %x =  $iOS::constant::iPadHeight * %scaleFactor;
                %y =  $iOS::constant::iPadWidth * %scaleFactor;
            }

        case $iOS::constant::iPhone5:
            if(%deviceScreenOrientation == $iOS::constant::Landscape)
            {
                %x =  $iOS::constant::iPhone5Width;
                %y =  $iOS::constant::iPhone5Height;
            }
            else
            {
                %x =  $iOS::constant::iPhone5Height;
                %y =  $iOS::constant::iPhone5Width;
            }
    }
   
    return %x @ " " @ %y;
}