//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Set log mode.
setLogMode(2);

// Set profiler.
//profilerEnable( true );

// Controls whether the execution or script files or compiled DSOs are echoed to the console or not.
// Being able to turn this off means far less spam in the console during typical development.
setScriptExecEcho( false );

// Controls whether all script execution is traced (echoed) to the console or not.
trace( false );

// Sets whether to ignore compiled TorqueScript files (DSOs) or not.
$Scripts::ignoreDSOs = true;

// Controls whether global 
$pref::T2D::imageAssetGlobalFilterMode = "Smooth";

// The name of the company. Used to form the path to save preferences. Defaults to GarageGames
// if not specified.
// The name of the game. Used to form the path to save preferences. Defaults to C++ engine define TORQUE_GAME_NAME
// if not specified.
// Appending version string to avoid conflicts with existing versions and other versions.
setCompanyAndProduct("GarageGames", "Sandbox" );

// Set module database information echo.
ModuleDatabase.EchoInfo = false;

// Set asset database information echo.
AssetDatabase.EchoInfo = false;

// Scan modules.
ModuleDatabase.scanModules( "modules" );

// Load sandbox module.
ModuleDatabase.LoadExplicit( "{Sandbox}" );
//ModuleDatabase.UnloadExplicit( "{Sandbox}" );
//quit();

//-----------------------------------------------------------------------------

function onExit()
{
}
