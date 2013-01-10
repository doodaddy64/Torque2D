//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SPRITE_BATCH_H_
#include "SpriteBatch.h"
#endif

#ifndef _SCENE_RENDER_OBJECT_H_
#include "2d/scene/SceneRenderObject.h"
#endif

//------------------------------------------------------------------------------

SpriteBatch::SpriteBatch() :
    mMasterBatchId( 1 ),
    mSelectedSprite( NULL ),
    mBatchSortMode( SceneRenderQueue::RENDER_SORT_OFF ),
    mDefaultSpriteStride( 1.0f, 1.0f),
    mDefaultSpriteSize( 1.0f, 1.0f ),
    mDefaultSpriteAngle( 0.0f ),
    mpSpriteBatchTree( NULL ),
    mBatchCulling( true )
{
    // Reset batch transform.
    mBatchTransform.SetIdentity();
    mBatchTransformDirty = true;
    mBatchTransformId = 0;

    // Reset local extents.
    mLocalExtents.SetZero();
    mLocalExtentsDirty = true;

    // Create the sprite batch tree if sprite clipping is on.
    createSpriteBatchTree();
}

//------------------------------------------------------------------------------

SpriteBatch::~SpriteBatch()
{
    // Clear the sprites.
    clearSprites();

    // Delete the sprite batch tree.
    destroySpriteBatchTree();
}

//-----------------------------------------------------------------------------

void SpriteBatch::prepareRender( SceneRenderObject* pSceneRenderObject, const SceneRenderState* pSceneRenderState, SceneRenderQueue* pSceneRenderQueue )
{
    // Set the sort mode.
    pSceneRenderQueue->setSortMode( getBatchSortMode() );

    // Calculate local AABB.
    const b2AABB localAABB = calculateLocalAABB( pSceneRenderState->mRenderAABB );

    // Do we have a sprite batch tree?
    if ( mpSpriteBatchTree != NULL )
    {
        // Yes, so fetch sprite batch query.
        SpriteBatchTree::typeSpriteItemVector& batchQuery = mpSpriteBatchTree->mBatchQuery;
        batchQuery.clear();

        // Perform sprite batch query.
        mpSpriteBatchTree->Query( mpSpriteBatchTree, localAABB );

        // Iterate the sprite batch query results.
        for( SpriteBatchTree::typeSpriteItemVector::iterator spriteItr = batchQuery.begin(); spriteItr != batchQuery.end(); ++spriteItr )
        {
            // Fetch sprite batch Item.
            SpriteBatchItem* pSpriteBatchItem = *spriteItr;

            // Skip if not visible.
            if ( !pSpriteBatchItem->getVisible() )
                continue;

            // Create a render request.
            SceneRenderRequest* pSceneRenderRequest = pSceneRenderQueue->createRenderRequest();

            // Prepare batch item.
            pSpriteBatchItem->prepareRender( pSceneRenderRequest, mBatchTransformId );

            // Set identity.
            pSceneRenderRequest->mpSceneRenderObject = pSceneRenderObject;

            // Set custom data.
            pSceneRenderRequest->mpCustomData1 = pSpriteBatchItem;
        }

        // Clear sprite batch query.
        batchQuery.clear();
    }
    else
    {
        // No, so perform a render request for all the sprites.
        for( typeSpriteBatchHash::iterator spriteItr = mSprites.begin(); spriteItr != mSprites.end(); ++spriteItr )
        {
            // Fetch sprite batch Item.
            SpriteBatchItem* pSpriteBatchItem = spriteItr->value;

            // Skip if not visible.
            if ( !pSpriteBatchItem->getVisible() )
                continue;

            // Create a render request.
            SceneRenderRequest* pSceneRenderRequest = pSceneRenderQueue->createRenderRequest();

            // Prepare batch item.
            pSpriteBatchItem->prepareRender( pSceneRenderRequest, mBatchTransformId );

            // Set identity.
            pSceneRenderRequest->mpSceneRenderObject = pSceneRenderObject;

            // Set custom data.
            pSceneRenderRequest->mpCustomData1 = pSpriteBatchItem;
        }
    }
}

//------------------------------------------------------------------------------

void SpriteBatch::render( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer )
{
    // Fetch sprite batch Item.
    SpriteBatchItem* pSpriteBatchItem = (SpriteBatchItem*)pSceneRenderRequest->mpCustomData1;

    // Batch render.
    pSpriteBatchItem->render( pBatchRenderer, pSceneRenderRequest, mBatchTransformId );
}

