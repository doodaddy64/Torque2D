//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import "platformOSX/platformOSX.h"
#import "platformOSX/osxCocoaUtilities.h"
#include "platform/nativeDialogs/fileDialog.h"
#include "io/resource/resourceManager.h"

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
            NSURL* fileURL =
                    [nsFileArray objectAtIndex:i];

            const UTF8* file = [[fileURL path] UTF8String];
            setDataField(StringTable->insert("files"), Con::getIntArg(i), StringTable->insert(file));
        }

        setDataField(StringTable->insert("fileCount"), NULL, Con::getIntArg([nsFileArray count]));
    }
    else
    {
        NSURL* fileURL = [nsFileArray objectAtIndex:0];

        const UTF8* file = [[fileURL path] UTF8String];

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

struct _NSStringMap
{
    S32 num;
    NSString* ok;
    NSString* cancel;
    NSString* third;
};

static _NSStringMap sgButtonTextMap[] =
{
        { MBOk,                 @"Ok",    nil,        nil },
        { MBOkCancel,           @"Ok",    @"Cancel",  nil },
        { MBRetryCancel,        @"Retry", @"Cancel",  nil },
        { MBSaveDontSave,       @"Yes",  @"No", nil },
        { MBSaveDontSaveCancel, @"Save",  @"Cancel",  @"Don't Save" },
        { -1, nil, nil, nil }
};

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertOK not yet implemented")
void Platform::AlertOK(const char *windowTitle, const char *message)
{
    Platform::messageBox(windowTitle, message, MBOk, MIWarning);

    /*NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText:[NSString stringWithUTF8String:windowTitle]];
    [alert setInformativeText:[NSString stringWithUTF8String:message]];

    [alert runModal];*/
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertOKCancel not yet implemented")
bool Platform::AlertOKCancel(const char *windowTitle, const char *message)
{
    return Platform::messageBox(windowTitle, message, MBOkCancel, MIWarning);

    /*
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert addButtonWithTitle:@"OK"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:[NSString stringWithUTF8String:windowTitle]];
    [alert setInformativeText:[NSString stringWithUTF8String:message]];

    S32 result = [alert runModal];
    */

    //return (result == NSAlertFirstButtonReturn);
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertRetry not yet implemented")
bool Platform::AlertRetry(const char *windowTitle, const char *message)
{
    return Platform::messageBox(windowTitle, message, MBRetryCancel, MIWarning);

    /*
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert addButtonWithTitle:@"Retry"];
    [alert addButtonWithTitle:@"Cancel"];
    [alert setMessageText:[NSString stringWithUTF8String:windowTitle]];
    [alert setInformativeText:[NSString stringWithUTF8String:message]];

    S32 result = [alert runModal];

    return (result == NSAlertFirstButtonReturn);
    */
}

//-----------------------------------------------------------------------------
#pragma message ("Platform::AlertYesNo not yet implemented")
bool Platform::AlertYesNo(const char *windowTitle, const char *message)
{
    return Platform::messageBox(windowTitle, message, MBSaveDontSave, MIWarning);

    /*
    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert addButtonWithTitle:@"Yes"];
    [alert addButtonWithTitle:@"No"];
    [alert setMessageText:[NSString stringWithUTF8String:windowTitle]];
    [alert setInformativeText:[NSString stringWithUTF8String:message]];

    S32 result = [alert runModal];

    return (result == NSAlertFirstButtonReturn);
    */
}


//-----------------------------------------------------------------------------
#pragma message ("Platform::messageBox not yet implemented")
S32 Platform::messageBox(const UTF8 *title, const UTF8 *message, MBButtons buttons, MBIcons icon)
{
    NSString *okBtn      = nil;
    NSString *cancelBtn  = nil;
    NSString *thirdBtn   = nil;

    U32 i;

    for (i = 0; sgButtonTextMap[i].num != -1; i++)
    {
        if (sgButtonTextMap[i].num != buttons)
            continue;

        okBtn = sgButtonTextMap[i].ok;
        cancelBtn = sgButtonTextMap[i].cancel;
        thirdBtn = sgButtonTextMap[i].third;

        break;
    }

    if(sgButtonTextMap[i].num == -1)
        Con::errorf("Unknown message box button set requested. Mac Platform::messageBox() probably needs to be updated.");

    // convert title and message to NSStrings
    NSString *nsTitle = [NSString stringWithUTF8String:title];
    NSString *nsMessage = [NSString stringWithUTF8String:message];

    Input::setCursorShape(CursorManager::curIBeam);

    NSAlert *alert = [[[NSAlert alloc] init] autorelease];
    [alert setAlertStyle:NSWarningAlertStyle];
    [alert addButtonWithTitle:okBtn];
    [alert addButtonWithTitle:cancelBtn];
    [alert addButtonWithTitle:thirdBtn];
    [alert setMessageText:nsTitle];
    [alert setInformativeText:nsMessage];

    S32 result = [alert runModal];

    return (result == NSAlertFirstButtonReturn);
}
