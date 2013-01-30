//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

$platformFontType = ($platform $= "windows") ? "lucida console" : "monaco";
$platformFontSize = 12;

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
    border = 0;
    borderColor   = "100 100 100 255";
    borderColorHL = "128 128 128";
    borderColorNA = "64 64 64";

    // font
    fontType = $platformFontType;
    fontSize = $platformFontSize;

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

// ----------------------------------------------------------------------------

if (!isObject(GuiTransparentProfile)) new GuiControlProfile (GuiTransparentProfile : GuiDefaultProfile)
{
    opaque = false;
    border = false;
};

// ----------------------------------------------------------------------------

if (!isObject(GuiToolTipProfile)) new GuiControlProfile (GuiToolTipProfile : GuiDefaultProfile)
{
    fillColor = "246 220 165 255";
    
    fontType = $platformFontType;
    fontSize = $platformFontSize;
};

// ----------------------------------------------------------------------------

if (!isObject(GuiPopupMenuItemBorder)) new GuiControlProfile (GuiPopupMenuItemBorder : GuiDefaultProfile)
{
    bitmap = "^Sandbox/gui/images/scroll";
    hasBitmapArray = true;
};

// ----------------------------------------------------------------------------

if (!isObject(GuiPopUpMenuDefault)) new GuiControlProfile (GuiPopUpMenuDefault)
{
    tab = false;
    canKeyFocus = false;
    hasBitmapArray = false;
    mouseOverSelected = false;

    // fill color
    opaque = false;
    fillColor = "255 255 255 192";
    fillColorHL = "255 0 0 192";
    fillColorNA = "0 0 255 255";

    // border color
    border = 1;
    borderColor    = "100 100 100 255";
    borderColorHL = "0 128 0 255";
    borderColorNA = "0 226 226 52";

    // font
    fontType = $platformFontType;
    fontSize = $platformFontSize;

    fontColor = "27 59 95 255";
    fontColorHL = "232 240 248 255";
    fontColorNA = "0 0 0 255";
    fontColorSEL= "255 255 255 255";

    // bitmap information
    bitmap = "^Sandbox/gui/images/scroll";
    hasBitmapArray = true;
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

    profileForChildren = GuiPopupMenuItemBorder;
    // sounds
    soundButtonDown = "";
    soundButtonOver = "";
};

// ----------------------------------------------------------------------------

if (!isObject(GuiPopUpMenuProfile)) new GuiControlProfile (GuiPopUpMenuProfile : GuiPopUpMenuDefault)
{
    textOffset = "6 3";
    justify = "center";
    bitmap = "^Sandbox/gui/images/dropDown";
    hasBitmapArray = true;
    border = -3;
    profileForChildren = GuiPopUpMenuDefault;
    opaque = true;
};

//-----------------------------------------------------------------------------

if (!isObject(GuiTextProfile)) new GuiControlProfile (GuiTextProfile)
{
    border=false;

    // font
    fontType = $platformFontType;
    fontSize = $platformFontSize;

    fontColor = "white";

    modal = true;
    justify = "left";
    autoSizeWidth = false;
    autoSizeHeight = false;
    returnTab = false;
    numbersOnly = false;
    cursorColor = "0 0 0 255";
};

//-----------------------------------------------------------------------------

if (!isObject(GuiCheckBoxProfile)) new GuiControlProfile (GuiCheckBoxProfile)
{
    opaque = false;
    fontColor = "white";
    fillColor = "232 232 232 255";
    fontColorHL = "white";
    border = false;
    borderColor = "0 0 0 255";
    fontType = $platformFontType;
    fontSize = $platformFontSize;
    fixedExtent = true;
    justify = "left";
    bitmap = "^Sandbox/gui/images/checkBox";
    hasBitmapArray = true;
};

//-----------------------------------------------------------------------------

if(!isObject(GuiConsoleProfile)) new GuiControlProfile (GuiConsoleProfile)
{
    fontType = $platformFontType;
    fontSize = $platformFontSize;
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
   returnTab = true;
   
};

//-----------------------------------------------------------------------------

if(!isObject(GuiNumberEditProfile)) new GuiControlProfile (GuiNumberEditProfile: GuiTextEditProfile)
{
   numbersOnly = true;
};

//-----------------------------------------------------------------------------

if(!isObject(GuiConsoleTextEditProfile)) new GuiControlProfile (GuiConsoleTextEditProfile : GuiTextEditProfile)
{
    fontType = $platformFontType;
    fontSize = $platformFontSize;
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
    fillColor = "0 0 0 92";

    // font
    fontType = $platformFontType;
    fontSize = $platformFontSize;
    fontColor = "255 255 255 255";
}; 