//------------------------------------------------------------------------------

void SpriteBatch::createTreeProxy( const b2AABB& localAABB, SpriteBatchItem* spriteBatchItem )
{
    // Finish if the batch tree is not available.
    if ( mpSpriteBatchTree == NULL )
        return;

    // Create proxy.
    spriteBatchItem->mProxyId = mpSpriteBatchTree->CreateProxy( localAABB, spriteBatchItem );    
}

//------------------------------------------------------------------------------

void SpriteBatch::destroyTreeProxy( SpriteBatchItem* spriteBatchItem )
{
    // Finish if the batch tree is not available.
    if ( mpSpriteBatchTree == NULL )
        return;

    // Fetch sprite proxy Id.
    const S32 proxyId = spriteBatchItem->getProxyId();

    // Destroy proxy.
    mpSpriteBatchTree->DestroyProxy( proxyId );

    // Remove proxy reference.
    spriteBatchItem->mProxyId = INVALID_SPRITE_PROXY;
}

//------------------------------------------------------------------------------

void SpriteBatch::moveTreeProxy( SpriteBatchItem* spriteBatchItem, const b2AABB& localAABB )
{
    // Finish if the batch tree is not available.
    if ( mpSpriteBatchTree == NULL )
        return;

    // Fetch sprite proxy Id.
    const S32 proxyId = spriteBatchItem->getProxyId();

    // Move proxy.
    mpSpriteBatchTree->MoveProxy( proxyId, localAABB, b2Vec2(0.0f, 0.0f) );
}

//------------------------------------------------------------------------------

void SpriteBatch::copyTo( SpriteBatch* pSpriteBatch ) const
{
    // Clear any existing sprites.
    pSpriteBatch->clearSprites();

    // Set master sprite Id.
    pSpriteBatch->mMasterBatchId = mMasterBatchId;

    // Set batch sort mode.
    pSpriteBatch->setBatchSortMode( getBatchSortMode() );

    // Set batch culling.
    pSpriteBatch->setBatchCulling( getBatchCulling() );

    // Set sprite default size and angle.
    pSpriteBatch->setDefaultSpriteStride( getDefaultSpriteStride() );
    pSpriteBatch->setDefaultSpriteSize( getDefaultSpriteSize() );
    pSpriteBatch->setDefaultSpriteAngle( getDefaultSpriteAngle() );

    // Copy sprites.   
    for( typeSpriteBatchHash::const_iterator spriteItr = mSprites.begin(); spriteItr != mSprites.end(); ++spriteItr )
    {        
        // Create sprite batch item.        
        SpriteBatchItem* pSpriteBatchItem = pSpriteBatch->createSprite();

        // Push a copy to it.
        spriteItr->value->copyTo( pSpriteBatchItem );
    }
}

//------------------------------------------------------------------------------

U32 SpriteBatch::addSprite( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Create sprite layout.
    mSelectedSprite = createSprite( logicalPosition );

    // Finish if no sprite created.
    if ( mSelectedSprite == NULL )
        return 0;

    // Insert into sprite positions.
    mSpritePositions.insert( logicalPosition, mSelectedSprite );

    // Flag local extents as dirty.
    setLocalExtentsDirty();

    return mSelectedSprite->getBatchId();
}

//------------------------------------------------------------------------------

bool SpriteBatch::removeSprite( void )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return false;

    // Remove the sprite position.
    mSpritePositions.erase( mSelectedSprite->getLogicalPosition() );

    // Destroy the sprite.
    destroySprite( mSelectedSprite->getBatchId() );

    // Reset the selected sprite.
    mSelectedSprite = NULL;

    // Flag local extents as dirty.
    setLocalExtentsDirty();

    return true;
}

//------------------------------------------------------------------------------

void SpriteBatch::clearSprites( void )
{
    // Deselect any sprite.
    deselectSprite();

    // Clear sprite positions.
    mSpritePositions.clear();

    // Cache all sprites.
    for( typeSpriteBatchHash::iterator spriteItr = mSprites.begin(); spriteItr != mSprites.end(); ++spriteItr )
    {
        SpriteBatchItemFactory.cacheObject( spriteItr->value );
    }
    mSprites.clear();
    mMasterBatchId = 0;

    // Flag local extents as dirty.
    setLocalExtentsDirty();
}

