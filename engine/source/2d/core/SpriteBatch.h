//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SPRITE_BATCH_H_
#define _SPRITE_BATCH_H_

#ifndef _SPRITE_BATCH_ITEM_H_
#include "2d/core/SpriteBatchItem.h"
#endif

#ifndef _SCENE_RENDER_OBJECT_H_
#include "2d/scene/SceneRenderObject.h"
#endif

//------------------------------------------------------------------------------  

class SpriteBatch
{
public:

    // Represents a logical position.
    struct LogicalPosition
    {
        LogicalPosition( const S32 argCount, const char* args[] )
        {
            // Sanity!
            AssertFatal( argCount > 0 && argCount <= 6, "Cannot set a logical position with no arguments." );

            // Set argument count.
            mArgCount = argCount;

            // Transfer arguments.
            for( S32 index = 0; index < mArgCount; ++index )
            {
                mArgs[index] = StringTable->insert( args[index] );
            }

            mArgKey = NULL;
        }

        inline S32 getArgCount( void ) { return mArgCount; }

        inline StringTableEntry getArg( const S32 argIndex )
        {
            // Is the argument index valid?
            if ( argIndex < 0 || argIndex > 5 )
            {
                // No, so warn.
                Con::warnf( "LogicalPosition::getArg() - Cannot get a logical position argument; index '%d' out of range.", argIndex );
                return StringTable->EmptyString;
            }

            return mArgs[argIndex];
        }

        inline Vector2 getAsVector2( void )
        {
            // Do we have enough arguments?
            if ( mArgCount != 2 )
            {
                // No, so warn.
                Con::warnf( "LogicalPosition::getAsVector2() - Cannot get a logical position as Vector2; not enough arguments." );
                return Vector2::getZero();
            }

            return Vector2( getFloatArg(0), getFloatArg(1) );
        }

        inline F32 getFloatArg( const S32 argIndex ) { return dAtof( getArg(argIndex) ); }
        inline S32 getIntArg( const S32 argIndex ) { return dAtoi( getArg(argIndex) ); }
        inline bool getBoolArg( const S32 argIndex ) { return dAtob( getArg(argIndex) ); }

        inline StringTableEntry getArgKey( void )
        {
            // Return argument key if it already exists.
            if ( mArgKey != NULL )
                return mArgKey;

            // Format key buffer.
            char keyBuffer[4096];
            char* pKeyBuffer = keyBuffer;
            S32 bufferLength = sizeof(keyBuffer);
            for( S32 index = 0; index < mArgCount; ++index )
            {
                const S32 offset = dSprintf(pKeyBuffer, bufferLength, index == 0 ? "%s" : " %s", mArgs[index] );
                pKeyBuffer += offset;
                bufferLength -= offset;
            }

            // Set argument key.
            mArgKey = StringTable->insert( keyBuffer );

            return mArgKey;
        }

    private:
        S32                 mArgCount;
        StringTableEntry    mArgs[6];
        StringTableEntry    mArgKey;
    };

protected:
    /// Sprite batch tree.
    class SpriteBatchTree : public b2DynamicTree
    {
        friend class b2DynamicTree;

    public:
        // Render query.
        inline bool QueryCallback( S32 proxyId )
        {    
            // Fetch sprite batch item.    
            SpriteBatchItem* pSpriteBatchItem = static_cast<SpriteBatchItem*>( GetUserData( proxyId ) );

            // Ignore if not visible.
            if ( !pSpriteBatchItem->getVisible() )
                return true;

            // Use sprite batch item.
            mBatchQuery.push_back( pSpriteBatchItem );

            return true;
        }

        typedef Vector< SpriteBatchItem* > typeSpriteItemVector;
        typeSpriteItemVector mBatchQuery;
    };

public:
    static const S32                INVALID_SPRITE_PROXY = -1;  

protected:
    typedef HashMap< U32, SpriteBatchItem* > typeSpriteBatchHash;
    typedef HashMap< StringTableEntry, U32 > typeSpriteKeyHash;

    typeSpriteBatchHash             mSprites;
    typeSpriteKeyHash               mSpriteLookup;
    SpriteBatchItem*                mSelectedSprite;
    SceneRenderQueue::RenderSort    mBatchSortMode;
    bool                            mBatchCulling;
    Vector2                         mDefaultSpriteStride;
    Vector2                         mDefaultSpriteSize;
    F32                             mDefaultSpriteAngle;

private:
    SpriteBatchTree*                mpSpriteBatchTree;
    U32                             mMasterBatchId;

    b2Transform                     mBatchTransform;
    bool                            mBatchTransformDirty;
    U32                             mBatchTransformId;

    Vector2                         mLocalExtents;
    bool                            mLocalExtentsDirty;

public:
    SpriteBatch();
    virtual ~SpriteBatch();

    void prepareRender( SceneRenderObject* pSceneRenderObject, const SceneRenderState* pSceneRenderState, SceneRenderQueue* pSceneRenderQueue );
    void render( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );

    inline void setBatchTransformDirty( void ) { mBatchTransformDirty = true; mBatchTransformId++; }
    inline bool getBatchTransformDirty( void ) const { return mBatchTransformDirty; }
    inline U32 getBatchTransformId( void ) { return mBatchTransformId; }
    const b2Transform& getBatchTransform( void ) const { return mBatchTransform; }

