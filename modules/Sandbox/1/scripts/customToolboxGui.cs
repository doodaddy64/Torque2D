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

$customLabelHeight = "15";
$customLabelWidth = "50";
$customLabelSpacing = "18";
$customOptionSpacing = "25";
$customContainerExtent = "0 0";
$containerXPosition = "0";
$flagOptionExtent = "100 25";
$buttonOptionExtent = "100 25";
$spinnerExtent = "22 25";
$intOptionExtent = "80 25";
$listOptionExtent = "0 0";
$customControlCount = 0;

//-----------------------------------------------------------------------------

function createCustomLabel(%text)
{
    %labelWidth = $customLabelWidth + (%characterCount * 5);
    %labelExtent = %labelWidth SPC $customLabelHeight;

    %labelControl = new GuiTextCtrl()
    {
        text = %label;
        Extent = %labelExtent;
        Profile = "GuiTextProfile";
        canSaveDynamicFields = "0";
        isContainer = "0";
        Position = "1 1";
        MinExtent = "8 2";
        canSave = "0";
        Visible = "1";
        Active = "0";
        tooltipprofile = "GuiToolTipProfile";
        tooltipWidth = "0";
        maxLength = "255";
        truncate = "0";
    };

    return %labelControl;
}

//-----------------------------------------------------------------------------

function nextCustomControlPosition(%index)
{
    %verticalOffset = $customOptionSpacing * %index;
    %position = $containerXPosition SPC %verticalOffset;

    return %position;
}

//-----------------------------------------------------------------------------

function addFlagOption( %label, %callback, %startingValue, %shouldReset)
{
    %containerPosition = nextCustomControlPosition($customControlCount);

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = $customContainerExtent;
        Profile = GuiTransparentProfile;
    };

    %button = new GuiButtonCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "BlueButtonProfile";
        Position = "1 1";
        Extent = $flagOptionExtent;
        Visible = "1";
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "FlagController";
        isContainer = "0";
        Active = "1";
        hovertime = "1000";
        toolTipProfile = "GuiToolTipProfile";
        text = %label;
        groupNum = "-1";
        buttonType = "ToggleButton";
        useMouseEvents = "0";
     };

    %button.setActive(%startingValue);
    %button.command = %button @ ".updateToy();";

    %container.add(%button);

    ToyCustomControls.add(%container);

    $customControlCount++;
}

//-----------------------------------------------------------------------------

function FlagController::updateToy(%this)
{
    if (%this.toy $= "")
        return;
        
    if (%this.callback !$= "")
    {
        %setter = "%this.toy." @ %this.callback @ "(" @ %this.getValue() @ ");";

        eval(%setter);
    }
    
    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}

//-----------------------------------------------------------------------------

function addButtonOption( %label, %callback, %shouldReset)
{
    %containerPosition = nextCustomControlPosition($customControlCount);

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = $customContainerExtent;
        Profile = GuiTransparentProfile;
    };

    %button = new GuiButtonCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "BlueButtonProfile";
        Position = "1 1";
        Extent = $buttonOptionExtent;
        Visible = "1";
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "ButtonController";
        isContainer = "0";
        Active = "1";
        hovertime = "1000";
        toolTipProfile = "GuiToolTipProfile";
        text = %label;
        groupNum = "-1";
        buttonType = "PushButton";
        useMouseEvents = "0";
     };

    %button.command = %button @ ".updateToy();";

    %container.add(%button);

    ToyCustomControls.add(%container);

    $customControlCount++;
}

//-----------------------------------------------------------------------------

function ButtonController::updateToy(%this)
{
    if (%this.toy $= "")
        return;

    if (%this.callback !$= "")
    {
        %setter = "%this.toy." @ %this.callback @ "();";
        eval(%setter);
    }

    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}

//-----------------------------------------------------------------------------

