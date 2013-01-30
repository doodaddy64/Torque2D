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

function addFlagOption( %label, %position, %extent, %toyController, %shouldReset, %callback, %startingValue)
{
    %containerWidth = getWord(%extent, 0) + 35;
    %containerHeight = getWord(%extent, 1) + 35;
    %containerExtent = %containerWidth SPC %containerHeight;

    %container = new GuiControl()
    {
        isContainer = 1;
        position = %position;
        extent = %containerExtent;
        Profile = GuiTransparentProfile;
    };

    %checkbox = new GuiCheckBoxCtrl()
    {
        Position = "1 1";
        Extent = "20 20";
        Profile = "GuiCheckBoxProfile";
        toy = %toyController;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "CheckboxController";
        tooltipprofile = "GuiToolTipProfile";
    };

    %checkBox.setValue(%startingValue);
    %checkbox.command = %checkbox @ ".updateToy();";

    %container.add(%checkBox);

    %labelControl = new GuiTextCtrl()
    {
        canSaveDynamicFields = "0";
        isContainer = "0";
        Profile = "GuiTextProfile";
        Position = "30 1";
        Extent = %extent;
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

function CheckboxController::updateToy(%this)
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

function addIntegerOption( %label, %position, %extent, %toyController, %shouldReset, %callback, %startingValue)
{
    %containerWidth = getWord(%extent, 0) + 100;
    %containerHeight = getWord(%extent, 1);
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
        toy = %toyController;
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
        Position = "30 0";
        Extent = "80 20";
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
        
    if (%this.callback !$= "")
    {
        %setter = "%this.toy." @ %this.callback @ "(" @ %this.getValue() @ ");";
        eval(%setter);
    }
    
    if (%this.shouldResetToy && %this.toy.isMethod("reset"))
        %this.toy.reset();
}

//-----------------------------------------------------------------------------

function createRangeControl( %name, %label, %position, %min, %max, %variable )
{
}