//------------------------------------------------------------------------------

void SpriteBatch::setBatchCulling( const bool batchCulling )
{
    // Finish if no change.
    if ( mBatchCulling == batchCulling )
        return;

    // Create/destroy sprite batch tree appropriately.
    if ( mBatchCulling )
        createSpriteBatchTree();
    else
        destroySpriteBatchTree();
}

//------------------------------------------------------------------------------

bool SpriteBatch::selectSprite( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Select sprite.
    mSelectedSprite = findSpritePosition( logicalPosition );

    // Finish if we selected the sprite.
    if ( mSelectedSprite != NULL )
        return true;

    // Not selected so warn.
    Con::warnf( "Cannot select sprite at logical position '%s' as one does not exist.", logicalPosition.getString() );

    return false;
}

//------------------------------------------------------------------------------

bool SpriteBatch::selectSpriteId( const U32 batchId )
{
    // Select sprite.
    mSelectedSprite = findSpriteId( batchId );

    // Finish if we selected the sprite.
    if ( mSelectedSprite != NULL )
        return true;

    // Not selected so warn.
    Con::warnf( "Cannot select sprite Id '%d' as it does not exist.", batchId );

    return false;
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteImage( const char* pAssetId, const U32 imageFrame )
{
    // Sanity!
    AssertFatal( pAssetId, "Cannot set sprite image using a NULL asset Id." );

    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set image and frame.
    mSelectedSprite->setImage( pAssetId, imageFrame );
}

//------------------------------------------------------------------------------

StringTableEntry SpriteBatch::getSpriteImage( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return StringTable->EmptyString;

    // Get sprite image.
    return mSelectedSprite->getImage();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteImageFrame( const U32 imageFrame )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set image frame.
    mSelectedSprite->setImageFrame( imageFrame );
}

//------------------------------------------------------------------------------

U32 SpriteBatch::getSpriteImageFrame( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return 0;

    // Get image frame.
    return mSelectedSprite->getImageFrame();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteAnimation( const char* pAssetId, const bool autoRestore )
{
    // Sanity!
    AssertFatal( pAssetId, "Cannot set sprite animation using a NULL asset Id." );

    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set animation.
    mSelectedSprite->setAnimation( pAssetId, autoRestore );
}

//------------------------------------------------------------------------------

StringTableEntry SpriteBatch::getSpriteAnimation( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return StringTable->EmptyString;

    // Get animation.
    return mSelectedSprite->getAnimation();
}

//------------------------------------------------------------------------------

void SpriteBatch::clearSpriteAsset( void )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Clear the asset.
    mSelectedSprite->clearAsset();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteVisible( const bool visible )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set visibility.
    mSelectedSprite->setVisible( visible );
}

//------------------------------------------------------------------------------

bool SpriteBatch::getSpriteVisible( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return false;

    // Get visibility.
    return mSelectedSprite->getVisible();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteLocalPosition( const Vector2& localPosition )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set local position.
    mSelectedSprite->setLocalPosition( localPosition );

    // Flag local extents as dirty.
    setLocalExtentsDirty();
}

//------------------------------------------------------------------------------

Vector2 SpriteBatch::getSpriteLocalPosition( void )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return Vector2::getZero();

    // Get local position.
    return mSelectedSprite->getLocalPosition();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteAngle( const F32 localAngle )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set local angle.
    mSelectedSprite->setLocalAngle( localAngle );

    // Flag local extents as dirty.
    setLocalExtentsDirty();
}

//------------------------------------------------------------------------------

F32 SpriteBatch::getSpriteAngle( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return 0.0f;

    // Get local angle.
    return mSelectedSprite->getLocalAngle();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteDepth( const F32 depth )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set depth.
    mSelectedSprite->setDepth( depth );
}

//------------------------------------------------------------------------------

F32 SpriteBatch::getSpriteDepth( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return 0.0f;

    // Get depth.
    return mSelectedSprite->getDepth();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteSize( const Vector2& size )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set size.
    mSelectedSprite->setSize( size );

    // Flag local extents as dirty.
    setLocalExtentsDirty();
}

//------------------------------------------------------------------------------

Vector2 SpriteBatch::getSpriteSize( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return Vector2::getZero();

    // Get size.
    return mSelectedSprite->getSize();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteFlipX( const bool flipX )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set flip X.
    mSelectedSprite->setFlipX( flipX );
}

