//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

/// <summary>
/// Sets up the AssetTextEdit object with the string it will display when empty
/// and sets initial values for some internal fields.
/// </summary>
/// <param name="emptyString">String that is displayed when the text edit field is empty.</param>
function AssetTextEdit::initialize(%this, %emptyString)
{
    %this.emptyString = %emptyString;
    %this.setText(%this.emptyString);
    %this.emptyFlag = true;
}

/// <summary>
/// Causes the AssetTextEdit to update its "empty" state based on its current value.
/// </summary>
function AssetTextEdit::update(%this)
{
    if (%this.getText() $= "" || %this.getText() $= %this.emptyString)
        %this.emptyFlag = true;
    else
        %this.emptyFlag = false;
}

/// <summary>
/// Returns whether the emptyFlag has been set for the AssetTextEdit object
/// </summary>
function AssetTextEdit::isEmpty(%this)
{
    return %this.emptyFlag;
}

/// <summary>
/// A modified version of setText for AssetTextEdit objects that causes the empty state to be
/// updated whenever the text value is set.
/// </summary>
function AssetTextEdit::setText(%this, %text)
{
    %this.text = %text;  
    %this.update();
}

function AssetTextEdit::onLoseFirstResponder(%this)
{
    if (%this.getText() $= "")
    {
        %this.text = %this.emptyString; 
        %this.emptyFlag = true; 
    }
}

function AssetTextEdit::onMouseDown(%this, %modifier, %globalPoint, %mouseClickCount)
{
    if (%this.getText() $= %this.emptyString)
        %this.text = "";
    %this.update();
    %this.selectAllText();
}
