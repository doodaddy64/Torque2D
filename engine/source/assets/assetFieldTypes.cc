//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _ASSET_FIELD_TYPES_H_
#include "assetFieldTypes.h"
#endif

#ifndef _ASSET_PTR_H_
#include "assetPtr.h"
#endif

#ifndef _ASSET_BASE_H_
#include "assetBase.h"
#endif

#ifndef _IMAGE_ASSET_H_
#include "2d/assets/ImageAsset.h"
#endif

#ifndef _ANIMATION_ASSET_H_
#include "2d/assets/AnimationAsset.h"
#endif

#ifndef _AUDIO_ASSET_H_
#include "audio/audioAsset.h"
#endif

#ifndef _STRINGUNIT_H_
#include "string/stringUnit.h"
#endif

//-----------------------------------------------------------------------------

ConsoleType( assetLooseFilePath, TypeAssetLooseFilePath, sizeof(StringTableEntry), ASSET_LOOSE_FILE_FIELD_PREFIX )
ConsoleType( assetIdString, TypeAssetId, sizeof(StringTableEntry), ASSET_ID_FIELD_PREFIX )

//-----------------------------------------------------------------------------

ConsoleGetType( TypeAssetLooseFilePath )
{
    // Fetch asset loose file-path.
    return *((StringTableEntry*)dptr);
}

//-----------------------------------------------------------------------------

ConsoleSetType( TypeAssetLooseFilePath )
{
    // Was a single argument specified?
    if( argc == 1 )
    {
        // Yes, so fetch field value.
        const char* pFieldValue = argv[0];

        // Fetch asset loose file-path.
        StringTableEntry* assetLooseFilePath = (StringTableEntry*)(dptr);

        // Update asset loose file-path value.
        *assetLooseFilePath = StringTable->insert(pFieldValue);

        return;
    }

    // Warn.
    Con::warnf( "(TypeAssetLooseFilePath) - Cannot set multiple args to a single asset loose-file." );
}

//-----------------------------------------------------------------------------

ConsoleGetType( TypeAssetId )
{
    // Fetch asset Id.
    return *((StringTableEntry*)dptr);
}

//-----------------------------------------------------------------------------

ConsoleSetType( TypeAssetId )
{
    // Was a single argument specified?
    if( argc == 1 )
    {
        // Yes, so fetch field value.
        const char* pFieldValue = argv[0];

        // Fetch asset Id.
        StringTableEntry* assetId = (StringTableEntry*)(dptr);

        // Update asset value.
        *assetId = StringTable->insert(pFieldValue);

        return;
    }

    // Warn.
    Con::warnf( "(TypeAssetId) - Cannot set multiple args to a single asset." );
}

