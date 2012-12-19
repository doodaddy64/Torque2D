//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
#include "platformOSX/platformOSX.h"
#include "platform/menus/popupMenu.h"
#include "memory/safeDelete.h"
#include "gui/guiCanvas.h"

@interface osxPopupMenuController : NSObject
{
    NSMenu* _menu;
    NSMenuItem* _menuItem;
    PopupMenu* _owner;
}

@property NSMenu* menu;
@property NSMenuItem* menuItem;
@property PopupMenu* owner;

-(void)handleSelect:(id)sender;

@end

@implementation osxPopupMenuController

@synthesize menu = _menu;
@synthesize menuItem = _menuItem;
@synthesize owner = _owner;

-(id) init
{
    self = [super init];

    if (self)
    {
        _owner = NULL;
        _menu = [[NSMenu allocWithZone:[NSMenu menuZone]] initWithTitle:@"New Menu"];
        _menuItem = [[NSMenuItem allocWithZone:[NSMenu menuZone]] initWithTitle:@"New Menu Item" action:NULL keyEquivalent:@""];
        [_menuItem setSubmenu:_menu];
    }
}

-(void) dealloc
{
    if (_menu)
        [_menu release];

    if (_menuItem)
        [_menuItem release];

    [super dealloc];
}

-(void)handleSelect:(id)sender
{
    if(_owner)
        _owner->handleSelect(3, [[_menu title] UTF8String]);
}

@end

class PlatformPopupMenuData
{
public:

    osxPopupMenuController* mController;
    S32 tag;

    PlatformPopupMenuData()
    {
        mController = NULL;

        tag = getTag();
    }

    ~PlatformPopupMenuData()
    {
        if (mController)
            [mController release];

        mController = NULL;
    }

    // We assign each new menu item an arbitrary integer tag.
    static S32 getTag()
    {
        static S32 lastTag = 'TORQ';
        return ++lastTag;
    }
};

//-----------------------------------------------------------------------------
// Allocates the OS X specific PopupMenuData
 void PopupMenu::createPlatformPopupMenuData()
{
    mData = new PlatformPopupMenuData;
}

//-----------------------------------------------------------------------------
// Deallocates the OS X specific PopupMenuData
void PopupMenu::deletePlatformPopupMenuData()
{
    SAFE_DELETE(mData);
}

//-----------------------------------------------------------------------------
// Called in PopupMenu's constructor, this handles platform specific menu setup
void PopupMenu::createPlatformMenu()
{
    mData->mController = [[osxPopupMenuController alloc] init];
    [mData->mController setOwner:this];
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::insertItem not yet implemented")
S32 PopupMenu::insertItem(S32 pos, const char *title, const char *accel)
{
    NSMenuItem *newItem;

    newItem = [[NSMenuItem allocWithZone:[NSMenu menuZone]]
            initWithTitle:[NSString stringWithUTF8String:title]
            action:NULL
            keyEquivalent:[NSString stringWithUTF8String:accel]];

    [newItem setTarget:mData->mController];
    [newItem setAction:@selector(handleSelect:)];

    [[mData->mController menu] addItem:newItem];

    [newItem release];

    return 0;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::insertSubMenu not yet implemented")
S32 PopupMenu::insertSubMenu(S32 pos, const char *title, PopupMenu *submenu)
{
    return 0;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::removeItem not yet implemented")
void PopupMenu::removeItem(S32 itemPos)
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::enableItem not yet implemented")
void PopupMenu::enableItem(S32 pos, bool enable)
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::checkItem not yet implemented")
void PopupMenu::checkItem(S32 pos, bool checked)
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::checkRadioItem not yet implemented")
void PopupMenu::checkRadioItem(S32 firstPos, S32 lastPos, S32 checkPos)
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::isItemChecked not yet implemented")
bool PopupMenu::isItemChecked(S32 pos)
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::canHandleID not yet implemented")
bool PopupMenu::canHandleID(U32 iD)
{
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::handleSelect not yet implemented")
bool PopupMenu::handleSelect(U32 command, const char *text /* = NULL */)
{
    //return dAtob(Con::executef(this, 4, "onSelectItem", Con::getIntArg(pos - 1), text ? text : ""));
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::showPopup not yet implemented")
void PopupMenu::showPopup(S32 x /* = -1 */, S32 y /* = -1 */)
{
    // Get the position of the cursor
    if(x < 0 || y < 0)
    {
        Point2I p = Canvas->getCursorPos();
        x = p.x;
        y = p.y;
    }

    // Convert to native coordinates
    //CGPoint native = MacCarbTorqueToNativeCoords(x, y);

    // Manually click the menu item
    //U32 result = PopUpMenuSelect(mData->mMenu, native.y, native.x, 0);
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::attachToMenuBar not yet implemented")
void PopupMenu::attachToMenuBar(S32 pos, const char *title)
{
    [[mData->mController menuItem] setTitle:[NSString stringWithUTF8String:title]];
    [[mData->mController menu] setTitle:[NSString stringWithUTF8String:title]];
    [[NSApp mainMenu] addItem:[mData->mController menuItem]];
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::removeFromMenuBar not yet implemented")
void PopupMenu::removeFromMenuBar()
{
    [[NSApp mainMenu] removeItem:[mData->mController menuItem]];
}