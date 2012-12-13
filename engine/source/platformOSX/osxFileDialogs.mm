//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import "platformOSX/platformOSX.h"
#import "platformOSX/osxCocoaUtilities.h"
#include "platform/nativeDialogs/fileDialog.h"
#include "io/resource/resourceManager.h"

/*
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
 */

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::Execute not yet implemented")
bool FileDialog::Execute()
{
    NSArray* nsFileArray;

    if (mData.mStyle & FileDialogData::FDS_OPEN)
    {
        nsFileArray = [NSOpenPanel showOpenPanel:&mData];
    }
    else if (mData.mStyle & FileDialogData::FDS_SAVE)
    {
        nsFileArray = [NSSavePanel showSavePanel:&mData];
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
        for(U32 i = 0; i < [nsFileArray count]; i++)
        {
            NSURL* fileURL = [nsFileArray objectAtIndex:i];

            const UTF8* file = [[fileURL absoluteString] UTF8String];
            setDataField(StringTable->insert("files"), Con::getIntArg(i), StringTable->insert(file));
        }

        setDataField(StringTable->insert("fileCount"), NULL, Con::getIntArg([nsFileArray count]));
    }
    else
    {
        NSURL* fileURL = [nsFileArray objectAtIndex:0];

        const UTF8* file = [[fileURL absoluteString] UTF8String];

        mData.mFile = StringTable->insert(file);
    }

    return true;
}


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
