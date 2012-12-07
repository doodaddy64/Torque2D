//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

// Set log mode.
setLogMode(2);

// Controls whether the execution or script files or compiled DSOs are echoed to the console or not.
// Being able to turn this off means far less spam in the console during typical development.
setScriptExecEcho( false );

// Controls whether all script execution is traced (echoed) to the console or not.
trace( false );

// Controls whether global 
$pref::T2D::imageAssetGlobalFilterMode = "Smooth";

// The name of the company. Used to form the path to save preferences. Defaults to GarageGames
// if not specified.
// The name of the game. Used to form the path to save preferences. Defaults to C++ engine define TORQUE_GAME_NAME
// if not specified.
// Appending version string to avoid conflicts with existing versions and other versions.
setCompanyAndProduct("GarageGames", "3StepStudio" @ getThreeStepStudioVersion());

// Set module database information echo.
ModuleDatabase.EchoInfo = false;

// Set asset database information echo.
AssetDatabase.EchoInfo = false;

// Is a module merge available?
if ( ModuleDatabase.isModuleMergeAvailable() )
{
    // Yes, so merge modules.
    if ( ModuleDatabase.mergeModules( "modules", true, false ) == false )
    {
        error( "A serious error occurred merging modules!" );
        quit();
    }
}

// Scan modules.
ModuleDatabase.scanModules( "modules" );

//-----------------------------------------------------------------------------

function resetCanvas()
{
    if (isObject(Canvas))
        Canvas.repaint();
}

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
        fontType = "Open Sans";
        fontSize = 18;
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

function createTestScene()
{
    new SceneWindow(testSceneWindow2D)
    { 
        Profile = "GuiDefaultProfile";
        useMouseEvents = "1";
        Active = "1";
    };
    
    if (!isObject(testSceneWindow2D))
    {
       error("Failed to create SceneWindow");
       quit();
    }

    new Scene(testScene);
    
    if (!isObject(testScene))
    {
        error("Failed to create Scene");
        quit();
    }
    
    testSceneWindow2D.setScene( testScene );
    Canvas.setContent(testSceneWindow2D);
}

//-----------------------------------------------------------------------------

function escapeQuit(%val)
{
    quit();
}

function echoLowerA(%val)
{
    echo("a");
}
function echoLowerB(%val)
{
    echo("b");
}
function echoLowerC(%val)
{
    echo("c");
}
function echoLowerD(%val)
{
    echo("d");
}
function echoLowerE(%val)
{
    echo("e");
}
function echoLowerF(%val)
{
    echo("f");
}
function echoLowerG(%val)
{
    echo("g");
}
function echoLowerH(%val)
{
    echo("h");
}
function echoLowerI(%val)
{
    echo("i");
}
function echoLowerJ(%val)
{
    echo("j");
}
function echoLowerK(%val)
{
    echo("k");
}
function echoLowerL(%val)
{
    echo("l");
}
function echoLowerM(%val)
{
    echo("m");
}
function echoLowerN(%val)
{
    echo("n");
}
function echoLowerO(%val)
{
    echo("o");
}
function echoLowerP(%val)
{
    echo("p");
}
function echoLowerQ(%val)
{
    echo("q");
}
function echoLowerR(%val)
{
    echo("r");
}
function echoLowerS(%val)
{
    echo("s");
}
function echoLowerT(%val)
{
    echo("t");
}
function echoLowerU(%val)
{
    echo("u");
}
function echoLowerV(%val)
{
    echo("v");
}
function echoLowerW(%val)
{
    echo("w");
}
function echoLowerX(%val)
{
    echo("x");
}
function echoLowerY(%val)
{
    echo("y");
}
function echoLowerZ(%val)
{
    echo("z");
}

function lowerAlphaBinds()
{
    moveMap.bind(keyboard, a, "echoLowerA");
    moveMap.bind(keyboard, b, "echoLowerB");
    moveMap.bind(keyboard, c, "echoLowerC");
    moveMap.bind(keyboard, d, "echoLowerD");
    moveMap.bind(keyboard, e, "echoLowerE");
    moveMap.bind(keyboard, f, "echoLowerF");
    moveMap.bind(keyboard, g, "echoLowerG");
    moveMap.bind(keyboard, h, "echoLowerH");
    moveMap.bind(keyboard, i, "echoLowerI");
    moveMap.bind(keyboard, j, "echoLowerJ");
    moveMap.bind(keyboard, k, "echoLowerK");
    moveMap.bind(keyboard, l, "echoLowerL");
    moveMap.bind(keyboard, m, "echoLowerM");
    moveMap.bind(keyboard, n, "echoLowerN");
    moveMap.bind(keyboard, o, "echoLowerO");
    moveMap.bind(keyboard, p, "echoLowerP");
    moveMap.bind(keyboard, q, "echoLowerQ");
    moveMap.bind(keyboard, r, "echoLowerR");
    moveMap.bind(keyboard, s, "echoLowerS");
    moveMap.bind(keyboard, t, "echoLowerT");
    moveMap.bind(keyboard, u, "echoLowerU");
    moveMap.bind(keyboard, v, "echoLowerV");
    moveMap.bind(keyboard, w, "echoLowerW");
    moveMap.bind(keyboard, x, "echoLowerX");
    moveMap.bind(keyboard, y, "echoLowerY");
    moveMap.bind(keyboard, z, "echoLowerZ");
}

