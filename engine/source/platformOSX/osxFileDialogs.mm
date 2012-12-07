//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platformOSX/platformOSX.h"
#include "sim/simBase.h"
#include "platform/nativeDialogs/fileDialog.h"

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(FileDialog);
IMPLEMENT_CONOBJECT(OpenFileDialog);
IMPLEMENT_CONOBJECT(SaveFileDialog);
IMPLEMENT_CONOBJECT(OpenFolderDialog);

//-----------------------------------------------------------------------------
#pragma message ("FileDialogData constructor not yet implemented")
FileDialogData::FileDialogData()
{
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
    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::Execute not yet implemented")
bool FileDialog::Execute()
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::setFilters not yet implemented")
bool FileDialog::setFilters(void* obj, const char* data)
{
    return false;
};

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::setDefaultPath not yet implemented")
bool FileDialog::setDefaultPath(void* obj, const char* data)
{
    return false;
};

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::setDefaultFile not yet implemented")
bool FileDialog::setDefaultFile(void* obj, const char* data)
{
    return false;
};

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::setChangePath not yet implemented")
bool FileDialog::setChangePath(void* obj, const char* data)
{
    return false;
};

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::getChangePath not yet implemented")
const char* FileDialog::getChangePath(void* obj, const char* data)
{
    return "";
}

//-----------------------------------------------------------------------------
#pragma message ("FileDialog::setFile not yet implemented")
bool FileDialog::setFile(void* obj, const char* data)
{
   return false;
};

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog constructor not yet implemented")
OpenFileDialog::OpenFileDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog destructor not yet implemented")
OpenFileDialog::~OpenFileDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog::initPersistFields not yet implemented")
void OpenFileDialog::initPersistFields()
{
    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog::setMustExist not yet implemented")
bool OpenFileDialog::setMustExist(void* obj, const char* data)
{
    return false;
};

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog::getMustExist not yet implemented")
const char* OpenFileDialog::getMustExist(void* obj, const char* data)
{
    return "";
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog::setMultipleFiles not yet implemented")
bool OpenFileDialog::setMultipleFiles(void* obj, const char* data)
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFileDialog::getMultipleFiles not yet implemented")
const char* OpenFileDialog::getMultipleFiles(void* obj, const char* data)
{
   return "";
}

//-----------------------------------------------------------------------------
#pragma message ("SaveFileDialog constructor not yet implemented")
SaveFileDialog::SaveFileDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("SaveFileDialog destructor not yet implemented")
SaveFileDialog::~SaveFileDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("SaveFileDialog::initPersistFields not yet implemented")
void SaveFileDialog::initPersistFields()
{
    Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
#pragma message ("SaveFileDialog::setOverwritePrompt not yet implemented")
bool SaveFileDialog::setOverwritePrompt(void* obj, const char* data)
{
    return false;
};

//-----------------------------------------------------------------------------
#pragma message ("SaveFileDialog::getOverwritePrompt not yet implemented")
const char* SaveFileDialog::getOverwritePrompt(void* obj, const char* data)
{
    return "";
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFolderDialog constructor not yet implemented")
OpenFolderDialog::OpenFolderDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFolderDialog destructor not yet implemented")
OpenFolderDialog::~OpenFolderDialog()
{
}

//-----------------------------------------------------------------------------
#pragma message ("OpenFolderDialog::initPersistFields not yet implemented")
void OpenFolderDialog::initPersistFields()
{
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
