//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

function runDialogTests()
{
    echo("Running dialog tests");

    buildFileDialogMenu();
    $LastOpenedFile = "";
}

function buildFileDialogMenu()
{
    echo("Building file dialog tests menu");

    //set up %cmdctrl variable so that it matches OS standards
    %cmdCtrl = $platform $= "macos" ? "Cmd" : "Ctrl";

    %editmenu = new PopupMenu()
    {
        superClass = "MenuBuilder";
        barPosition = 1;
        barName = "File Dialogs";

        item[0] = "Open Image" TAB %cmdCtrl SPC "O" TAB "openImageAsset();";
        item[1] = "Save Image" TAB %cmdCtrl @ "S" TAB "saveImageAsset();";
    };

    if (!isObject(%editMenu))
    {
        error("### Could not create PopupMenu!");
    }
    else
    {
        %editMenu.attachToMenuBar();
    }
}

function openImageAsset()
{
    %assetFileSpec = "Asset Files (*.taml)|*.taml|";

    echo("Opening a file");

    %dlg = new OpenFileDialog()
    {
        Filters = %assetFileSpec;
        ChangePath = false;
        MustExist = true;
        MultipleFiles = false;
    };

    if (!isObject(%dlg))
    {
        error("Could not create OpenFileDialog");
        return;
    }

    echo("Executing OpenFileDialog");

    if (%dlg.Execute())
    {
        echo("Successfully picked a file");

        %file = %dlg.files[0];
        %fileOnlyName = fileName(%file);
        %fileOnlyBase = fileBase(%file);
        %fileOnlyExtension = fileExt(%file);

        echo("File: " @ %file);
        echo("Name: " @ %fileOnlyName);
        echo("Base: " @ %fileOnlyBase);
        echo("Extension: " @ %fileOnlyExtension);

        %fileExists = isFile(%file);

        if (%fileExists)
        {
            echo("File does exist. Let's save it to another location");
            $LastOpenedFile = %file;
        }
        else
        {
            echo("Did not properly find the file");
        }
    }
    else
    {
        error("Could not execute OpenFileDialog");
    }
}

function saveImageAsset()
{
    if ($LastOpenedFile $= "")
            return;

    %assetFileSpec = "Asset Files (*.taml)|*.taml|";

    %object = TamlRead($LastOpenedFile);

    %dlg = new SaveFileDialog()
    {
        Filters           = %assetFileSpec;
        DefaultPath       = filePath(%originalFile);
        DefaultFile       = fileName(%originalFile);
        ChangePath        = true;
        OverwritePrompt   = true;
    };

    if (%dlg.execute())
    {
        %file = %dlg.FileName;

        echo("Saving file to: " @ %file);

        TamlWrite(%object, %file);

        if (isFile(%file))
        {
            echo("File finished saving properly");
        }
    }
}
