//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------
#include "platformOSX/platformOSX.h"
#include "platform/menus/popupMenu.h"

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::createPlatformPopupMenuData not yet implemented")
void PopupMenu::createPlatformPopupMenuData()
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::deletePlatformPopupMenuData not yet implemented")
void PopupMenu::deletePlatformPopupMenuData()
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::createPlatformMenu not yet implemented")
void PopupMenu::createPlatformMenu()
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::insertItem not yet implemented")
S32 PopupMenu::insertItem(S32 pos, const char *title, const char* accel)
{
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
    return false;
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::showPopup not yet implemented")
void PopupMenu::showPopup(S32 x /* = -1 */, S32 y /* = -1 */)
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::attachToMenuBar not yet implemented")
void PopupMenu::attachToMenuBar(S32 pos, const char *title)
{
}

//-----------------------------------------------------------------------------
#pragma message ("PopupMenu::removeFromMenuBar not yet implemented")
void PopupMenu::removeFromMenuBar()
{
}