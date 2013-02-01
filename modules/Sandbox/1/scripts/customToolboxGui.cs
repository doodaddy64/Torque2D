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

Sandbox.customLabelHeight = "15";
Sandbox.customLabelWidth = "220";
Sandbox.customLabelSpacing = "18";
Sandbox.customOptionSpacing = "15";
Sandbox.customContainerExtent = "250 3";
Sandbox.containerXPosition = "0";
Sandbox.flagOptionExtent = "240 35";
Sandbox.buttonOptionExtent = "240 35";
Sandbox.spinnerExtent = "22 25";
Sandbox.intOptionExtent = "196 25";
Sandbox.listOptionExtent = "240 25";
Sandbox.customControlCount = "0";
Sandbox.lastControlBottom = "0";

//-----------------------------------------------------------------------------

function resetCustomControls()
{
    Sandbox.lastControlBottom = "0";
    Sandbox.customControlCount = 0;
}

//-----------------------------------------------------------------------------

function createCustomLabel(%text)
{
    %labelWidth = Sandbox.customLabelWidth + (%characterCount * 5);
    %labelExtent = %labelWidth SPC Sandbox.customLabelHeight;

    %labelControl = new GuiTextCtrl()
    {
        text = %text;
        Extent = %labelExtent;
        HorizSizing = "relative";
        VertSizing = "relative";
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
    %verticalOffset = (Sandbox.customOptionSpacing + %index) + Sandbox.lastControlBottom;
    %position = Sandbox.containerXPosition SPC %verticalOffset;
    return %position;
}

//-----------------------------------------------------------------------------

function addFlagOption( %label, %callback, %startingValue, %shouldReset)
{
    %containerPosition = nextCustomControlPosition(Sandbox.customControlCount);

    %customX = getWord(Sandbox.customContainerExtent, 0);
    %customY = getWord(Sandbox.customContainerExtent, 1) + getWord(Sandbox.flagOptionExtent, 1);

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
        HorizSizing = "relative";
        VertSizing = "relative";
    };

    %button = new GuiButtonCtrl()
    {
        canSaveDynamicFields = "0";
        HorizSizing = "relative";
        VertSizing = "relative";
        isContainer = "0";
        Profile = "BlueButtonProfile";
        Position = "0 0";
        Extent = Sandbox.flagOptionExtent;
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

    %button.setStateOn(%startingValue);
    %button.command = %button @ ".updateToy();";

    %container.add(%button);

    ToyCustomControls.add(%container);

    Sandbox.lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

    Sandbox.customControlCount++;
}

//-----------------------------------------------------------------------------

function FlagController::updateToy(%this)
{
    if (%this.toy $= "")
        return;
        
    if (%this.callback !$= "")
    {
        %setter = "%this.toy." @ %this.callback @ "(" @ %this.getStateOn() @ ");";

        eval(%setter);
    }
    
    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}

//-----------------------------------------------------------------------------

function addButtonOption( %label, %callback, %shouldReset)
{
    %containerPosition = nextCustomControlPosition(Sandbox.customControlCount);

    %customX = getWord(Sandbox.customContainerExtent, 0);
    %customY = getWord(Sandbox.customContainerExtent, 1) + getWord(Sandbox.buttonOptionExtent, 1);

    %container = new GuiControl()
    {
        isContainer = 1;
        HorizSizing = "relative";
        VertSizing = "relative";
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %button = new GuiButtonCtrl()
    {
        canSaveDynamicFields = "0";
        HorizSizing = "relative";
        VertSizing = "relative";
        isContainer = "0";
        Profile = "BlueButtonProfile";
        Position = "0 0";
        Extent = Sandbox.buttonOptionExtent;
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

    Sandbox.lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

    Sandbox.customControlCount++;
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

function addIntegerOption( %label, %min, %max, %step, %callback, %startingValue, %shouldReset)
{
    %customLabel = createCustomLabel(%label);

    %containerPosition = nextCustomControlPosition(Sandbox.customControlCount);

    %customX = getWord(Sandbox.customContainerExtent, 0);
    %customY = getWord(Sandbox.customContainerExtent, 1) + getWord(Sandbox.intOptionExtent, 1) + Sandbox.customLabelHeight;

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %containerPosition;
        HorizSizing = "relative";
        VertSizing = "relative";
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %container.add(%customLabel);

    %spinnerPosition = "1" SPC Sandbox.customLabelSpacing;

    %spinnerDown = new GuiImageButtonCtrl()
    {
        Action = "decrease";
        Class = "SpinnerController";
        Step = %step;
        HorizSizing = "relative";
        VertSizing = "relative";
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiDefaultProfile";
        Position = %spinnerPosition;
        Extent = Sandbox.spinnerExtent;
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

    %controlPosition = (getWord(Sandbox.spinnerExtent, 0) + 1) SPC Sandbox.customLabelSpacing;

    %textEdit = new GuiTextEditCtrl()
    {
        Position = %controlPosition;
        HorizSizing = "relative";
        VertSizing = "relative";
        min = %min;
        max = %max;
        Text = %startingValue;
        Extent = Sandbox.intOptionExtent;
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "TextEditController";
        isContainer = "0";
        Profile = "GuiSpinnerProfile";
        tooltipprofile = "GuiToolTipProfile";
        hovertime = "1000";
    };

    %spinnerPosition = (getWord(%textEdit.Extent, 0) + getWord(%textEdit.position, 0)) SPC Sandbox.customLabelSpacing;

    %spinnerUp = new GuiImageButtonCtrl()
    {
        Action = "increase";
        HorizSizing = "relative";
        VertSizing = "relative";
        Class = "SpinnerController";
        Step = %step;
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiDefaultProfile";
        Position = %spinnerPosition;
        Extent = Sandbox.spinnerExtent;
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

    Sandbox.lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

    Sandbox.customControlCount++;
}

//-----------------------------------------------------------------------------

function SpinnerController::updateTarget(%this)
{
    %target = %this.target;

    if (%this.action $= "increase" && %target.getText() < %target.max)
    {
        %value = %target.getText();
        %value += %this.step;
        %target.setText(%value);
    }
    else if (%this.action $= "decrease" && %target.getText() > %target.min)
    {
        %value = %target.getText();
        %value -= %this.step;
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
    %customLabel = createCustomLabel(%label);

    %containerPosition = nextCustomControlPosition(Sandbox.customControlCount);

    %customX = getWord(Sandbox.customContainerExtent, 0);
    %customY = getWord(Sandbox.customContainerExtent, 1) + getWord(Sandbox.listOptionExtent, 1) + Sandbox.customLabelHeight;

    %container = new GuiControl()
    {
        isContainer = 1;
        HorizSizing = "relative";
        VertSizing = "relative";
        position = %containerPosition;
        extent = %customX SPC %customY;
        Profile = GuiTransparentProfile;
    };

    %container.add(%customLabel);

    %controlPosition = "0" SPC Sandbox.customLabelSpacing;

    %menu = new GuiPopUpMenuCtrl()
    {
        class = "SelectionController";
        HorizSizing = "relative";
        VertSizing = "relative";        
        toy = Sandbox.ActiveToy.ScopeSet;
        shouldResetToy = %shouldReset;
        callback = %callback;
        isContainer = "0";
        Profile = "GuiPopUpMenuProfile";
        Position = %controlPosition;
        Extent = Sandbox.listOptionExtent;
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

    for (%i = 0; %i < getUnitCount(%entries, ","); %i++)
    {
        %menu.add(getUnit(%entries, %i, ","), %i);
    }

    %menu.setSelected(0);

    %container.add(%menu);

    ToyCustomControls.add(%container);

    Sandbox.lastControlBottom = getWord(%container.position, 1) + getWord(%container.extent, 1);

    Sandbox.customControlCount++;
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