function addIntegerOption( %label, %min, %max, %callback, %startingValue, %shouldReset)
{
    %customLabel = createCustomLabel(%label);

    %containerPosition = nextCustomControlPosition($customControlCount);

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = $customContainerExtent;
        Profile = GuiTransparentProfile;
    };

    %container.add(%customLabel);

    %spinnerPosition = "1" SPC $customLabelSpacing;

    %spinnerDown = new GuiImageButtonCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiDefaultProfile";
        Position = %spinnerPosition;
        Extent = $spinnerExtent;
        MinExtent = "8 2";
        canSave = "1";
        Visible = "1";
        Active = "1";
        hovertime = "1000";
        toolTipProfile = "GuiToolTipProfile";
        groupNum = "-1";
        buttonType = "PushButton";
        useMouseEvents = "0";
        NormalImage = "Sandbox:minusButtonNormal";
        HoverImage = "Sandbox:minusButtonHover";
        DownImage = "Sandbox:minusButtonDown";
        InactiveImage = "Sandbox:minusButtonInactive";
    };

    %controlPosition = getWord($spinnerExtent, 1) SPC $customLabelSpacing;

    %textEdit = new GuiTextEditCtrl()
    {
        Position = %controlPosition;
        Text = %startingValue;
        Extent = $intOptionExtent;
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "TextEditController";
        isContainer = "0";
        Profile = "GuiSpinnerProfile";
        tooltipprofile = "GuiToolTipProfile";
        hovertime = "1000";
    };

    %spinnerPosition = getWord(%textEdit.Extent, 0) SPC $customLabelSpacing;

    %spinnerUp = new GuiImageButtonCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiDefaultProfile";
        Position = %spinnerPosition;
        Extent = $spinnerExtent;
        MinExtent = "8 2";
        canSave = "1";
        Visible = "1";
        Active = "1";
        hovertime = "1000";
        toolTipProfile = "GuiToolTipProfile";
        groupNum = "-1";
        buttonType = "PushButton";
        useMouseEvents = "0";
        NormalImage = "Sandbox:plusButtonNormal";
        HoverImage = "Sandbox:plusButtonHover";
        DownImage = "Sandbox:plusButtonDown";
        InactiveImage = "Sandbox:plusButtonInactive";
    };

    %textEdit.validate = %textEdit @ ".updateToy();";

    %container.add(%spinnerDown);
    %container.add(%textEdit);
    %container.add(%spinnerUp);

    ToyCustomControls.add(%container);

    return %container;
}

//-----------------------------------------------------------------------------

function TextEditController::updateToy(%this)
{
    if (%this.toy $= "")
        return;
        
    if (%this.callback !$= "" && %this.getValue() !$= "")
    {
        %setter = "%this.toy." @ %this.callback @ "(" @ %this.getValue() @ ");";
        eval(%setter);
    }
    
    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}

//-----------------------------------------------------------------------------

function addSelectionOption( %entries, %label, %callback, %shouldReset)
{
    //%entries, %label, %position, %extent, %shouldReset, %callback
    %width = getWord(%extent, 0);
    %height = getWord(%extent, 1);
    %characterCount = strlen(%label);

    %labelWidth = %width + (%characterCount * 5);
    %labelExtent = %labelWidth SPC %height;
    %positionOffset = (%width + 15) SPC "1";

    %containerWidth = %labelWidth + 25;
    %containerHeight = getWord(%extent, 1) + 35;
    %containerExtent = %containerWidth SPC %containerHeight;

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %position;
        extent = %containerExtent;
        Profile = GuiTransparentProfile;
    };

    %menu = new GuiPopUpMenuCtrl()
    {
        class = "SelectionController";
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        isContainer = "0";
        Profile = "GuiPopUpMenuProfile";
        Position = "1 1";
        Extent = %extent;
        MinExtent = "8 2";
        Visible = "1";
        Active = "1";
        toolTipProfile = "GuiToolTipProfile";
        maxLength = "1024";
        maxPopupHeight = "200";
        sbUsesNAColor = "0";
        reverseTextList = "0";
        bitmapBounds = "16 16";
    };

    for (%i = 0; %i < getWordCount(%entries); %i++)
    {
        %menu.add(getWord(%entries, %i), %i);
    }

    %menu.setSelected(0);

    %container.add(%menu);

    %labelControl = new GuiTextCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiTextProfile";
        Position = %positionOffset;
        Extent = %labelExtent;
        MinExtent = "8 2";
        canSave = "0";
        Visible = "1";
        Active = "0";
        tooltipprofile = "GuiToolTipProfile";
        tooltipWidth = "0";
        text = %label;
        maxLength = "255";
        truncate = "0";
    };

    %container.add(%labelControl);

    ToyCustomControls.add(%container);

    return %container;
}

//-----------------------------------------------------------------------------

function SelectionController::onSelect(%this)
{
    if (%this.toy $= "")
        return;

    if (%this.callback !$= "")
    {
        %value = %this.getTextById(%this.getSelected());
        %setter = "%this.toy." @ %this.callback @ "(\"" @ %this.getValue() @ "\");";
        eval(%setter);
    }

    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}