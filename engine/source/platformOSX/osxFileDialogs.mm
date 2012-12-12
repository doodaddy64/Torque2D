//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import "platformOSX/platformOSX.h"
#import "platformOSX/osxCocoaUtilities.h"
#include "platform/nativeDialogs/fileDialog.h"
#include "console/consoleTypes.h"
#include "io/resource/resourceManager.h"

//-----------------------------------------------------------------------------
// Builds and runs a NSOpenPanel. It will return the results of the user's
// interaction as a NSArray of file URLs
static NSArray* ShowOSXOpenFileDialog(FileDialogData &mData)
{
    NSOpenPanel* panel = [NSOpenPanel openPanel];

    bool chooseDir = (mData.mStyle & FileDialogData::FDS_BROWSEFOLDER);

    // User freedom niceties
    [panel setCanCreateDirectories:YES];
    [panel setCanSelectHiddenExtension:YES];
    [panel setTreatsFilePackagesAsDirectories:YES];
    [panel setAllowsMultipleSelection:(mData.mStyle & FileDialogData::FDS_MULTIPLEFILES)];
    [panel setCanChooseFiles:!chooseDir];
    [panel setCanChooseDirectories:chooseDir];

    if (chooseDir)
    {
        [panel setPrompt:@"Choose"];
        [panel setTitle:@"Choose Folder"];
    }

    NSString *initialFile = [[NSString stringWithUTF8String:mData.mDefaultFile] lastPathComponent];

    // We only use mDefaultDir if mDefault path is not set.
    NSString *dir;

    if (dStrlen(mData.mDefaultPath) < 1)
        dir = [[NSString stringWithUTF8String:mData.mDefaultFile] stringByDeletingLastPathComponent];
    else
        dir = [NSString stringWithUTF8String: mData.mDefaultPath];

    [panel setDirectoryURL:[NSURL fileURLWithPath:dir isDirectory:YES]];

    [panel setFilters:mData.mFilters];

    NSMutableArray *array = [NSMutableArray arrayWithCapacity:10];


}

//-----------------------------------------------------------------------------
// Builds and runs a NSOpenPanel. It will return the results of the user's
// interaction as a NSArray of file URLs
static NSArray* ShowOSXSaveFileDialog(FileDialogData &mData)
{
    NSSavePanel* savePanel = [NSSavePanel savePanel];
}

//-----------------------------------------------------------------------------
// MICH NOTE: Ok. There is a lot of repeat code in this file. Aside from the
// most important part ::execute, the code found in each of the classes
// are duplicated across all platforms. This is not platform abstraction,
// just bad practice. In our next platform cleanup pass, this whole section
// should be reworked. Due to the Cocoa deadline, I'll let it go for now,
// but I will be coming back to this early in the next phase.

IMPLEMENT_CONOBJECT(FileDialog);
IMPLEMENT_CONOBJECT(OpenFileDialog);
IMPLEMENT_CONOBJECT(SaveFileDialog);
IMPLEMENT_CONOBJECT(OpenFolderDialog);