    inline void setLocalExtentsDirty( void ) { mLocalExtentsDirty = true; }
    inline bool getLocalExtentsDirty( void ) const { return mLocalExtentsDirty; }
    inline const Vector2& getLocalExtents( void ) { if ( getLocalExtentsDirty() ) updateLocalExtents(); return mLocalExtents; }

    void createTreeProxy( const b2AABB& localAABB, SpriteBatchItem* spriteBatchItem );
    void destroyTreeProxy( SpriteBatchItem* spriteBatchItem );
    void moveTreeProxy( SpriteBatchItem* spriteBatchItem, const b2AABB& localAABB );         

    virtual void copyTo( SpriteBatch* pSpriteBatch ) const;

    inline U32 getSpriteCount( void ) { return (U32)mSprites.size(); }

    U32 addSprite( LogicalPosition& logicalPosition );
    bool removeSprite( void );
    virtual void clearSprites( void );

    inline void setBatchSortMode( SceneRenderQueue::RenderSort sortMode ) { mBatchSortMode = sortMode; }
    inline SceneRenderQueue::RenderSort getBatchSortMode( void ) const { return mBatchSortMode; }

    void setBatchCulling( const bool batchCulling );
    inline bool getBatchCulling( void ) { return mBatchCulling; }

    inline void setDefaultSpriteStride( const Vector2& defaultStride ) { mDefaultSpriteStride = defaultStride; }
    inline Vector2 getDefaultSpriteStride( void ) const { return mDefaultSpriteStride; }

    inline void setDefaultSpriteSize( const Vector2& defaultSize ) { mDefaultSpriteSize = defaultSize; }
    inline Vector2 getDefaultSpriteSize( void ) const { return mDefaultSpriteSize; }

    inline void setDefaultSpriteAngle( const F32 defaultAngle ) { mDefaultSpriteAngle = defaultAngle; }
    inline F32 getDefaultSpriteAngle( void ) const { return mDefaultSpriteAngle; }

    bool selectSprite( LogicalPosition& logicalPosition );
    bool selectSpriteId( const U32 batchId );
    inline void deselectSprite( void ) { mSelectedSprite = NULL; }
    bool isSpriteSelected( void ) const { return mSelectedSprite != NULL; }

    void setSpriteImage( const char* pAssetId, const U32 imageFrame = 0 );
    StringTableEntry getSpriteImage( void ) const;
    void setSpriteImageFrame( const U32 imageFrame );
    U32 getSpriteImageFrame( void ) const;
    void setSpriteAnimation( const char* pAssetId, const bool autoRestore = false );
    StringTableEntry getSpriteAnimation( void ) const;
    void clearSpriteAsset( void );

    void setSpriteVisible( const bool visible );
    bool getSpriteVisible( void ) const;

    void setSpriteLocalPosition( const Vector2& localPosition );
    Vector2 getSpriteLocalPosition( void );

    void setSpriteAngle( const F32 localAngle );
    F32 getSpriteAngle( void ) const;

    void setSpriteDepth( const F32 depth );
    F32 getSpriteDepth( void ) const;

    void setSpriteSize( const Vector2& size );
    Vector2 getSpriteSize( void ) const;

    void setSpriteFlipX( const bool flipX );
    bool getSpriteFlipX( void ) const;

    void setSpriteFlipY( const bool flipY );
    bool getSpriteFlipY( void ) const;

    void setSpriteSortPoint( const Vector2& sortPoint );
    Vector2 getSpriteSortPoint( void ) const;

    void setSpriteBlendMode( const bool blendMode );
    bool getSpriteBlendMode( void ) const;
    void setSpriteSrcBlendFactor( GLenum srcBlendFactor );
    GLenum getSpriteSrcBlendFactor( void ) const;
    void setSpriteDstBlendFactor( GLenum dstBlendFactor );
    GLenum getSpriteDstBlendFactor( void ) const;
    void setSpriteBlendColor( const ColorF& blendColor );
    const ColorF& getSpriteBlendColor( void ) const;
    void setSpriteBlendAlpha( const F32 alpha );
    F32  getSpriteBlendAlpha( void ) const;
    
    void setSpriteAlphaTest( const F32 alphaTestMode );
    F32 getSpriteAlphaTest( void ) const;

protected:
    SpriteBatchItem* createSprite( void );
    SpriteBatchItem* findSpriteKey( const StringTableEntry key );
    SpriteBatchItem* findSpriteId( const U32 batchId );

    virtual SpriteBatchItem* createSprite( LogicalPosition& logicalPosition );

    void setBatchTransform( const b2Transform& batchTransform );
    void updateLocalExtents( void );

    void createSpriteBatchTree( void );
    void destroySpriteBatchTree( void );

    void onTamlCustomWrite( TamlCollectionProperty* pSpritesProperty );
    void onTamlCustomRead( const TamlCollectionProperty* pSpritesProperty );

private:
    bool destroySprite( const U32 batchId );
    bool checkSpriteSelected( void ) const;

    b2AABB calculateLocalAABB( const b2AABB& renderAABB );
};

#endif // _SPRITE_BATCH_H_
