//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SPRITE_BATCH_ITEM_H_
#define _SPRITE_BATCH_ITEM_H_

#ifndef _SPRITE_PROXY_BASE_H_
#include "2d/core/SpriteProxyBase.h"
#endif

#ifndef _FACTORY_CACHE_H_
#include "memory/factoryCache.h"
#endif

//------------------------------------------------------------------------------  

class SpriteBatch;
class SceneRenderRequest;

//------------------------------------------------------------------------------  

class SpriteBatchItem : public SpriteProxyBase
{
    friend class SpriteBatch;

    typedef SpriteProxyBase Parent;

protected:
    SpriteBatch*        mSpriteBatch;
    U32                 mBatchId;
    S32                 mProxyId;
    StringTableEntry    mKey;

    bool                mVisible;

    Vector2             mLocalPosition;
    F32                 mLocalAngle;
    Vector2             mSize;
    F32                 mDepth;
    bool                mFlipX;
    bool                mFlipY;

    Vector2             mSortPoint;

    bool                mBlendMode;
    GLenum              mSrcBlendFactor;
    GLenum              mDstBlendFactor;
    ColorF              mBlendColor;
    F32                 mAlphaTest;

    Vector2             mLocalOOBB[4];
    b2AABB              mLocalAABB;
    bool                mLocalTransformDirty;

    Vector2             mRenderOOBB[4];
    b2AABB              mRenderAABB;
    Vector2             mRenderPosition;
    U32                 mLastBatchTransformId;

public:
    SpriteBatchItem();
    virtual ~SpriteBatchItem();

    virtual void resetState( void );

    inline SpriteBatch* getBatchParent( void ) const { return mSpriteBatch; }
    inline U32 getBatchId( void ) const { return mBatchId; }
    inline S32 getProxyId( void ) const { return mProxyId; }

    inline void setKey( StringTableEntry key ) { mKey = key; }
    inline StringTableEntry getKey( void ) const { return mKey; }

    inline void setVisible( const bool visible ) { mVisible = visible; }
    inline bool getVisible( void ) const { return mVisible; }

    inline void setLocalPosition( const Vector2& localPosition ) { mLocalPosition = localPosition; mLocalTransformDirty = true; }
    inline Vector2 getLocalPosition( void ) const { return mLocalPosition; }

    inline void setLocalAngle( const F32 localAngle ) { mLocalAngle = localAngle; mLocalTransformDirty = true; }
    inline F32 getLocalAngle( void ) const { return mLocalAngle; }

    inline void setSize( const Vector2& size ) { mSize = size; mLocalTransformDirty = true; }
    inline Vector2 getSize( void ) const { return mSize; }

    inline const b2AABB& getLocalAABB( void ) { if ( mLocalTransformDirty ) updateLocalTransform(); return mLocalAABB; }

    void setDepth( const F32 depth ) { mDepth = depth; }
    F32 getDepth( void ) const { return mDepth; }

    inline void setFlipX( const bool flipX ) { mFlipX = flipX; }
    inline bool getFlipX( void ) const { return mFlipX; }

    inline void setFlipY( const bool flipY ) { mFlipY = flipY; }
    inline bool getFlipY( void ) const { return mFlipY; }

    inline void setSortPoint( const Vector2& sortPoint ) { mSortPoint = sortPoint; }
    inline Vector2 getSortPoint( void ) const { return mSortPoint; }

    inline void setBlendMode( const bool blendMode ) { mBlendMode = blendMode; }
    inline bool getBlendMode( void ) const { return mBlendMode; }
    inline void setSrcBlendFactor( GLenum srcBlendFactor ) { mSrcBlendFactor = srcBlendFactor; }
    inline GLenum getSrcBlendFactor( void ) const { return mSrcBlendFactor; }
    inline void setDstBlendFactor( GLenum dstBlendFactor ) { mDstBlendFactor = dstBlendFactor; }
    inline GLenum getDstBlendFactor( void ) const { return mDstBlendFactor; }
    inline void setBlendColor( const ColorF& blendColor ) { mBlendColor = blendColor; }
    inline const ColorF& getBlendColor( void ) const { return mBlendColor; }
    inline void setBlendAlpha( const F32 alpha ) { mBlendColor.alpha = alpha; }
    inline F32 getBlendAlpha( void ) const { return mBlendColor.alpha; }

    /// Set alpha-test de.
    inline void setAlphaTest( const F32 alphaTest ) { mAlphaTest = alphaTest; }
    inline F32 getAlphaTest( void ) const { return mAlphaTest; }

    virtual void copyTo( SpriteBatchItem* pSpriteBatchItem ) const;

    void prepareRender( SceneRenderRequest* pSceneRenderRequest, const U32 batchTransformId );
    void render( BatchRender* pBatchRenderer, const U32 batchTransformId );

protected:
    void setBatchParent( SpriteBatch* pSpriteBatch, const U32 batchId );
    inline void setProxyId( const S32 proxyId ) { mProxyId = proxyId; }
    void updateLocalTransform( void );
    void updateWorldTransform( const U32 batchTransformId );

    void onTamlCustomWrite( TamlPropertyTypeAlias* pSpriteTypeAlias );
    void onTamlCustomRead( const TamlPropertyTypeAlias* pSpriteTypeAlias );
};

//------------------------------------------------------------------------------  

static FactoryCache<SpriteBatchItem> SpriteBatchItemFactory;

#endif // _SPRITE_BATCH_ITEM_H_
