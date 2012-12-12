//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platformMacCarb/platformMacCarb.h"
#include "sim/simBase.h"
#include "platform/nativeDialogs/fileDialog.h"
#include "platform/threads/mutex.h"
#include "memory/safeDelete.h"
#include "math/mMath.h"
#include "string/Unicode.h"
#include "console/consoleTypes.h"
#include "platform/threads/thread.h"
#include "platformMacCarb/cocoaUtils.h"
#include <Cocoa/Cocoa.h>

class FileDialogFileExtList
{
public:
   Vector<UTF8*> list;
   UTF8* data;
   
   FileDialogFileExtList(const char* exts) { data = dStrdup(exts); }
   ~FileDialogFileExtList() { SAFE_DELETE(data); }
};

class FileDialogFileTypeList
{
public:
   UTF8* filterData;
   Vector<UTF8*> names;
   Vector<FileDialogFileExtList*> exts;
   bool any;
   
   FileDialogFileTypeList(const char* filter) { filterData = dStrdup(filter); any = false;}
   ~FileDialogFileTypeList()
   { 
      SAFE_DELETE(filterData);
      for(U32 i = 0; i < exts.size(); i++)
         delete exts[i];
   }
};


static FileDialogFileExtList* _MacCarbGetFileExtensionsFromString(const char* filter)
{
   FileDialogFileExtList* list = new FileDialogFileExtList(filter);
   
   char* token = list->data;
   char* place = list->data;
   
   for( ; *place; place++)
   {
      if(*place != ';')
         continue;
      
      *place = '\0';
      
      list->list.push_back(token);
      
      ++place;
      token = place;
   }
   // last token   
   list->list.push_back(token);
   
   return list;
   
}

static FileDialogFileTypeList* _MacCarbGetFileTypesFromString(const char* filter)
{
   FileDialogFileTypeList &list = *(new FileDialogFileTypeList(filter));

   char* token = list.filterData;
   char* place = list.filterData;
   
   // scan the filter list until we hit a null.
   // when we see the separator '|', replace it with a null, and save the token
   // format is description|extension|description|extension
   bool isDesc = true;
   for( ; *place; place++)
   {
      if(*place != '|')
         continue;
      
      *place = '\0';

      if(isDesc)
         list.names.push_back(token);
      else
      {
         // detect *.*
         if(dStrstr((const char*)token, "*.*"))
            list.any = true;
         
         list.exts.push_back(_MacCarbGetFileExtensionsFromString(token));
      }
   
      
      isDesc = !isDesc;
      ++place;
      token = place;
   }
   list.exts.push_back(_MacCarbGetFileExtensionsFromString(token));
   
   return &list;
}

static NSArray* _MacCocoaCreateAndRunSavePanel(FileDialogData &mData)
{
   NSSavePanel* panel = [NSSavePanel savePanel];

   // User freedom niceties
   [panel setCanCreateDirectories:YES];
   [panel setCanSelectHiddenExtension:YES];
   [panel setTreatsFilePackagesAsDirectories:YES];

   NSString *initialFile = [[NSString stringWithUTF8String:mData.mDefaultFile] lastPathComponent];

   // we only use mDefaultDir if mDefault path is not set.
   NSString *dir;
   if(dStrlen(mData.mDefaultPath) < 1)
      dir = [[NSString stringWithUTF8String:mData.mDefaultFile] stringByDeletingLastPathComponent];
   else
      dir = [NSString stringWithUTF8String: mData.mDefaultPath];
   [panel setDirectory:dir];
   
   // todo: move file type handling to an accessory view.
   // parse file types
   FileDialogFileTypeList *fTypes = _MacCarbGetFileTypesFromString(mData.mFilters);
   
   // fill an array with the possible file types
   NSMutableArray* types = [NSMutableArray arrayWithCapacity:10];
   for(U32 i = 0; i < fTypes->exts.size(); i++)
   {
      for(U32 j = 0; j < fTypes->exts[i]->list.size(); j++)
      {
         char* ext = fTypes->exts[i]->list[j];
         if(ext)
         {
            if(dStrncmp(ext, "*.", 2) == 0)
               ext+=2;
               
            [types addObject:[NSString stringWithUTF8String:ext]];
         }
      }
   }
   if([types count] > 0)
      [panel setAllowedFileTypes:types];

    // if any file type was *.*, user may select any file type.
   if(fTypes->any)
	   [panel setAllowsOtherFileTypes:YES];

   
   //---------------------------------------------------------------------------
   // Display the panel, enter a modal loop. This blocks.
   //---------------------------------------------------------------------------   
   U32 button = [panel runModalForDirectory:dir file:initialFile];
  
   // return the file name
   NSMutableArray *array = [NSMutableArray arrayWithCapacity:10];
   if(button != NSFileHandlingPanelCancelButton)
      [array addObject:[panel filename]];
   
   return array;
   
   // TODO: paxorr: show as sheet
   // crashes when we try to display the window as a sheet. Not sure why.
   // the sheet is instantly dismissed, and crashes as it's dismissing itself.
   // here's the code snippet to get an nswindow from our carbon WindowRef
   //NSWindow *nsAppWindow = [[NSWindow alloc] initWithWindowRef:platState.appWindow];
}

