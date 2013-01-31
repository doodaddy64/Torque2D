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
$customLabelWidth = "80";
$customLabelSpacing = "18";
$customOptionSpacing = "15";
$customContainerExtent = "190 3";
$containerXPosition = "0";
$flagOptionExtent = "100 25";
$buttonOptionExtent = "100 25";
$spinnerExtent = "22 25";
$intOptionExtent = "80 25";
$listOptionExtent = "180 25";
$customControlCount = "0";
$lastControlBottom = "0";

//-----------------------------------------------------------------------------

function createCustomLabel(%text)
{
    %labelWidth = $customLabelWidth + (%characterCount * 5);
    %labelExtent = %labelWidth SPC $customLabelHeight;

    %labelControl = new GuiTextCtrl()
    {
        text = %text;
        Extent = %labelExtent;
        Profile = "GuiTextProfile";
        canSaveDynamicFields = "0";
        isContainer = "0";
        Position = "3 0";
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
    %verticalOffset = ($customOptionSpacing + %index) + $lastControlBottom;
    %position = $containerXPosition SPC %verticalOffset;
    return %position;
}

//-----------------------------------------------------------------------------

function addFlagOption( %label, %callback, %startingValue, %shouldReset)
{
    %containerPosition = nextCustomControlPosition($customControlCount);

    %customX = getWord($customContainerExtent, 0);
    %customY = getWord($customContainerExtent, 1) + getWord($flagOptionExtent, 1);

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %button = new GuiButtonCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "BlueButtonProfile";
        Position = "0 0";
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

    $lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

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

    %customX = getWord($customContainerExtent, 0);
    %customY = getWord($customContainerExtent, 1) + getWord($buttonOptionExtent, 1);

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %button = new GuiButtonCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "BlueButtonProfile";
        Position = "0 0";
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

    $lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

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

    %customX = getWord($customContainerExtent, 0);
    %customY = getWord($customContainerExtent, 1) + getWord($intOptionExtent, 1) + $customLabelHeight;

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %container.add(%customLabel);

    %spinnerPosition = "1" SPC $customLabelSpacing;

    %spinnerDown = new GuiImageButtonCtrl()
    {
        Action = "decrease";
        Class = "SpinnerController";
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

    %controlPosition = (getWord($spinnerExtent, 0) + 1) SPC $customLabelSpacing;

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

    %spinnerPosition = (getWord(%textEdit.Extent, 0) + getWord(%textEdit.position, 0)) SPC $customLabelSpacing;

    %spinnerUp = new GuiImageButtonCtrl()
    {
        Action = "increase";
        Class = "SpinnerController";
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
    %spinnerDown.target = %textEdit;
    %spinnerUp.target = %textEdit;
    %spinnerDown.command = %spinnerDown @ ".updateTarget();";
    %spinnerUp.command = %spinnerUp @ ".updateTarget();";

    %container.add(%spinnerDown);
    %container.add(%textEdit);
    %container.add(%spinnerUp);

    ToyCustomControls.add(%container);

    $lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

    $customControlCount++;
}

//-----------------------------------------------------------------------------

function SpinnerController::updateTarget(%this)
{
    %target = %this.target;

    if (%this.action $= "increase")
    {
        %value = %target.getText();
        %value++;
        %target.setText(%value);
    }
    else if (%this.action $= "decrease")
    {
        %value = %target.getText();
        %value--;
        %target.setText(%value);
    }
    else
    {
        error("!!! SpinnerController " @ %this @ " not assigned an action!");
        return;
    }

    %target.updateToy();
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
    %customLabel = createCustomLabel(%label);

    %containerPosition = nextCustomControlPosition($customControlCount);

    %customX = getWord($customContainerExtent, 0);
    %customY = getWord($customContainerExtent, 1) + getWord($listOptionExtent, 1) + $customLabelHeight;

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %container.add(%customLabel);

    %controlPosition = "0" SPC $customLabelSpacing;

    %menu = new GuiPopUpMenuCtrl()
    {
        class = "SelectionController";
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        isContainer = "0";
        Profile = "GuiPopUpMenuProfile";
        Position = %controlPosition;
        Extent = $listOptionExtent;
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

    ToyCustomControls.add(%container);

    $lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

    $customControlCount++;
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