//------------------------------------------------------------------------------

bool SpriteBatch::getSpriteFlipX( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return false;

    // Get flip X.
    return mSelectedSprite->getFlipX();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteFlipY( const bool flipY )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set flip Y.
    mSelectedSprite->setFlipY( flipY );
}

//------------------------------------------------------------------------------

bool SpriteBatch::getSpriteFlipY( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return false;

    // Get flip Y.
    return mSelectedSprite->getFlipY();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteSortPoint( const Vector2& sortPoint )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set sort point.
    mSelectedSprite->setSortPoint( sortPoint );
}

//------------------------------------------------------------------------------

Vector2 SpriteBatch::getSpriteSortPoint( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return Vector2::getZero();

    // Get sort point.
    return mSelectedSprite->getSortPoint();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteBlendMode( const bool blendMode )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set blend mode.
    mSelectedSprite->setBlendMode( blendMode );
}

//------------------------------------------------------------------------------

bool SpriteBatch::getSpriteBlendMode( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return true;

    // Get blend mode.
    return mSelectedSprite->getBlendMode();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteSrcBlendFactor( GLenum srcBlendFactor )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set source blend factor.
    mSelectedSprite->setSrcBlendFactor( srcBlendFactor );
}

//------------------------------------------------------------------------------

GLenum SpriteBatch::getSpriteSrcBlendFactor( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return GL_SRC_ALPHA;

    // Get source blend factor.
    return mSelectedSprite->getSrcBlendFactor();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteDstBlendFactor( GLenum dstBlendFactor )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return ;

    // Set destination blend factor.
    mSelectedSprite->setDstBlendFactor( dstBlendFactor );
}

//------------------------------------------------------------------------------

GLenum SpriteBatch::getSpriteDstBlendFactor( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return GL_ONE_MINUS_SRC_ALPHA;

    // Get destination blend factor.
    return mSelectedSprite->getDstBlendFactor();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteBlendColor( const ColorF& blendColor )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set blend color.
    mSelectedSprite->setBlendColor( blendColor );
}

//------------------------------------------------------------------------------

const ColorF& SpriteBatch::getSpriteBlendColor( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return ColorF::StockColor("White");

    // Get blend color.
    return mSelectedSprite->getBlendColor();
}

//------------------------------------------------------------------------------

void SpriteBatch::setSpriteBlendAlpha( const F32 alpha )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set blend alpha.
    mSelectedSprite->setBlendAlpha( alpha );
}

//------------------------------------------------------------------------------

F32 SpriteBatch::getSpriteBlendAlpha( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return 0.0f;

    // Get blend alpha.
    return mSelectedSprite->getBlendAlpha();
}
   
//------------------------------------------------------------------------------

void SpriteBatch::setSpriteAlphaTest( const F32 alphaTestMode )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set alpha-test mode.
    mSelectedSprite->setAlphaTest( alphaTestMode );
}

//------------------------------------------------------------------------------

F32 SpriteBatch::getSpriteAlphaTest( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return -1.0f;

    // Get alpha-test mode.
    return mSelectedSprite->getAlphaTest();
}

//------------------------------------------------------------------------------

void SpriteBatch::setDataObject( SimObject* pDataObject )
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return;

    // Set data object.
    mSelectedSprite->setDataObject( pDataObject );
}

//------------------------------------------------------------------------------

SimObject* SpriteBatch::getDataObject( void ) const
{
    // Finish if a sprite is not selected.
    if ( !checkSpriteSelected() )
        return NULL;

    // Get data object.
    return mSelectedSprite->getDataObject();
}

//------------------------------------------------------------------------------

SpriteBatchItem* SpriteBatch::createSprite( void )
{
    // Allocate batch Id.
    const U32 batchId = mMasterBatchId++;

    // Create sprite batch item,
    SpriteBatchItem* pSpriteBatchItem = SpriteBatchItemFactory.createObject();

    // Set batch parent.
    pSpriteBatchItem->setBatchParent( this, batchId );

    // Create sprite batch item,
    mSprites.insert( batchId, pSpriteBatchItem );

    return pSpriteBatchItem;
}

//------------------------------------------------------------------------------

