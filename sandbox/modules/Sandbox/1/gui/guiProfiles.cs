//-----------------------------------------------------------------------------
// Three Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

new GuiCursor(DefaultCursor)
{
    hotSpot = "4 4";
    renderOffset = "0 0";
    bitmapName = "^Sandbox/gui/images/defaultCursor";
};

//---------------------------------------------------------------------------------------------
// GuiDefaultProfile is a special profile that all other profiles inherit defaults from. It
// must exist.
//---------------------------------------------------------------------------------------------
if(!isObject(GuiDefaultProfile)) new GuiControlProfile (GuiDefaultProfile)
{
    tab = false;
    canKeyFocus = false;
    hasBitmapArray = false;
    mouseOverSelected = false;

    // fill color
    opaque = false;
    fillColor = "211 211 211";
    fillColorHL = "244 244 244";
    fillColorNA = "244 244 244";

    // border color
    border = 1;
    borderColor   = "100 100 100 255";
    borderColorHL = "128 128 128";
    borderColorNA = "64 64 64";

    // font
    fontType = "Arial";
    fontSize = 14;

    fontColor = "0 0 0";
    fontColorHL = "32 100 100";
    fontColorNA = "0 0 0";
    fontColorSEL= "10 10 10";

    // bitmap information
    bitmap = "^Sandbox/gui/images/window.png";
    bitmapBase = "";
    textOffset = "0 0";

    // used by guiTextControl
    modal = true;
    justify = "left";
    autoSizeWidth = false;
    autoSizeHeight = false;
    returnTab = false;
    numbersOnly = false;
    cursorColor = "0 0 0 255";

    // sounds
    soundButtonDown = $ButtonSound.fileName;
    soundButtonOver = "";
};

//-----------------------------------------------------------------------------

if(!isObject(GuiConsoleProfile)) new GuiControlProfile (GuiConsoleProfile)
{
    fontType = ($platform $= "macos") ? "monaco" : "lucida console";
    fontSize = 12;
    fontColor = "255 255 255";
    fontColorHL = "155 155 155";
    fontColorNA = "255 0 0";
    fontColors[6] = "100 100 100";
    fontColors[7] = "100 100 0";
    fontColors[8] = "0 0 100";
    fontColors[9] = "0 100 0";
};

//-----------------------------------------------------------------------------

if(!isObject(GuiTextEditProfile)) new GuiControlProfile (GuiTextEditProfile)
{
   opaque = true;
   fillColor = "255 255 255";
   fillColorHL = "128 128 128";
   border = -2;
   bitmap = "^Sandbox/gui/images/textEdit.png";
   borderColor = "40 40 40 10";
   fontColor = "0 0 0";
   fontColorHL = "0 0 0";
   fontColorNA = "0 0 0";
   fontColorSEL = "0 0 0";
   textOffset = "4 2";
   autoSizeWidth = false;
   autoSizeHeight = true;
   tab = true;
   canKeyFocus = true;
   
};

//-----------------------------------------------------------------------------

if(!isObject(GuiConsoleTextEditProfile)) new GuiControlProfile (GuiConsoleTextEditProfile : GuiTextEditProfile)
{
    fontType = ($platform $= "macos") ? "monaco" : "lucida console";
    fontSize = 12;
};

//-----------------------------------------------------------------------------

if(!isObject(GuiScrollProfile)) new GuiControlProfile (GuiScrollProfile)
{
    opaque = true;
    fillColor = "255 255 255";
    border = 1;
    borderThickness = 2;
    bitmap = "^Sandbox/gui/images/scrollBar.png";
    hasBitmapArray = true;
};

//-----------------------------------------------------------------------------

if(!isObject(ConsoleScrollProfile)) new GuiControlProfile( ConsoleScrollProfile : GuiScrollProfile )
{
    opaque = true;
    fillColor = "0 0 0 120";
    border = 3;
    borderThickness = 0;
    borderColor = "0 0 0";
};

//-----------------------------------------------------------------------------

if(!isObject(GuiToolboxProfile)) new GuiControlProfile( GuiToolboxProfile : GuiScrollProfile )
{
    opaque = true;
    fillColor = "0 0 0 192";
    border = 3;
    borderThickness = 0;
    borderColor = "0 0 0";
};

//-----------------------------------------------------------------------------

if(!isObject(SandboxWindowProfile)) new GuiControlProfile (SandboxWindowProfile : GuiDefaultProfile)
{
    // fill color
    opaque = false;
    fillColor = "0 0 0 64";

    // font
    fontType = ($platform $= "macos") ? "monaco" : "lucida console";
    fontSize = 12;
    fontColor = "255 255 255 255";
}; 
