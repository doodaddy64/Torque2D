//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function startTesting(%scopeSet)
{
    //-----------------------------------------------------------------------------
    // Execute scripts
    //-----------------------------------------------------------------------------
    exec("./scripts/setup.cs");
    exec("./scripts/spriteTests.cs");
    exec("./scripts/menuBuilder.cs");
    exec("./scripts/fileDialogTests.cs");

    //-----------------------------------------------------------------------------
    // Load GUIs
    //----------------------------------------------------------------------------- 

    //-----------------------------------------------------------------------------
    // Initialization
    //----------------------------------------------------------------------------- 

    createDefaultProfile();
    createDefaultCanvas();
    createTestBindings();

    //runSpriteTests();
    
    runDialogTests();
    //runGUITests();
    //runRandomTests();
    
    if (isObject(moveMap))
        moveMap.push();
}

function endTesting()
{
    if (isObject(moveMap))
    {
        moveMap.pop();
        moveMap.delete();
    }
    
    endSpriteTests();
}