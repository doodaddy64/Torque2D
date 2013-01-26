//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _REFERENCED_ASSETS_H_
#include "assets/referencedAssets.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT( ReferencedAssets );

//-----------------------------------------------------------------------------

void ReferencedAssets::initPersistFields()
{
    // Call Parent.
    Parent::initPersistFields();
        
    addField("Path", TypeString, Offset(mPath, ReferencedAssets), "" );
    addField("Extension", TypeString, Offset(mExtension, ReferencedAssets), "" );
    addField("Recurse", TypeBool, Offset(mRecurse, ReferencedAssets), "" );
}