NSArray* _MacCocoaCreateAndRunOpenPanel(FileDialogData &mData)
{
   NSOpenPanel* panel = [NSOpenPanel openPanel];

   // User freedom niceties
   [panel setCanCreateDirectories:YES];
   [panel setCanSelectHiddenExtension:YES];
   [panel setTreatsFilePackagesAsDirectories:YES];

   [panel setAllowsMultipleSelection:(mData.mStyle & FileDialogData::FDS_MULTIPLEFILES)];

   // 
   bool chooseDir = (mData.mStyle & FileDialogData::FDS_BROWSEFOLDER);
   [panel setCanChooseFiles: !chooseDir ];
   [panel setCanChooseDirectories: chooseDir ];
   if(chooseDir)
   {
      [panel setPrompt:@"Choose"];
      [panel setTitle:@"Choose Folder"];
   }

   NSString *initialFile = [[NSString stringWithUTF8String:mData.mDefaultFile] lastPathComponent];

   // we only use mDefaultDir if mDefault path is not set.
   NSString *dir;
   if(dStrlen(mData.mDefaultPath) < 1)
      dir = [[NSString stringWithUTF8String:mData.mDefaultFile] stringByDeletingLastPathComponent];
   else
      dir = [NSString stringWithUTF8String: mData.mDefaultPath];
   [panel setDirectory:dir];
   
   // todo: move file type handling to an accessory view.
   // parse file types
   FileDialogFileTypeList *fTypes = _MacCarbGetFileTypesFromString(mData.mFilters);
   
   // fill an array with the possible file types
   NSMutableArray* types = [NSMutableArray arrayWithCapacity:10];
   for(U32 i = 0; i < fTypes->exts.size(); i++)
   {
      for(U32 j = 0; j < fTypes->exts[i]->list.size(); j++)
      {
         char* ext = fTypes->exts[i]->list[j];
         if(ext)
         {
			 // MP: Added for 1.4.1
			 // Passing *.*, *., .*, or just . does nothing on OS X 10.6
			 // Manually adding the extensions supported by the engine for now
			if(dStrncmp(ext, "*.*", 3) == 0)
			{
				[types addObject:[NSString stringWithUTF8String:"png"]];
				[types addObject:[NSString stringWithUTF8String:"jpg"]];
				[types addObject:[NSString stringWithUTF8String:"eff"]];
				[types addObject:[NSString stringWithUTF8String:"lyr"]];
				[types addObject:[NSString stringWithUTF8String:"gui"]];
				[types addObject:[NSString stringWithUTF8String:"bmp"]];
				continue;
			}
				
            if(dStrncmp(ext, "*.", 2) == 0)
               ext+=2;
               
            [types addObject:[NSString stringWithUTF8String:ext]];
         }
      }
   }
	
	if([types count] > 0)
		[panel setAllowedFileTypes:types];
	
	// if any file type was *.*, user may select any file type.
	if(fTypes->any)
		[panel setAllowsOtherFileTypes:YES];
	

   //---------------------------------------------------------------------------
   // Display the panel, enter a modal loop. This blocks.
   //---------------------------------------------------------------------------   
   U32 button = [panel runModalForDirectory:dir file:initialFile types:types];
  
   // return the file name
   NSMutableArray *array = [NSMutableArray arrayWithCapacity:10];
   if(button != NSFileHandlingPanelCancelButton)
      [array addObject:[panel filename]];
   
   return array;
}

void MacCarbShowDialog(void* dialog)
{
   FileDialog* d = static_cast<FileDialog*>(dialog);
   d->Execute();
}
//
// Execute Method
//
bool FileDialog::Execute()
{
//   if(! ThreadManager::isCurrentThread(platState.firstThreadId))
//   {
//      MacCarbSendTorqueEventToMain(kEventTorqueModalDialog,this);
//      mData.mOpaqueData->sem->acquire();
//      return;
//   }
   
   NSArray* nsFileArray;
   if(mData.mStyle & FileDialogData::FDS_OPEN)
      nsFileArray = _MacCocoaCreateAndRunOpenPanel(mData);
   else if(mData.mStyle & FileDialogData::FDS_SAVE)
      nsFileArray = _MacCocoaCreateAndRunSavePanel(mData);
   else
   {
      Con::errorf("Bad File Dialog Setup.");
      return false;
   }
   
   if([nsFileArray count] == 0)
      return false;
   
   if(! (mData.mStyle & FileDialogData::FDS_MULTIPLEFILES) && [nsFileArray count] >= 1)
   {
      const UTF8* f = [(NSString*)[nsFileArray objectAtIndex:0] UTF8String];
      mData.mFile = StringTable->insert(f);
   }
   else
   {
      for(U32 i = 0; i < [nsFileArray count]; i++)
      {
         const UTF8* f = [(NSString*)[nsFileArray objectAtIndex:i] UTF8String];
         setDataField(StringTable->insert("files"), Con::getIntArg(i), StringTable->insert(f));
      }
      setDataField(StringTable->insert("fileCount"), NULL, Con::getIntArg([nsFileArray count]));
   }
   
   return true;

}

//-----------------------------------------------------------------------------
// Default Path Property - String Validated on Write
//-----------------------------------------------------------------------------
bool FileDialog::setDefaultPath(void* obj, const char* data)
{

   if( !data )
      return true;

   return true;

}

//-----------------------------------------------------------------------------
// Default File Property - String Validated on Write
//-----------------------------------------------------------------------------
bool FileDialog::setDefaultFile(void* obj, const char* data)
{
   if( !data )
      return true;

   return true;
}