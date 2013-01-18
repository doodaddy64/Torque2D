//-----------------------------------------------------------------------------
// 3 Step Studio
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function initializeImageEditor(%scopeSet)
{  
    //-----------------------------------------------------------------------------
    // Execute scripts
    //-----------------------------------------------------------------------------
    execPrefs("imageEditorPrefs.cs");
    exec("./scripts/editor.cs");
    exec("./scripts/globalFunctions.cs");
    exec("./scripts/gui.cs");
    exec("./scripts/editorPreviewWindow.cs");
    exec("./scripts/cursors.cs");
   
    //-----------------------------------------------------------------------------
    // Load GUIs
    //-----------------------------------------------------------------------------  
    %scopeSet.add( TamlRead("./gui/imageBuilderGui.gui.taml") );
    
    //-----------------------------------------------------------------------------
    // Initialization
    //----------------------------------------------------------------------------- 
    $T2D::ImageSpec = "All Supported Graphics (*.jpg;*.jpeg;*.png;)|*.png;*.jpg;*.jpeg|";
    
    // Create the editor object for namespace usage
    if (!isObject(ImageEditor))
    {
        new ScriptObject(ImageEditor)
        {
            loadingPreview = false;
        };
        
        %scopeSet.add(ImageEditor);
    }
}

function destroyImageEditor()
{
   // Export Preferences.
   echo("Exporting Image Editor preferences.");
   export("$imageEditor::*", "imageEditorPrefs.cs", false, false);
}