SpriteBatchItem* SpriteBatch::findSpritePosition( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Find sprite.
    typeSpritePositionHash::iterator spriteItr = mSpritePositions.find( logicalPosition );

    return spriteItr == mSpritePositions.end() ? NULL : spriteItr->value;
}

//------------------------------------------------------------------------------

SpriteBatchItem* SpriteBatch::findSpriteId( const U32 batchId )
{
    // Find sprite.
    typeSpriteBatchHash::iterator spriteItr = mSprites.find( batchId );

    return spriteItr != mSprites.end() ? spriteItr->value : NULL;
}

//------------------------------------------------------------------------------

SpriteBatchItem* SpriteBatch::createSprite( const SpriteBatchItem::LogicalPosition& logicalPosition )
{
    // Do we have a valid logical position?
    if ( logicalPosition.getArgCount() != 2 )
    {
        // No, so warn.
        Con::warnf( "Invalid logical position specified for composite sprite." );
        return NULL;
    }

    // Does the sprite already exist?
    if ( findSpritePosition( logicalPosition ) != NULL )
    {
        // Yes, so warn.
        Con::warnf( "Cannot add sprite at logical position '%s' as one already exists.", logicalPosition.getString() );
        return NULL;
    }

    // Create the sprite.
    SpriteBatchItem* pSpriteBatchItem = createSprite();

    // Set sprite key.
    pSpriteBatchItem->setLogicalPosition( logicalPosition );

    // Set the sprite default position.
    pSpriteBatchItem->setLocalPosition( logicalPosition.getAsVector2() );

    // Set the sprite default size and angle.
    pSpriteBatchItem->setSize( getDefaultSpriteSize() );
    pSpriteBatchItem->setLocalAngle( getDefaultSpriteAngle() );

    return pSpriteBatchItem;
}

//------------------------------------------------------------------------------

void SpriteBatch::setBatchTransform( const b2Transform& batchTransform )
{
    // Update world transform.
    mBatchTransform = batchTransform;

    // Flag the batch transform as dirty.
    setBatchTransformDirty();
}

//------------------------------------------------------------------------------

void SpriteBatch::updateLocalExtents( void )
{
    // Finish if the local extents are not dirty.
    if ( !mLocalExtentsDirty )
        return;

    // Flag as NOT dirty.
    mLocalExtentsDirty = false;

    // Do we have any sprites?
    if ( mSprites.size() == 0 )
    {
        // No, so reset local extents.
        mLocalExtents.setOne();

        return;
    }

    // Fetch first sprite.
    typeSpriteBatchHash::iterator spriteItr = mSprites.begin();

    // Set render AABB to this sprite.
    b2AABB localAABB = spriteItr->value->getLocalAABB();

    // Combine with the rest of the sprites.
    for( ; spriteItr != mSprites.end(); ++spriteItr )
    {
        localAABB.Combine( spriteItr->value->getLocalAABB() );
    }

    // Fetch local render extents.
    const b2Vec2& localLowerExtent = localAABB.lowerBound;
    const b2Vec2& localUpperExtent = localAABB.upperBound;

    // Calculate maximum extents.
    const F32 lowerExtentX = mFabs(localLowerExtent.x);
    const F32 lowerExtentY = mFabs(localLowerExtent.y);
    const F32 upperExtentX = mFabs(localUpperExtent.x);
    const F32 upperExtentY = mFabs(localUpperExtent.y);

    // Calculate local extents.
    mLocalExtents.Set( mFabs(lowerExtentX > upperExtentX ? lowerExtentX : upperExtentX) * 2.0f, mFabs(lowerExtentY > upperExtentY ? lowerExtentY : upperExtentY) * 2.0f );
}

//------------------------------------------------------------------------------

void SpriteBatch::createSpriteBatchTree( void )
{
    // Finish if batch culling is off or there is already a sprite batch tree.
    if ( !mBatchCulling || mpSpriteBatchTree != NULL )
        return;

    // Set the sprite batch tree appropriately.
    mpSpriteBatchTree = new SpriteBatchTree();

    // Finish if there are no sprites.
    if ( mSprites.size() == 0 )
        return;

    // Add proxies for all the sprites.
    for( typeSpriteBatchHash::iterator spriteItr = mSprites.begin(); spriteItr != mSprites.end(); ++spriteItr )
    {
        // Fetch sprite batch item.
        SpriteBatchItem* pSpriteBatchItem = spriteItr->value;

        // Create tree proxy for sprite.
        createTreeProxy( pSpriteBatchItem->getLocalAABB(), pSpriteBatchItem );
    }
}

