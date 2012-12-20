//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------
#include "platform/nativeDialogs/fileDialog.h"
#include "memory/safeDelete.h"
#include "platform/menus/popupMenu.h"

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

@interface NSSavePanel (TorqueFileDialogs)
- (void)setFilters:(StringTableEntry)filters;
- (void)setAttributesFromData:(FileDialogData *)data;
- (FileDialogFileExtList *)getFileExtensionsFromString:(const char*) filter;
+ (NSArray *)showSavePanel:(FileDialogData *)withData;
@end

@interface NSOpenPanel (TorqueFileDialogs)
- (void)setAttributesFromData:(FileDialogData *)data;
+ (NSArray *)showOpenPanel:(FileDialogData *)withData;
@end

@interface osxPopupMenuController : NSObject
{
    NSMenu* _menu;
    NSMenuItem* _menuItem;
    PopupMenu* _owner;
}

@property (strong) NSMenu* menu;
@property (strong) NSMenuItem* menuItem;
@property (assign) PopupMenu* owner;

-(void)handleSelect:(id)sender;

@end