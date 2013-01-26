//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _ASSET_FIELD_TYPES_H_
#define _ASSET_FIELD_TYPES_H_

#ifndef _CONSOLE_BASE_TYPE_H_
#include "console/consoleBaseType.h"
#endif

//-----------------------------------------------------------------------------

DefineConsoleType( TypeAssetId )
DefineConsoleType( TypeAssetLooseFilePath )

//-----------------------------------------------------------------------------

#define ASSET_ASSIGNMENT_SEPARATOR      "="
#define ASSET_SCOPE_SEPARATOR           ":"

/// Asset Id.
#define ASSET_ID_SIGNATURE              "@asset"
#define ASSET_ID_FIELD_PREFIX           "@asset="

/// Asset loose file.
#define ASSET_LOOSEFILE_SIGNATURE       "@assetFile"
#define ASSET_LOOSE_FILE_FIELD_PREFIX   "@assetFile="

#endif // _ASSET_FIELD_TYPES_H_