//-----------------------------------------------------------------------------
#pragma message ("FileDialogData constructor not yet implemented")
FileDialogData::FileDialogData()
{
    mDefaultPath = StringTable->insert(Con::getVariable("Tools::FileDialogs::LastFilePath"));

    if (mDefaultPath == StringTable->EmptyString || !Platform::isDirectory(mDefaultPath))
        mDefaultPath = Platform::getCurrentDirectory();

    mDefaultFile = StringTable->insert("");
    mFilters = StringTable->insert("");
    mFile = StringTable->insert("");
    mTitle = StringTable->insert("");

    mStyle = 0;
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialogData destructor not yet implemented")
FileDialogData::~FileDialogData()
{
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog constructor not yet implemented")
FileDialog::FileDialog() : mData()
{
    // Default to File Must Exist Open Dialog style
    mData.mStyle = FileDialogData::FDS_OPEN | FileDialogData::FDS_MUSTEXIST;
    mChangePath = false;
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog destructor not yet implemented")
FileDialog::~FileDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::initPersistFields not yet implemented")
void FileDialog::initPersistFields()
{
    // Let's relocate this to a more platform agonstic location in the next cleanup
    addProtectedField("DefaultPath", TypeString, Offset(mData.mDefaultPath, FileDialog), &setDefaultPath, &defaultProtectedGetFn, "Default Path when Dialog is shown");
    addProtectedField("DefaultFile", TypeString, Offset(mData.mDefaultFile, FileDialog), &setDefaultFile, &defaultProtectedGetFn, "Default File when Dialog is shown");
    addProtectedField("FileName", TypeString, Offset(mData.mFile, FileDialog), &setFile, &defaultProtectedGetFn, "Default File when Dialog is shown");
    addProtectedField("Filters", TypeString, Offset(mData.mFilters, FileDialog), &setFilters, &defaultProtectedGetFn, "Default File when Dialog is shown");
    addField("Title", TypeString, Offset(mData.mTitle, FileDialog), "Default File when Dialog is shown");
    addProtectedField("ChangePath", TypeBool, Offset(mChangePath, FileDialog), &setChangePath, &getChangePath, "True/False whether to set the working directory to the directory returned by the dialog");
    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::Execute not yet implemented")
bool FileDialog::Execute()
{
    NSArray* nsFileArray;

    if (mData.mStyle & FileDialogData::FDS_OPEN)
    {
        nsFileArray = [NSOpenPanel showOpenPanel];
    }
    else if (mData.mStyle & FileDialogData::FDS_SAVE)
    {
        nsFileArray = [NSSavePanel showSavePanel];
    }
    else
    {
        Con::errorf("Bad File Dialog Setup.");
        return false;
    }

    // If multiple file selection was allowed and the dialog did grab multiple files
    // loop through and add them
    if ((mData.mStyle & FileDialogData::FDS_MULTIPLEFILES) && [nsFileArray count] >= 1)
    {

    }
    else
    {
        // Multiple file selection was not allowed or only one file was selected
    }

    return true;
}

//-----------------------------------------------------------------------------
// Re-implemented based on Windows and old OS X. Honestly, why even have this
// TODO: Consider removing in next cleanup
bool FileDialog::setFilters(void* obj, const char* data)
{
    if (!data)
        return true;

    return true;
};

//-----------------------------------------------------------------------------
// Default Path Property - String Validated on Write
bool FileDialog::setDefaultPath(void* obj, const char* data)
{
    if (!data || !dStrncmp(data, "", 1))
        return true;

    // Expand the path to something fully qualified
    static char szPathValidate[512];

    Platform::makeFullPathName(data, szPathValidate, sizeof(szPathValidate));

    // Check to make sure the path is valid
    ResourceManager->addPath(szPathValidate, true);
    if (Platform::isDirectory(szPathValidate))
    {
        // Finally, assign in proper format.
        FileDialog *pDlg = static_cast<FileDialog*>(obj);
        pDlg->mData.mDefaultPath = StringTable->insert(szPathValidate);
    }

    return false;
}

//-----------------------------------------------------------------------------
// Default Path Property - String Validated on Write
bool FileDialog::setDefaultFile(void* obj, const char* data)
{
    if (!data || !dStrncmp(data, "", 1))
        return true;

    // Copy and Backslash the path (Windows dialogs are VERY picky about this format)
    static char szPathValidate[512];
    Platform::makeFullPathName(data,szPathValidate, sizeof(szPathValidate));

    // Finally, assign in proper format.
    FileDialog *pDlg = static_cast<FileDialog*>(obj);
    pDlg->mData.mDefaultFile = StringTable->insert(szPathValidate);

    return false;
};

//-----------------------------------------------------------------------------
// ChangePath Property - Change working path on successful file selection
bool FileDialog::setChangePath(void* obj, const char* data)
{
    bool bMustExist = dAtob(data);

    FileDialog *pDlg = static_cast<FileDialog*>(obj);

    if (bMustExist)
        pDlg->mData.mStyle |= FileDialogData::FDS_CHANGEPATH;
    else
        pDlg->mData.mStyle &= ~FileDialogData::FDS_CHANGEPATH;

    return true;
};

//-----------------------------------------------------------------------------
// ChangePath Property - Get working path on successful file selection
const char* FileDialog::getChangePath(void* obj, const char* data)
{
    FileDialog *pDlg = static_cast<FileDialog*>(obj);

    if (pDlg->mData.mStyle & FileDialogData::FDS_CHANGEPATH)
        return StringTable->insert("true");
    else
        return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
// This returns false on every platform, so why use it?
// TODO: Consider removing in the next platform cleanup
bool FileDialog::setFile(void* obj, const char* data)
{
   return false;
};

//-----------------------------------------------------------------------------
OpenFileDialog::OpenFileDialog()
{
    // Default File Must Exist
    mData.mStyle = FileDialogData::FDS_OPEN | FileDialogData::FDS_MUSTEXIST;
}

//-----------------------------------------------------------------------------
OpenFileDialog::~OpenFileDialog()
{
    mMustExist = true;
    mMultipleFiles = false;
}

//-----------------------------------------------------------------------------
void OpenFileDialog::initPersistFields()
{
    addProtectedField("MustExist", TypeBool, Offset(mMustExist, OpenFileDialog), &setMustExist, &getMustExist, "True/False whether the file returned must exist or not");
    addProtectedField("MultipleFiles", TypeBool, Offset(mMultipleFiles, OpenFileDialog), &setMultipleFiles, &getMultipleFiles, "True/False whether multiple files may be selected and returned or not");

    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
bool OpenFileDialog::setMustExist(void* obj, const char* data)
{
    bool bMustExist = dAtob(data);

    OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);

    if (bMustExist)
        pDlg->mData.mStyle |= FileDialogData::FDS_MUSTEXIST;
    else
        pDlg->mData.mStyle &= ~FileDialogData::FDS_MUSTEXIST;

    return true;
}

//-----------------------------------------------------------------------------
const char* OpenFileDialog::getMustExist(void* obj, const char* data)
{
    OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);

    if (pDlg->mData.mStyle & FileDialogData::FDS_MUSTEXIST)
        return StringTable->insert("true");
    else
        return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
bool OpenFileDialog::setMultipleFiles(void* obj, const char* data)
{
    bool bMustExist = dAtob(data);

    OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);

    if (bMustExist)
        pDlg->mData.mStyle |= FileDialogData::FDS_MULTIPLEFILES;
    else
        pDlg->mData.mStyle &= ~FileDialogData::FDS_MULTIPLEFILES;

    return true;
}

//-----------------------------------------------------------------------------
const char* OpenFileDialog::getMultipleFiles(void* obj, const char* data)
{
    OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);

    if (pDlg->mData.mStyle & FileDialogData::FDS_MULTIPLEFILES)
        return StringTable->insert("true");
    else
        return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
SaveFileDialog::SaveFileDialog()
{
    // Default File Must Exist
    mData.mStyle = FileDialogData::FDS_SAVE | FileDialogData::FDS_OVERWRITEPROMPT;
    mOverwritePrompt = true;
}

//-----------------------------------------------------------------------------
SaveFileDialog::~SaveFileDialog()
{
}

//-----------------------------------------------------------------------------
void SaveFileDialog::initPersistFields()
{
    addProtectedField("OverwritePrompt", TypeBool, Offset(mOverwritePrompt, SaveFileDialog), &setOverwritePrompt, &getOverwritePrompt, "True/False whether the dialog should prompt before accepting an existing file name");

    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
bool SaveFileDialog::setOverwritePrompt(void* obj, const char* data)
{
    bool bOverwrite = dAtob(data);

    SaveFileDialog *pDlg = static_cast<SaveFileDialog*>(obj);

    if (bOverwrite)
        pDlg->mData.mStyle |= FileDialogData::FDS_OVERWRITEPROMPT;
    else
        pDlg->mData.mStyle &= ~FileDialogData::FDS_OVERWRITEPROMPT;

    return true;
}

//-----------------------------------------------------------------------------
const char* SaveFileDialog::getOverwritePrompt(void* obj, const char* data)
{
    SaveFileDialog *pDlg = static_cast<SaveFileDialog*>(obj);

    if (pDlg->mData.mStyle & FileDialogData::FDS_OVERWRITEPROMPT)
        return StringTable->insert("true");
    else
        return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
OpenFolderDialog::OpenFolderDialog()
{
    mData.mStyle = FileDialogData::FDS_OPEN | FileDialogData::FDS_OVERWRITEPROMPT | FileDialogData::FDS_BROWSEFOLDER;

    mMustExistInDir = "";
}

//-----------------------------------------------------------------------------
OpenFolderDialog::~OpenFolderDialog()
{
}

//-----------------------------------------------------------------------------
void OpenFolderDialog::initPersistFields()
{
    addField("fileMustExist", TypeFilename, Offset(mMustExistInDir, OpenFolderDialog), "File that must in selected folder for it to be valid");

    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertOK not yet implemented")
void Platform::AlertOK(const char *windowTitle, const char *message)
{
    
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertOKCancel not yet implemented")
bool Platform::AlertOKCancel(const char *windowTitle, const char *message)
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertRetry not yet implemented")
bool Platform::AlertRetry(const char *windowTitle, const char *message)
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertYesNo not yet implemented")
bool Platform::AlertYesNo(const char *windowTitle, const char *message)
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::messageBox not yet implemented")
S32 Platform::messageBox(const UTF8 *title, const UTF8 *message, MBButtons buttons, MBIcons icon)
{
    return 0;
}