//------------------------------------------------------------------------------

void SpriteBatch::destroySpriteBatchTree( void )
{
    // Finish if there is no sprite batch tree.
    if ( mpSpriteBatchTree == NULL )
        return;

    // Are there any sprites?
    if ( mSprites.size() > 0 )
    {
        // Yes, so destroy proxies of all the sprites.
        for( typeSpriteBatchHash::iterator spriteItr = mSprites.begin(); spriteItr != mSprites.end(); ++spriteItr )
        {
            // Destroy tree proxy for sprite.
            destroyTreeProxy( spriteItr->value );
        }
    }

    // Finish if sprite clipping 
    delete mpSpriteBatchTree;
}

//------------------------------------------------------------------------------

void SpriteBatch::onTamlCustomWrite( TamlCollectionProperty* pSpritesProperty )
{
    // Fetch property names.
    StringTableEntry spriteItemTypeName = StringTable->insert( "Sprite" );

    // Write all sprites.
    for( typeSpriteBatchHash::iterator spriteItr = mSprites.begin(); spriteItr != mSprites.end(); ++spriteItr )
    {
        // Add type alias.
        TamlPropertyTypeAlias* pSpriteTypeAlias = pSpritesProperty->addTypeAlias( spriteItemTypeName );
        
        // Write type with sprite item.
        spriteItr->value->onTamlCustomWrite( pSpriteTypeAlias );
    }
}

//------------------------------------------------------------------------------

void SpriteBatch::onTamlCustomRead( const TamlCollectionProperty* pSpritesProperty )
{
    // Fetch property names.
    StringTableEntry spriteItemTypeName = StringTable->insert( "Sprite" );

    // Iterate sprite item types.
    for( TamlCollectionProperty::const_iterator spriteTypeAliasItr = pSpritesProperty->begin(); spriteTypeAliasItr != pSpritesProperty->end(); ++spriteTypeAliasItr )
    {
        // Fetch sprite type alias.
        TamlPropertyTypeAlias* pSpriteTypeAlias = *spriteTypeAliasItr;

        // Fetch alias name.
        StringTableEntry aliasName = pSpriteTypeAlias->mAliasName;

        // Is this a known alias?
        if ( aliasName != spriteItemTypeName )
        {
            // No, so warn.
            Con::warnf( "SpriteBatch - Unknown custom type '%s'.", aliasName );
            continue;
        }

        // Create sprite.
        SpriteBatchItem* pSpriteBatchItem = createSprite();

        // Read type with sprite item.
        pSpriteBatchItem->onTamlCustomRead( pSpriteTypeAlias );

        // Fetch logical position.
        const SpriteBatchItem::LogicalPosition& logicalPosition = pSpriteBatchItem->getLogicalPosition();

        // Did we get a logical position?
        if ( logicalPosition.isValid() )
        {
            // Yes, so insert into sprite positions.
            mSpritePositions.insert( logicalPosition, pSpriteBatchItem );
        }
    }
}

//------------------------------------------------------------------------------

bool SpriteBatch::destroySprite( const U32 batchId )
{
    // Find sprite.    
    SpriteBatchItem* pSpriteBatchItem = findSpriteId( batchId );

    // Finish if not found.
    if ( pSpriteBatchItem == NULL )
        return false;

    // Cache sprite.
    SpriteBatchItemFactory.cacheObject( pSpriteBatchItem );

    return true;
}

//------------------------------------------------------------------------------

bool SpriteBatch::checkSpriteSelected( void ) const
{
    // Finish if a sprite is selected.
    if ( mSelectedSprite != NULL )
        return true;

    // No, so warn,
    Con::warnf( "Cannot perform sprite operation no sprite is selected." );

    return false;
}

//------------------------------------------------------------------------------

b2AABB SpriteBatch::calculateLocalAABB( const b2AABB& renderAABB )
{
    // Calculate local OOBB.
    b2Vec2 localOOBB[4];
    CoreMath::mAABBtoOOBB( renderAABB, localOOBB );
    CoreMath::mCalculateInverseOOBB( localOOBB, mBatchTransform, localOOBB );

    // Calculate local AABB.
    b2AABB localAABB;
    CoreMath::mOOBBtoAABB( localOOBB, localAABB );
    
    return localAABB;
}