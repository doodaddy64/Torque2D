//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#include "platform/nativeDialogs/fileDialog.h"
#include "console/consoleTypes.h"
#include "fileDialog_ScriptBinding.h"

IMPLEMENT_CONOBJECT(FileDialog);
IMPLEMENT_CONOBJECT(OpenFileDialog);
IMPLEMENT_CONOBJECT(SaveFileDialog);
IMPLEMENT_CONOBJECT(OpenFolderDialog);

//-----------------------------------------------------------------------------
FileDialogData::FileDialogData()
{
	mDefaultPath = StringTable->insert(Con::getVariable("Tools::FileDialogs::LastFilePath"));

	if (mDefaultPath == StringTable->EmptyString || !Platform::isDirectory(mDefaultPath))
		mDefaultPath = Platform::getCurrentDirectory();

	mDefaultPath = StringTable->insert("");
	mFilters = StringTable->insert("");
	mFile = StringTable->insert("");
	mTitle = StringTable->insert("");

	mStyle = 0;
}

//-----------------------------------------------------------------------------
FileDialogData::~FileDialogData()
{

}

//-----------------------------------------------------------------------------
FileDialog::FileDialog() : mData()
{
	// Default to File Must Exist Open Dialog style
	mData.mStyle = FileDialogData::FDS_OPEN | FileDialogData::FDS_MUSTEXIST;
	mChangePath = false;
}

//-----------------------------------------------------------------------------
FileDialog::~FileDialog()
{
}

//-----------------------------------------------------------------------------
void FileDialog::initPersistFields()
{
	addProtectedField("DefaultPath", TypeString, Offset(mData.mDefaultPath, FileDialog), &setDefaultPath, &defaultProtectedGetFn, "Default Path when Dialog is shown");
	addProtectedField("DefaultFile", TypeString, Offset(mData.mDefaultFile, FileDialog), &setDefaultFile, &defaultProtectedGetFn, "Default File when Dialog is shown");
	addProtectedField("FileName", TypeString, Offset(mData.mFile, FileDialog), &setFile, &defaultProtectedGetFn, "Default File when Dialog is shown");
	addProtectedField("Filters", TypeString, Offset(mData.mFilters, FileDialog), &setFilters, &defaultProtectedGetFn, "Default File when Dialog is shown");
	addField("Title", TypeString, Offset(mData.mTitle, FileDialog), "Default File when Dialog is shown");
	addProtectedField("ChangePath", TypeBool, Offset(mChangePath, FileDialog), &setChangePath, &getChangePath, "True/False whether to set the working directory to the directory returned by the dialog");

	Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
// Dialog Filters
bool FileDialog::setFilters(void* obj, const char* data)
{
	// Will do validate on write at some point.
	if (!data)
		return true;

	return true;
}

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
}

