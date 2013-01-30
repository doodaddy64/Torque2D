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

function createCheckBoxControl( %label, %position, %extent, %toyController, %shouldReset, %callback, %startingValue)
{
    %checkbox = new GuiCheckBoxCtrl()
    {
        Position = %position;
        Extent = %extent;
        Text = %label;
        Profile = "GuiCheckBoxProfile";
        toy = %toyController;
        shouldResetToy = %shouldReset;
        callback = %callback;
        class = "CheckboxController";        
        tooltipprofile = "GuiToolTipProfile";
    };
    %checkBox.setText(%label);
    %checkBox.setValue(%startingValue);
    %checkbox.command = %checkbox @ ".updateToy();";

    ToyCustomControls.add(%checkbox);


    return %checkbox;
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

function createNumberEditControl( %label, %position, %extent, %toyController, %shouldReset, %callback, %startingValue)
{
    %textEdit = new GuiTextEditCtrl()
    {
        Position = %position;
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

    ToyCustomControls.add(%textEdit);

    return %textEdit;
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