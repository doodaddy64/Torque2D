//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _COMPOSITE_SPRITE_H_
#include "2d/sceneobject/CompositeSprite.h"
#endif

#ifndef _SPRITE_BATCH_ITEM_H_
#include "2d/core/SpriteBatchItem.h"
#endif

#ifndef _RENDER_PROXY_H_
#include "2d/core/RenderProxy.h"
#endif

// Script bindings.
#include "2d/sceneobject/CompositeSprite_ScriptBinding.h"

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(CompositeSprite);

//------------------------------------------------------------------------------

CompositeSprite::CompositeSprite() :
    mBatchLayoutType( RECTILINEAR_LAYOUT )
{
    // Set as auto-sizing.
    mAutoSizing = true;
}

//------------------------------------------------------------------------------

CompositeSprite::~CompositeSprite()
{
}

//------------------------------------------------------------------------------

void CompositeSprite::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();

    /// Defaults.
    addProtectedField( "DefaultSpriteStride", TypeVector2, Offset(mDefaultSpriteStride, CompositeSprite), &defaultProtectedSetFn, &defaultProtectedGetFn, &defaultProtectedWriteFn, "");
    addProtectedField( "DefaultSpriteSize", TypeVector2, Offset(mDefaultSpriteSize, CompositeSprite), &defaultProtectedSetFn, &defaultProtectedGetFn, &defaultProtectedWriteFn, "");
    addProtectedField( "DefaultSpriteAngle", TypeF32, Offset(mDefaultSpriteSize, CompositeSprite), &setDefaultSpriteAngle, &getDefaultSpriteAngle, &writeDefaultSpriteAngle, "");
    addProtectedField( "BatchLayout", TypeEnum, Offset(mBatchLayoutType, CompositeSprite), &setBatchLayout, &defaultProtectedGetFn, &writeBatchLayout, 1, &batchLayoutTypeTable, "");
    addProtectedField( "BatchCulling", TypeBool, Offset(mBatchCulling, CompositeSprite), &setBatchCulling, &defaultProtectedGetFn, &writeBatchCulling, "");
    addField( "BatchIsolated", TypeBool, Offset(mBatchIsolated, CompositeSprite), &writeBatchIsolated, "");
    addField( "BatchSortMode", TypeEnum, Offset(mBatchSortMode, CompositeSprite), &writeBatchSortMode, 1, &SceneRenderQueue::renderSortTable, "");
}

//-----------------------------------------------------------------------------

void CompositeSprite::preIntegrate( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats )
{
    // Are the spatials dirty?
    if ( getSpatialDirty() )
    {
        // Yes, so update the world transform.
        setBatchTransform( getRenderTransform() );
    }

    // Are the render extents dirty?
    if ( getLocalExtentsDirty() )
    {
        // Yes, so set size as local extents.
        setSize( getLocalExtents() );
    }

    // Call parent.
    Parent::preIntegrate( totalTime, elapsedTime, pDebugStats );
}

//-----------------------------------------------------------------------------

void CompositeSprite::integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats )
{
    // Call Parent.
    Parent::integrateObject( totalTime, elapsedTime, pDebugStats );

    // Finish if the spatials are NOT dirty.
    if ( !getSpatialDirty() )
        return;

    // Update the batch world transform.
    setBatchTransform( getRenderTransform() );
}

//-----------------------------------------------------------------------------

void CompositeSprite::interpolateObject( const F32 timeDelta )
{
    // Call parent.
    Parent::interpolateObject( timeDelta );

    // Finish if the spatials are NOT dirty.
    if ( !getSpatialDirty() )
        return;

    // Update the batch world transform.
    setBatchTransform( getRenderTransform() );
}

//-----------------------------------------------------------------------------

void CompositeSprite::scenePrepareRender( const SceneRenderState* pSceneRenderState, SceneRenderQueue* pSceneRenderQueue )
{
    // Prepare render.
    SpriteBatch::prepareRender( this, pSceneRenderState, pSceneRenderQueue );
}

//-----------------------------------------------------------------------------

