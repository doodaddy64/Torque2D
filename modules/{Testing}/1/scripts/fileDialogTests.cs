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

    %fileMenu = new PopupMenu()
    {
        superClass = "MenuBuilder";
        barPosition = 1;
        barName = "File";
    };
    %saveMenu = new PopupMenu()
    {
        superClass = "MenuBuilder";
        barPosition = 1;
        barName = "Save...";

        item[0] = "Last opened file" TAB %cmdCtrl SPC "O" TAB "saveTamlFile();";
    };

    %openMenu = new PopupMenu()
    {
        superClass = "MenuBuilder";
        barPosition = 1;
        barName = "Open...";

        item[0] = "Open One" TAB %cmdCtrl SPC "O" TAB "openOneTamlFile();";
        item[1] = "Open Multiple" TAB %cmdCtrl SPC "M" TAB "openMultipleTamlFiles();";
    };

    %fileMenu.attachToMenuBar();
    %fileMenu.insertSubMenu(0, "Open...", %openMenu);
    %fileMenu.insertSubMenu(1, "Save...", %saveMenu);
    //%openMenu.attachToMenuBar();
}

function openMultipleTamlFiles()
{
    %assetFileSpec = "Asset Files (*.taml)|*.taml|";

        echo("Opening a file");

        %dlg = new OpenFileDialog()
        {
            Filters = %assetFileSpec;
            ChangePath = false;
            MustExist = true;
            MultipleFiles = true;
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

            for (%i = 0; %i < %dlg.fileCount; %i++)
            {
                %file = %dlg.files[%i];
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
                    echo("isFile successful on: " @ %fileOnlyName);
                    $LastOpenedFile = %file;
                }
                else
                {
                    echo("Did not properly find the file path");
                }
            }
        }
        else
        {
            error("Could not execute OpenFileDialog");
        }
}
function openOneTamlFile()
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

        %file = %dlg.fileName;
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
            echo("isFile successful on: " @ %fileOnlyName);
            $LastOpenedFile = %file;
        }
        else
        {
            echo("Did not properly find the file path");
        }
    }
    else
    {
        error("Could not execute OpenFileDialog");
    }
}

function saveTamlFile()
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
