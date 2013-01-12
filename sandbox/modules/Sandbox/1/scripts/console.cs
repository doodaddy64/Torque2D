//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
function ConsoleEntry::eval()
{
    %text = trim(ConsoleEntry.getValue());

    if(strpos(%text, "(") == -1)
    {
        if(strpos(%text, "=") == -1 && strpos(%text, " ") == -1)
        {
            if(strpos(%text, "{") == -1 && strpos(%text, "}") == -1)
            {
                %text = %text @ "()";
            }
        }
    }

    %pos = strlen(%text) - 1;
    
    if(strpos(%text, ";", %pos) == -1 && strpos(%text, "}") == -1)
        %text = %text @ ";";

    echo("==>" @ %text);
    eval(%text);
    ConsoleEntry.setValue("");

}

function ToggleConsole(%make)
{
    if (%make)
    {
        if (ConsoleDialog.isAwake())
        {
            // Deactivate the console.
            if ( $enableDirectInput )
                activateKeyboard();

            Canvas.popDialog(ConsoleDialog);
        }
        else
        {
            if ( $enableDirectInput )
                deactivateKeyboard();
            
            Canvas.pushDialog(ConsoleDialog, 2);         
            ConsoleEntry.setFirstResponder();
        }
    }
}