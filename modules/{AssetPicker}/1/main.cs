//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function initializeAssetPicker(%scopeSet)
{
    //-----------------------------------------------------------------------------
    // Execute scripts
    //-----------------------------------------------------------------------------
    exec("./scripts/gui.cs");
    
    //-----------------------------------------------------------------------------
    // Load GUIs
    //----------------------------------------------------------------------------- 
    %scopeSet.add( TamlRead("./gui/assetPicker.gui.taml") );

    //-----------------------------------------------------------------------------
    // Initialization
    //----------------------------------------------------------------------------- 
    if (!isObject(AssetPicker))
    {
        new ScriptObject(AssetPicker)
        {
            assetType = "All";
            tagFilter = "Any";
            requestingTool = "";
        };
        
        %scopeSet.add(AssetPicker);
    }
}

function destroyAssetPicker()
{
}

function AssetPicker::onAdd(%this)
{
}

function AssetPicker::onRemove(%this)
{
    AssetPicker.close();
}