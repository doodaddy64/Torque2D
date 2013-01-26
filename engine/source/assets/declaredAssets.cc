//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _DECLARED_ASSETS_H_
#include "assets/declaredAssets.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT( DeclaredAssets );

//-----------------------------------------------------------------------------

void DeclaredAssets::initPersistFields()
{
    // Call Parent.
    Parent::initPersistFields();
        
    addField("Path", TypeString, Offset(mPath, DeclaredAssets), "" );
    addField("Extension", TypeString, Offset(mExtension, DeclaredAssets), "" );
    addField("Recurse", TypeBool, Offset(mRecurse, DeclaredAssets), "" );
}
