//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

/// Game
$Game::CompanyName              = "GarageGames LLC";
$Game::ProductName              = "Torque 2D Sandbox";

// Sandbox.
$pref::Sandbox::defaultToyId    = "MelvToy";
$pref::Sandbox::defaultBackgroundColor = "DimGray";
$pref::Sandbox::metricsOption   = true;
$pref::Sandbox::jointsOption    = false;
$pref::Sandbox::wireframeOption = false;
$pref::Sandbox::aabbOption      = false;
$pref::Sandbox::oobbOption      = false;
$pref::Sandbox::sleepOption     = false;
$pref::Sandbox::collisionOption = false;
$pref::Sandbox::positionOption  = false;
$pref::Sandbox::sortOption      = false;
$pref::Sandbox::cameraZoomRate  = 0.02;

/// iOS
$pref::iOS::ScreenOrientation   = $iOS::constant::Landscape;
$pref::iOS::ScreenDepth		    = 32;
$pref::iOS::UseGameKit          = 0;
$pref::iOS::UseMusic            = 0;
$pref::iOS::UseMoviePlayer      = 0;
$pref::iOS::UseAutoRotate       = 0;   
$pref::iOS::EnableOrientationRotation = 1;   
$pref::iOS::StatusBarType       = 0;

/// Audio
$pref::Audio::driver = "OpenAL";
$pref::Audio::forceMaxDistanceUpdate = 0;
$pref::Audio::environmentEnabled = 0;
$pref::Audio::masterVolume   = 1.0;
$pref::Audio::channelVolume1 = 1.0;
$pref::Audio::channelVolume2 = 1.0;
$pref::Audio::channelVolume3 = 1.0;
$pref::Audio::sfxVolume = 1.0;
$pref::Audio::musicVolume = 1.0;

/// T2D
$pref::T2D::particleEngineQuantityScale = 1.0;
$pref::T2D::warnFileDeprecated = 1;
$pref::T2D::warnSceneOccupancy = 1;

/// Video
$pref::Video::appliedPref = 0;
$pref::Video::disableVerticalSync = 1;
$pref::Video::displayDevice = "OpenGL";
$pref::Video::preferOpenGL = 1;
$pref::Video::fullScreen = 0;
$pref::OpenGL::gammaCorrection = 0.5;
$pref::Video::defaultResolution = "1024 768";

/// Fonts.
$Gui::fontCacheDirectory = expandPath( "^Sandbox/fonts" );