void CompositeSprite::sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer )
{
    // Render.
    SpriteBatch::render( pSceneRenderState, pSceneRenderRequest, pBatchRenderer );
}

//------------------------------------------------------------------------------

void CompositeSprite::copyTo(SimObject* object)
{
    // Call to parent.
    Parent::copyTo(object); 

    // Fetch object.
    CompositeSprite* pCompositeSprite = dynamic_cast<CompositeSprite*>(object);

    // Sanity!
    AssertFatal(pCompositeSprite != NULL, "CompositeSprite::copyTo() - Object is not the correct type.");

    // Copy batch layout.
    pCompositeSprite->setBatchLayout( getBatchLayout() );

    // Call sprite batch.
    SpriteBatch::copyTo( dynamic_cast<SpriteBatch*>(object) );
}

//------------------------------------------------------------------------------

void CompositeSprite::setBatchLayout( const BatchLayoutType& batchLayoutType )
{
    // Finish if no change.
    if ( mBatchLayoutType == batchLayoutType )
        return;

    // Do we already have some sprites?
    if ( getSpriteCount() > 0 )
    {
        // Yes, so warn.
        Con::warnf( "CompositeSprite::setBatchLayout() - Changing the batch layout with existing sprites is not allowed.  Clear the sprites first." );
        return;
    }

    // Set layout type.
    mBatchLayoutType = batchLayoutType;
}

//------------------------------------------------------------------------------

SpriteBatchItem* CompositeSprite::createSprite( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Handle layout type appropriately.
    switch( mBatchLayoutType )
    {
        // No layout.
        case NO_LAYOUT:
            return SpriteBatch::createSprite( logicalPosition );

        // Rectilinear layout.
        case RECTILINEAR_LAYOUT:
            return createSpriteRectilinearLayout( logicalPosition );

        // Isometric layout.
        case ISOMETRIC_LAYOUT:
            return createSpriteIsometricLayout( logicalPosition );

        default:
            // Sanity!
            AssertFatal( false, "CompositeSprite::createSprite() - Unknown layout type encountered." );
            return SpriteBatch::createSprite( logicalPosition );
    }
}

//-----------------------------------------------------------------------------

SpriteBatchItem* CompositeSprite::createSpriteRectilinearLayout( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Do we have a valid logical position?
    if ( logicalPosition.getArgCount() != 2 )
    {
        // No, so warn.
        Con::warnf( "Invalid logical position specified for composite sprite." );
        return NULL;
    }

    // Does the sprite position already exist?
    if ( findSpritePosition( logicalPosition ) != NULL )
    {
        // Yes, so warn.
        Con::warnf( "Cannot add sprite at logical position '%s' as one already exists.", logicalPosition.getString() );
        return NULL;
    }

    // Create the sprite.
    SpriteBatchItem* pSpriteBatchItem = SpriteBatch::createSprite();

    // Set sprite logical position.
    pSpriteBatchItem->setLogicalPosition( logicalPosition );

    // Set the sprite default position.
    pSpriteBatchItem->setLocalPosition( logicalPosition.getAsVector2().mult( getDefaultSpriteStride() ) );

    // Set the sprite default size and angle.
    pSpriteBatchItem->setSize( SpriteBatch::getDefaultSpriteSize() );
    pSpriteBatchItem->setLocalAngle( SpriteBatch::getDefaultSpriteAngle() );

    return pSpriteBatchItem;
}

//-----------------------------------------------------------------------------

