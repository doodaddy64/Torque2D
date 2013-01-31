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
$customLabelSpacing = "18";
$customOptionSpacing = "25";
$customContainerExtent = "0 0";
$containerXPosition = "10";
$flagOptionExtent = "60 25";
$buttonOptionExtent = "60 25";
$intOptionExtent = "0 0";
$listOptionExtent = "0 0";
$customControlCount = 0;

//-----------------------------------------------------------------------------

function createCustomLabel(%text, %width)
{
    %labelWidth = %width + (%characterCount * 5);
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
    //%label, %position, %extent, %shouldReset, %callback, %startingValue
    %width = getWord(%extent, 0);
    %height = getWord(%extent, 1);
    %characterCount = strlen(%label);

    %labelWidth = %width + (%characterCount * 5);
    %labelExtent = %labelWidth SPC %height;
    %positionOffset = (%width + 10) SPC "1";

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

    %textEdit = new GuiTextEditCtrl()
    {
        Position = "1 1";
        Text = %startingValue;
        Extent = %extent;
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "TextEditController";
        isContainer = "0";
        Profile = "GuiNumberEditProfile";
        tooltipprofile = "GuiToolTipProfile";
        hovertime = "1000";
        text = %startingValue;
    };

    %textEdit.validate = %textEdit @ ".updateToy();";
    %container.add(%textEdit);

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

//-----------------------------------------------------------------------------

function addRangeOption( %label, %position, %extent, %range, %ticks, %shouldReset, %callback, %startingValue)
{
    %width = getWord(%extent, 0);
    %height = getWord(%extent, 1);
    %characterCount = strlen(%label);

    %labelWidth = %width + (%characterCount * 5);
    %labelExtent = %labelWidth SPC "15";
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

    %labelControl = new GuiTextCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiTextProfile";
        Position = "1 1";
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

    %slider = new GuiSliderCtrl()
    {
        class = "SliderController";
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        Position = "1 16";
        Profile = "GuiSliderProfile";
        Extent = %extent;
        Range = %range;
        Ticks = %ticks;
        Value = %startingValue;
    };

    %slider.command = %slider @ ".updateToy();";

    %container.add(%slider);

    ToyCustomControls.add(%container);

    return %container;
}

//-----------------------------------------------------------------------------

function SliderController::updateToy(%this)
{
    echo("@@@ Updating slider controller");
    if (%this.toy $= "")
        return;

    if (%this.callback !$= "")
    {
        %setter = "%this.toy." @ %this.callback @ "(" @ %this.getValue() @ ");";
        echo("@@@ Setter: " @ %setter);
        eval(%setter);
    }

    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}

/*
<GuiTextEditCtrl
    Name="Lt_PowerField"
    canSaveDynamicFields="0"
    isContainer="0"
    Profile="GuiSpinnerProfile"
    HorizSizing="right"
    VertSizing="bottom"
    Position="150 0"
    Extent="80 25"
    MinExtent="8 2"
    canSave="1"
    Visible="1"
    Active="1"
    hovertime="1000"
    toolTipProfile="GuiToolTipProfile"
    toolTip="Set the power of the Launcher between 1 and 100."
    text="10"
    maxLength="1024"
    historySize="0"
    password="0"
    tabComplete="0"
    sinkAllKeyEvents="0"
    passwordMask="*"
    truncate="0" />
<GuiImageButtonCtrl
    Name="Lt_PowerDownButton"
    canSaveDynamicFields="0"
    isContainer="0"
    Profile="GuiDefaultProfile"
    HorizSizing="right"
    VertSizing="bottom"
    Position="128 0"
    Extent="22 25"
    MinExtent="8 2"
    canSave="1"
    Visible="1"
    Active="1"
    hovertime="1000"
    toolTipProfile="GuiToolTipProfile"
    toolTip="Set the power of the Launcher between 1 and 100."
    groupNum="-1"
    buttonType="PushButton"
    useMouseEvents="0"
    NormalImage="@asset={EditorAssets}:minusImageMap"
    HoverImage="@asset={EditorAssets}:minus_hImageMap"
    DownImage="@asset={EditorAssets}:minus_dImageMap"
    InactiveImage="@asset={EditorAssets}:minus_iImageMap" />
<GuiImageButtonCtrl
    Name="Lt_PowerUpButton"
    canSaveDynamicFields="0"
    isContainer="0"
    Profile="GuiDefaultProfile"
    HorizSizing="right"
    VertSizing="bottom"
    Position="230 0"
    Extent="22 25"
    MinExtent="8 2"
    canSave="1"
    Visible="1"
    Active="1"
    hovertime="1000"
    toolTipProfile="GuiToolTipProfile"
    toolTip="Set the power of the Launcher between 1 and 100."
    groupNum="-1"
    buttonType="PushButton"
    useMouseEvents="0"
    NormalImage="@asset={EditorAssets}:plusImageMap"
    HoverImage="@asset={EditorAssets}:plus_hImageMap"
    DownImage="@asset={EditorAssets}:plus_dImageMap"
    InactiveImage="@asset={EditorAssets}:plus_iImageMap" />
*/