function echoUpperA(%val)
{
    echo("A");
}
function echoUpperB(%val)
{
    echo("B");
}
function echoUpperC(%val)
{
    echo("C");
}
function echoUpperD(%val)
{
    echo("D");
}
function echoUpperE(%val)
{
    echo("E");
}
function echoUpperF(%val)
{
    echo("F");
}
function echoUpperG(%val)
{
    echo("G");
}
function echoUpperH(%val)
{
    echo("H");
}
function echoUpperI(%val)
{
    echo("I");
}
function echoUpperJ(%val)
{
    echo("J");
}
function echoUpperK(%val)
{
    echo("K");
}
function echoUpperL(%val)
{
    echo("L");
}
function echoUpperM(%val)
{
    echo("M");
}
function echoUpperN(%val)
{
    echo("N");
}
function echoUpperO(%val)
{
    echo("O");
}
function echoUpperP(%val)
{
    echo("P");
}
function echoUpperQ(%val)
{
    echo("Q");
}
function echoUpperR(%val)
{
    echo("R");
}
function echoUpperS(%val)
{
    echo("S");
}
function echoUpperT(%val)
{
    echo("T");
}
function echoUpperU(%val)
{
    echo("U");
}
function echoUpperV(%val)
{
    echo("V");
}
function echoUpperW(%val)
{
    echo("W");
}
function echoUpperX(%val)
{
    echo("X");
}
function echoUpperY(%val)
{
    echo("Y");
}
function echoUpperZ(%val)
{
    echo("Z");
}

function upperAlphaBinds()
{
    moveMap.bind(keyboard, "shift a", "echoUpperA");
    moveMap.bind(keyboard, "shift b", "echoUpperB");
    moveMap.bind(keyboard, "shift c", "echoUpperC");
    moveMap.bind(keyboard, "shift d", "echoUpperD");
    moveMap.bind(keyboard, "shift e", "echoUpperE");
    moveMap.bind(keyboard, "shift f", "echoUpperF");
    moveMap.bind(keyboard, "shift g", "echoUpperG");
    moveMap.bind(keyboard, "shift h", "echoUpperH");
    moveMap.bind(keyboard, "shift i", "echoUpperI");
    moveMap.bind(keyboard, "shift j", "echoUpperJ");
    moveMap.bind(keyboard, "shift k", "echoUpperK");
    moveMap.bind(keyboard, "shift l", "echoUpperL");
    moveMap.bind(keyboard, "shift m", "echoUpperM");
    moveMap.bind(keyboard, "shift n", "echoUpperN");
    moveMap.bind(keyboard, "shift o", "echoUpperO");
    moveMap.bind(keyboard, "shift p", "echoUpperP");
    moveMap.bind(keyboard, "shift q", "echoUpperQ");
    moveMap.bind(keyboard, "shift r", "echoUpperR");
    moveMap.bind(keyboard, "shift s", "echoUpperS");
    moveMap.bind(keyboard, "shift t", "echoUpperT");
    moveMap.bind(keyboard, "shift u", "echoUpperU");
    moveMap.bind(keyboard, "shift v", "echoUpperV");
    moveMap.bind(keyboard, "shift w", "echoUpperW");
    moveMap.bind(keyboard, "shift x", "echoUpperX");
    moveMap.bind(keyboard, "shift y", "echoUpperY");
    moveMap.bind(keyboard, "shift z", "echoUpperZ");
}

function selectAll(%val)
{
    if (%val)
        echo("Selecting all");
}

function copyText(%val)
{
    if (%val)
        echo("Copying");
}

function pasteText(%val)
{
    if (%val)
        echo("Pasting");
}

function undoText(%val)
{
    if (%val)
        echo("Alt undoing");
}

function specialBindings()
{
    %cmdCtrl = $platform $= "macos" ? "Cmd" : "Ctrl";
    
    moveMap.bind(keyboard, %cmdCtrl SPC a, selectAll);
    moveMap.bind(keyboard, %cmdCtrl SPC c, copyText);
    moveMap.bind(keyboard, %cmdCtrl SPC v, pasteText);
    moveMap.bind(keyboard, "Cmd z", undoText);
}

function createTestBindings()
{
    new ActionMap(moveMap);
    
    moveMap.bind(keyboard, escape, "escapeQuit");

    lowerAlphaBinds();
    upperAlphaBinds();
    specialBindings();
    
    moveMap.push();
}

//-----------------------------------------------------------------------------

function startKeyBindTesting()
{
    echo("@@@ Starting key bind tests");
    
    createDefaultProfile();
    createDefaultCanvas();
    
    createTestScene();
    
    createTestBindings();
}

//-----------------------------------------------------------------------------

startKeyBindTesting();