SpriteBatchItem* CompositeSprite::createSpriteIsometricLayout( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Do we have a valid logical position?
    if ( logicalPosition.getArgCount() != 2 )
    {
        // No, so warn.
        Con::warnf( "Invalid logical position specified for composite rectilinear sprite." );
        return NULL;
    }

    // Does the sprite position already exist?
    if ( findSpritePosition( logicalPosition ) != NULL )
    {
        // Yes, so warn.
        Con::warnf( "Cannot add sprite at logical position '%s' as one already exists.", logicalPosition.getString() );
        return NULL;
    }

    // Create the sprite.
    SpriteBatchItem* pSpriteBatchItem = SpriteBatch::createSprite();

    // Set sprite logical position.
    pSpriteBatchItem->setLogicalPosition( logicalPosition );

    // Fetch sprite stride.
    const Vector2 spriteStride = getDefaultSpriteStride();

    // Fetch logical coordinates.
    Vector2 position = logicalPosition.getAsVector2();

    // Calculate position.
    position.Set( (position.x * spriteStride.x) + (position.y * spriteStride.x), (position.x * spriteStride.y) + (position.y * -spriteStride.y) );

    // Set the sprite default position.
    pSpriteBatchItem->setLocalPosition( position );

    // Set the sprite default size and angle.
    pSpriteBatchItem->setSize( getDefaultSpriteSize() );
    pSpriteBatchItem->setLocalAngle( SpriteBatch::getDefaultSpriteAngle() );

    return pSpriteBatchItem;
}

//-----------------------------------------------------------------------------

void CompositeSprite::onTamlCustomWrite( TamlCollection& customCollection )
{
    // Call parent.
    Parent::onTamlCustomWrite( customCollection );

    // Finish if we have a prefab assigned.
    if ( hasPrefab() )
        return;

    // Fetch sprite count.
    const U32 spriteCount = getSpriteCount();

    // Finish if no sprites.
    if ( spriteCount == 0 )
        return;

    // Add sprites property.
    TamlCollectionProperty* pSpritesProperty = customCollection.addCollectionProperty( StringTable->insert("Sprites") );

    // Write property with sprite batch.
    SpriteBatch::onTamlCustomWrite( pSpritesProperty );
}

//-----------------------------------------------------------------------------

void CompositeSprite::onTamlCustomRead( const TamlCollection& customCollection )
{
    // Call parent.
    Parent::onTamlCustomRead( customCollection );

    // Finish if we have a prefab assigned.
    if ( hasPrefab() )
        return;

    // Find sprites collection.
    const TamlCollectionProperty* pSpritesProperty = customCollection.findProperty( StringTable->insert("Sprites") );

    // Finish if we don't have the property.
    if ( pSpritesProperty == NULL )
        return;

    // Read property with sprite batch.
    SpriteBatch::onTamlCustomRead( pSpritesProperty );
}

//-----------------------------------------------------------------------------

static EnumTable::Enums batchLayoutTypeLookup[] =
                {
                    { CompositeSprite::NO_LAYOUT,            "off"    },
                    { CompositeSprite::RECTILINEAR_LAYOUT,   "rect" },
                    { CompositeSprite::ISOMETRIC_LAYOUT,     "iso"   },
                };

EnumTable batchLayoutTypeTable(sizeof(batchLayoutTypeLookup) / sizeof(EnumTable::Enums), &batchLayoutTypeLookup[0]);

//-----------------------------------------------------------------------------

CompositeSprite::BatchLayoutType getBatchLayoutTypeEnum(const char* label)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(batchLayoutTypeLookup) / sizeof(EnumTable::Enums)); i++)
    {
        if( dStricmp(batchLayoutTypeLookup[i].label, label) == 0)
            return (CompositeSprite::BatchLayoutType)batchLayoutTypeLookup[i].index;
    }

    // Warn!
    Con::warnf("CompositeSprite::getBatchLayoutTypeEnum() - Invalid batch layout type of '%s'", label );

    // Bah!
    return CompositeSprite::INVALID_LAYOUT;
}

//-----------------------------------------------------------------------------

const char* getBatchLayoutTypeDescription(const CompositeSprite::BatchLayoutType batchLayoutType )
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(batchLayoutTypeLookup) / sizeof(EnumTable::Enums)); i++)
    {
        if( batchLayoutTypeLookup[i].index == batchLayoutType )
            return batchLayoutTypeLookup[i].label;
    }

    // Fatal!
    AssertFatal(false, "CompositeSprite::getBatchLayoutTypeDescription() - Invalid batch layout type.");

    // Bah!
    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

bool CompositeSprite::setBatchLayout(void* obj, const char* data)
{
    static_cast<CompositeSprite*>(obj)->setBatchLayout( getBatchLayoutTypeEnum(data) );
    return false;
}
