//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SPRITE_BATCH_ITEM_H_
#define _SPRITE_BATCH_ITEM_H_

#ifndef _SPRITE_PROXY_BASE_H_
#include "2d/core/SpriteProxyBase.h"
#endif

#ifndef _SIM_OBJECT_PTR_H_
#include "sim/simObjectPtr.h"
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

public:
    // Represents a logical position.
    struct LogicalPosition : public IFactoryObjectReset
    {
        const static S32 MAX_ARGUMENTS = 6;

        LogicalPosition() :
            mArgCount( 0 ),
            mArgString( StringTable->EmptyString ) {}

        LogicalPosition( const char* pLogicalPositionArgs ) :
            mArgString( StringTable->EmptyString )
        {
            // Fetch argument count.
            const char* pLogicalPositionSeparator = ",\t ";
            mArgCount = (S32)StringUnit::getUnitCount( pLogicalPositionArgs, pLogicalPositionSeparator );

            // Do we have a valid argument count?
            if ( mArgCount > MAX_ARGUMENTS )
            {
                // No, so warn.
                Con::warnf( "LogicalPosition: Encountered an invalid logical position of '%s'.", pLogicalPositionArgs );

                // Set no arguments.
                mArgCount = 0;

                return;
            }

            // Set arguments.
            for ( S32 index = 0; index < mArgCount; ++index )
            {
                mArgs[index] = StringUnit::getStringTableUnit( pLogicalPositionArgs, index, pLogicalPositionSeparator );
            }

            // Set argument string.
            mArgString = StringTable->insert( pLogicalPositionArgs );
        }

        inline bool isValid( void ) const { return mArgCount != 0; }

        inline S32 getArgCount( void ) const { return mArgCount; }

        inline StringTableEntry getArg( const S32 argIndex ) const
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

        inline Vector2 getAsVector2( void ) const
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

        inline F32 getFloatArg( const S32 argIndex ) const { return dAtof( getArg(argIndex) ); }
        inline S32 getIntArg( const S32 argIndex ) const { return dAtoi( getArg(argIndex) ); }
        inline bool getBoolArg( const S32 argIndex ) const { return dAtob( getArg(argIndex) ); }

        inline StringTableEntry getString( void ) const { return mArgString; }

        virtual void resetState( void )
        {
            mArgCount = 0;
            dMemset( mArgs, 0, sizeof(mArgs) );
            mArgString = StringTable->EmptyString;
        }

        // This should be as unique as possible as it is used for hashing.
        operator const U32() const
        {
            return (U32)(mArgString) * (U32)2654435761;
        }

        /// Value equality check for hashing.
        bool operator==( const LogicalPosition& logicalPosition ) const
        {
            // Not equal if argument counts are different.
            if ( mArgCount != logicalPosition.getArgCount() )
                return false;

            // Not equal if arguments are different.
            for ( S32 index = 0; index < mArgCount; ++index )
            {
                if ( mArgs[index] != logicalPosition.getArg(index) )
                    return false;
            }

            // Equal.
            return true;
        }

        /// Value inequality check.
        bool operator!=( const LogicalPosition& logicalPosition ) const
        {
            // Not equal if argument counts are different.
            if ( mArgCount != logicalPosition.getArgCount() )
                return true;

            // Not equal if arguments are different.
            for ( S32 index = 0; index < mArgCount; ++index )
            {
                if ( mArgs[index] != logicalPosition.getArg(index) )
                    return true;
            }

            // Equal.
            return false;
        }

    private:
        S32                 mArgCount;
        StringTableEntry    mArgs[MAX_ARGUMENTS];
        StringTableEntry    mArgString;
    };

protected:
    SpriteBatch*        mSpriteBatch;
    U32                 mBatchId;
    S32                 mProxyId;
    StringTableEntry    mName;
    LogicalPosition     mLogicalPosition;

    bool                mVisible;

    Vector2             mLocalPosition;
    F32                 mLocalAngle;
    Vector2             mSize;
    F32                 mDepth;
    bool                mFlipX;
    bool                mFlipY;

    Vector2             mSortPoint;
    StringTableEntry    mRenderGroup;

    bool                mBlendMode;
    GLenum              mSrcBlendFactor;
    GLenum              mDstBlendFactor;
    ColorF              mBlendColor;
    F32                 mAlphaTest;

    SimObjectPtr<SimObject> mDataObject;

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
    inline StringTableEntry getName( void ) const { return mName; }

    inline void setLogicalPosition( const LogicalPosition& logicalPosition ) { mLogicalPosition = logicalPosition; }
    inline const LogicalPosition& getLogicalPosition( void ) const { return mLogicalPosition; }

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
    inline void setRenderGroup( const char* pRenderGroup ) { mRenderGroup = StringTable->insert( pRenderGroup ); }
    inline StringTableEntry getRenderGroup( void ) const { return mRenderGroup; }

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

    inline void setAlphaTest( const F32 alphaTest ) { mAlphaTest = alphaTest; }
    inline F32 getAlphaTest( void ) const { return mAlphaTest; }

    inline void setDataObject( SimObject* pDataObject ) { mDataObject = pDataObject; }
    inline SimObject* getDataObject( void ) const { return mDataObject; }

    virtual void copyTo( SpriteBatchItem* pSpriteBatchItem ) const;

    void prepareRender( SceneRenderRequest* pSceneRenderRequest, const U32 batchTransformId );
    void render( BatchRender* pBatchRenderer, const SceneRenderRequest* pSceneRenderRequest, const U32 batchTransformId );

protected:
    void setBatchParent( SpriteBatch* pSpriteBatch, const U32 batchId );
    inline void setProxyId( const S32 proxyId ) { mProxyId = proxyId; }
    inline void setName( const char* pName ) { mName = StringTable->insert( pName ); }
    void updateLocalTransform( void );
    void updateWorldTransform( const U32 batchTransformId );

    void onTamlCustomWrite( TamlPropertyTypeAlias* pSpriteTypeAlias );
    void onTamlCustomRead( const TamlPropertyTypeAlias* pSpriteTypeAlias );
};

//------------------------------------------------------------------------------  

static FactoryCache<SpriteBatchItem> SpriteBatchItemFactory;

#endif // _SPRITE_BATCH_ITEM_H_