//-----------------------------------------------------------------------------
// ChangePath Property - Get the path change
const char* FileDialog::getChangePath(void* obj, const char* data)
{
	FileDialog *pDlg = static_cast<FileDialog*>(obj);
   
	if (pDlg->mData.mStyle & FileDialogData::FDS_CHANGEPATH)
		return StringTable->insert("true");
	else
		return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
// Dead function. Let's clean this up in the next platform sweep
bool FileDialog::setFile(void* obj, const char* data)
{
   return false;
}

//-----------------------------------------------------------------------------
// OpenFileDialog Constructor
OpenFileDialog::OpenFileDialog()
{
	// Default File Must Exist
	mData.mStyle = FileDialogData::FDS_OPEN | FileDialogData::FDS_MUSTEXIST;
}

//-----------------------------------------------------------------------------
// OpenFileDialog Destructor
OpenFileDialog::~OpenFileDialog()
{
	mMustExist = true;
	mMultipleFiles = false;
}

//-----------------------------------------------------------------------------
// Console Properties
void OpenFileDialog::initPersistFields()
{
	addProtectedField("MustExist", TypeBool, Offset(mMustExist, OpenFileDialog), &setMustExist, &getMustExist, "True/False whether the file returned must exist or not");
	addProtectedField("MultipleFiles", TypeBool, Offset(mMultipleFiles, OpenFileDialog), &setMultipleFiles, &getMultipleFiles, "True/False whether multiple files may be selected and returned or not");
   
	Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
// Set the mData.mStyle to use the FDS_MUSTEXIST flag
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
// Check if the mData.mStyle has the FDS_MUSTEXIST flag
const char* OpenFileDialog::getMustExist(void* obj, const char* data)
{
	OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);
   
	if (pDlg->mData.mStyle & FileDialogData::FDS_MUSTEXIST)
		return StringTable->insert("true");
	else
		return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
// Set the the mData.mStyle property to use the FDS_MULTIPLEFILES flag
bool OpenFileDialog::setMultipleFiles(void* obj, const char* data)
{
	bool bMustExist = dAtob(data);

	OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);

	if (bMustExist)
		pDlg->mData.mStyle |= FileDialogData::FDS_MULTIPLEFILES;
	else
		pDlg->mData.mStyle &= ~FileDialogData::FDS_MULTIPLEFILES;

	return true;
};

//-----------------------------------------------------------------------------
// Check if the mData.mStyle has the FDS_MULTIPLEFILES flag
const char* OpenFileDialog::getMultipleFiles(void* obj, const char* data)
{
	OpenFileDialog *pDlg = static_cast<OpenFileDialog*>(obj);
   
	if (pDlg->mData.mStyle & FileDialogData::FDS_MULTIPLEFILES)
		return StringTable->insert("true");
	else
		return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
// SaveFileDialog Constructor
SaveFileDialog::SaveFileDialog()
{
	// Default File Must Exist
	mData.mStyle = FileDialogData::FDS_SAVE | FileDialogData::FDS_OVERWRITEPROMPT;
	mOverwritePrompt = true;
}

//-----------------------------------------------------------------------------
// SaveFileDialog Destructor
SaveFileDialog::~SaveFileDialog()
{
}

//-----------------------------------------------------------------------------
// Console Properties
void SaveFileDialog::initPersistFields()
{
	addProtectedField("OverwritePrompt", TypeBool, Offset(mOverwritePrompt, SaveFileDialog), &setOverwritePrompt, &getOverwritePrompt, "True/False whether the dialog should prompt before accepting an existing file name");
   
	Parent::initPersistFields();
}

//-----------------------------------------------------------------------------
// Set the mData.mStyle property to use the FDS_OVERWRITEPROMPT flag
bool SaveFileDialog::setOverwritePrompt(void* obj, const char* data)
{
	bool bMustExist = dAtob(data);

	SaveFileDialog *pDlg = static_cast<SaveFileDialog*>(obj);

	if (bMustExist)
		pDlg->mData.mStyle |= FileDialogData::FDS_OVERWRITEPROMPT;
	else
		pDlg->mData.mStyle &= ~FileDialogData::FDS_OVERWRITEPROMPT;

	return true;
}

//-----------------------------------------------------------------------------
// Check if the mData.mStyle property uses the FDS_OVERWRITEPROMPT flag
const char* SaveFileDialog::getOverwritePrompt(void* obj, const char* data)
{
	SaveFileDialog *pDlg = static_cast<SaveFileDialog*>(obj);

	if (pDlg->mData.mStyle & FileDialogData::FDS_OVERWRITEPROMPT)
		return StringTable->insert("true");
	else
		return StringTable->insert("false");
}

//-----------------------------------------------------------------------------
// OpenFolderDialog Constructor
OpenFolderDialog::OpenFolderDialog()
{
	mData.mStyle = FileDialogData::FDS_OPEN | FileDialogData::FDS_OVERWRITEPROMPT | FileDialogData::FDS_BROWSEFOLDER;
	mMustExistInDir = "";
}

//-----------------------------------------------------------------------------
// OpenFolderDialog Destructor
OpenFolderDialog::~OpenFolderDialog()
{
}

//-----------------------------------------------------------------------------
// OpenFolderDialog Console Properties
void OpenFolderDialog::initPersistFields()
{
	addField("fileMustExist", TypeFilename, Offset(mMustExistInDir, OpenFolderDialog), "File that must in selected folder for it to be valid");

	Parent::initPersistFields();
}
