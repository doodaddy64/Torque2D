//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function toggleToolbox(%make)
{
    // Finish if being released.
    if ( !%make )
        return;
        
    // Finish if the console is awake.
    if ( ConsoleDialog.isAwake() )
        return;       
        
    // Is the toolbox awake?
    if ( ToolboxDialog.isAwake() )
    {
        // Yes, so deactivate it.
        if ( $enableDirectInput )
            activateKeyboard();
        Canvas.popDialog(ToolboxDialog);        
        return;
    }
    
    // Activate it.
    if ( $enableDirectInput )
        deactivateKeyboard();    
    Canvas.pushDialog(ToolboxDialog, 2);         
    ToySelectList.setFirstResponder();
}

//-----------------------------------------------------------------------------

function ToySelectList::onSelect(%this)
{
    // Fetch the index.
    %index = %this.getSelected();
    
    // Finish if already selected.
    if ( %index == $activeToy )
        return;
    
    // Load the selected toy.
    loadToy( %index );
}