//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
#include "platformOSX/platformOSX.h"
#include "platformOSX/osxCocoaUtilities.h"
#include "gui/guiCanvas.h"

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

    [[mData->mController menu] insertItem:newItem atIndex:pos];

    [newItem release];

    return 0;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::insertSubMenu not yet implemented")
S32 PopupMenu::insertSubMenu(S32 pos, const char *title, PopupMenu *submenu)
{
    for(S32 i = 0;i < mSubmenus->size();i++)
    {
        if(submenu == (*mSubmenus)[i])
        {
            Con::errorf("PopupMenu::insertSubMenu - Attempting to add submenu twice");
            return -1;
        }
    }

    NSMenuItem *newItem;

    newItem = [[NSMenuItem allocWithZone:[NSMenu menuZone]]
            initWithTitle:[NSString stringWithUTF8String:title] action:NULL
            keyEquivalent:[NSString stringWithUTF8String:""]];

    [newItem setSubmenu:[submenu->mData->mController menu]];
    [newItem setTarget:submenu->mData->mController];
    [newItem setAction:@selector(handleSelect:)];

    [[mData->mController menu] insertItem:newItem atIndex:pos];

    [newItem release];

    mSubmenus->addObject(submenu);

    return 0;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::removeItem not yet implemented")
void PopupMenu::removeItem(S32 itemPos)
{
    [[mData->mController menu] removeItemAtIndex:itemPos];
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::enableItem not yet implemented")
void PopupMenu::enableItem(S32 pos, bool enable)
{
    [[[mData->mController menu] itemAtIndex:pos] setEnabled:enable];
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::checkItem not yet implemented")
void PopupMenu::checkItem(S32 pos, bool checked)
{
    [[[mData->mController menu] itemAtIndex:pos] setState:(checked ? NSOnState : NSOffState)];
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::checkRadioItem not yet implemented")
void PopupMenu::checkRadioItem(S32 firstPos, S32 lastPos, S32 checkPos)
{
    for(int i = firstPos; i <= lastPos; i++)
        checkItem( i, false);

    // check the selected item
    checkItem( checkPos, true);
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::isItemChecked not yet implemented")
bool PopupMenu::isItemChecked(S32 pos)
{
    S32 state = [[[mData->mController menu] itemAtIndex:pos] state];

    return (state == NSOnState);
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::canHandleID not yet implemented")
bool PopupMenu::canHandleID(U32 iD)
{
//    for(S32 i = 0;i < mSubmenus->size();i++)
//    {
//        PopupMenu *subM = dynamic_cast<PopupMenu *>((*mSubmenus)[i]);
//        if(subM == NULL)
//            continue;
//
//        if(subM->canHandleID(iD))
//            return true;
//    }

//    S32 index;
//    U32 itemCount = [[mData->mController menu] numberOfItems];

//    for (int i = 0; i <= itemCount; i++)
//    {

//    }
    //U32 nItems = CountMenuItems(mData->mMenu);
//    for(int i = 1; i <= nItems; i++)
//    {
//        GetMenuItemRefCon(mData->mMenu, i, &refcon);
//        if(refcon == iD)
//            return true;
//    }

    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::handleSelect not yet implemented")
bool PopupMenu::handleSelect(U32 command, const char *text /* = NULL */)
{
    return dAtob(Con::executef(this, 4, "onSelectItem", command, text ? text : ""));
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