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

CompositeSprite::CompositeSprite()
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
    addProtectedField("DefaultSpriteStride", TypeVector2, Offset(mDefaultSpriteStride, CompositeSprite), &defaultProtectedSetFn, &defaultProtectedGetFn, &defaultProtectedWriteFn, "");
    addProtectedField("DefaultSpriteSize", TypeVector2, Offset(mDefaultSpriteSize, CompositeSprite), &defaultProtectedSetFn, &defaultProtectedGetFn, &defaultProtectedWriteFn, "");
    addProtectedField("DefaultSpriteAngle", TypeF32, Offset(mDefaultSpriteSize, CompositeSprite), &setDefaultSpriteAngle, &getDefaultSpriteAngle, &writeDefaultSpriteAngle, "");
    addField( "BatchIsolated", TypeBool, Offset(mBatchIsolated, CompositeSprite), &writeBatchIsolated, "");
    addField( "BatchSortMode", TypeEnum, Offset(mSortMode, CompositeSprite), &writeBatchSortMode, 1, &SceneRenderQueue::renderSortTable, "");
    addProtectedField( "BatchCulling", TypeBool, Offset(mSpriteCulling, CompositeSprite), setBatchCulling, &defaultProtectedGetFn, &writeBatchCulling, "");
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

    // Call sprite batch.
    SpriteBatch::copyTo( dynamic_cast<SpriteBatch*>(object) );
}

//------------------------------------------------------------------------------

SpriteBatchItem* CompositeSprite::createSprite( const LogicalPosition& logicalPosition )
{
    // Handle layout type appropriately.
    switch( mLayoutType )
    {
        // No layout.
        case None:
            return SpriteBatch::createSprite( logicalPosition );

        // Rectilinear layout.
        case Rectilinear:
            return createSpriteRectilinearLayout( logicalPosition );

        // Isometric layout.
        case Isometric:
            return createSpriteIsometricLayout( logicalPosition );

        default:
            // Sanity!
            AssertFatal( false, "CompositeSprite::createSprite() - Unknown layout type encountered." );
            return SpriteBatch::createSprite( logicalPosition );
    }
}

//-----------------------------------------------------------------------------

SpriteBatchItem* CompositeSprite::createSpriteRectilinearLayout( const SpriteBatch::LogicalPosition& logicalPosition )
{
    // Do we have a valid logical position?
    if ( logicalPosition.mArgCount != 2 )
    {
        // No, so warn.
        Con::warnf( "Invalid logical position specified for composite sprite." );
        return NULL;
    }

    // Fetch logical position.
    Vector2 position( dAtof(logicalPosition.mArgs[0]), dAtof(logicalPosition.mArgs[1]) );

    // Fetch sprite key.
    const StringTableEntry spriteKey = SpriteBatch::getSpriteKey( logicalPosition );

    // Does the sprite already exist?
    if ( findSpriteKey( spriteKey ) != NULL )
    {
        // Yes, so warn.
        Con::warnf( "Cannot add sprite at logical position '%s' as one already exists.", spriteKey );
        return NULL;
    }

    // Create the sprite.
    SpriteBatchItem* pSpriteBatchItem = SpriteBatch::createSprite();

    // Set sprite key.
    pSpriteBatchItem->setKey( spriteKey );

    // Set the sprite default position.
    pSpriteBatchItem->setLocalPosition( position.mult( getDefaultSpriteStride() ) );

    // Set the sprite default size and angle.
    pSpriteBatchItem->setSize( SpriteBatch::getDefaultSpriteSize() );
    pSpriteBatchItem->setLocalAngle( SpriteBatch::getDefaultSpriteAngle() );

    return pSpriteBatchItem;
}

//-----------------------------------------------------------------------------

SpriteBatchItem* CompositeSprite::createSpriteIsometricLayout( const SpriteBatch::LogicalPosition& logicalPosition )
{
    // Do we have a valid logical position?
    if ( logicalPosition.mArgCount != 2 )
    {
        // No, so warn.
        Con::warnf( "Invalid logical position specified for composite rectilinear sprite." );
        return NULL;
    }

    // Fetch sprite key.
    const StringTableEntry spriteKey = getSpriteKey( logicalPosition );

    // Does the sprite already exist?
    if ( findSpriteKey( spriteKey ) != NULL )
    {
        // Yes, so warn.
        Con::warnf( "Cannot add sprite at logical position '%s' as one already exists.", spriteKey );
        return NULL;
    }

    // Create the sprite.
    SpriteBatchItem* pSpriteBatchItem = SpriteBatch::createSprite();

    // Set sprite key.
    pSpriteBatchItem->setKey( spriteKey );

    // Fetch sprite stride.
    const Vector2 spriteStride = getDefaultSpriteStride();

    // Fetch logical coordinates.
    Vector2 position( dAtof(logicalPosition.mArgs[0]), dAtof(logicalPosition.mArgs[1]) );

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

static EnumTable::Enums compositeLayoutTypeLookup[] =
                {
                    { CompositeSprite::None,        "none"    },
                    { CompositeSprite::Rectilinear, "rect" },
                    { CompositeSprite::Isometric,   "iso"   },
                };

EnumTable compositeLayoutTypeTable(sizeof(compositeLayoutTypeLookup) / sizeof(EnumTable::Enums), &compositeLayoutTypeLookup[0]);
