//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_EMITTER_H_
#define _PARTICLE_ASSET_EMITTER_H_

#ifndef _PARTICLE_ASSET_FIELD_H_
#include "2d/assets/particleAssetFieldCollection.h"
#endif

#ifndef _PARTICLE_ASSET_FIELD_H_
#include "2d/assets/particleAssetField.h"
#endif

#ifndef _IMAGE_ASSET_H_
#include "2d/assets/ImageAsset.h"
#endif

#ifndef _ANIMATION_CONTROLLER_H_
#include "2d/assets/AnimationController.h"
#endif

//-----------------------------------------------------------------------------

class ParticleAsset;

//-----------------------------------------------------------------------------

class ParticleAssetEmitter : public SimObject
{
    friend ParticleAsset;

public:
    enum ParticleOrientationType
    {
        INVALID_ORIENTATION,

        FIXED_ORIENTATION,
        ALIGNED_ORIENTATION,
        RANDOM_ORIENTATION
    };

    enum EmitterType
    {
        INVALID_EMITTER_TYPE,

        POINT_EMITTER,
        LINEX_EMITTER,
        LINEY_EMITTER,
        AREA_EMITTER
    };

private:
    typedef SimObject Parent;

    StringTableEntry                        mEmitterName;
    ParticleAsset*                          mOwner;

    EmitterType                             mEmitterType;
    bool                                    mFixedAspect;
    F32                                     mFixedForceAngle;
    Vector2                                 mFixedForceDirection;
    ParticleOrientationType                 mOrientationType;
    F32                                     mAlignedAngleOffset;
    bool                                    mKeepAligned;
    F32                                     mRandomAngleOffset;
    F32                                     mRandomArc;
    F32                                     mFixedAngleOffset;
    Vector2                                 mPivotPoint;

    bool                                    mEmitterEmission;
    bool                                    mLinkEmissionRotation;
    bool                                    mIntenseParticles;
    bool                                    mOrderedParticles;
    bool                                    mSingleParticle;
    bool                                    mAttachPositionToEmitter;
    bool                                    mAttachRotationToEmitter;
    bool                                    mFirstInFront;

    bool                                    mStaticMode;
    AssetPtr<ImageAsset>                    mImageAsset;
    U32                                     mImageFrame;
    AssetPtr<AnimationAsset>                mAnimationAsset;

    bool                                    mBlendMode;
    S32                                     mSrcBlendFactor;
    S32                                     mDstBlendFactor;
    F32                                     mAlphaTest;

    /// Particle fields.
    ParticleAssetFieldCollection            mFields;
    ParticleAssetFieldBaseVariation         mParticleLife;
    ParticleAssetFieldBaseVariation         mQuantity;
    ParticleAssetFieldBaseVariationLife     mSizeX;
    ParticleAssetFieldBaseVariationLife     mSizeY;
    ParticleAssetFieldBaseVariationLife     mSpeed;
    ParticleAssetFieldBaseVariationLife     mSpin;
    ParticleAssetFieldBaseVariationLife     mFixedForce;
    ParticleAssetFieldBaseVariationLife     mRandomMotion;
    ParticleAssetFieldBaseVariation         mEmissionForce;
    ParticleAssetFieldBaseVariation         mEmissionAngle;
    ParticleAssetFieldBaseVariation         mEmissionArc;
    ParticleAssetFieldLife                  mColourRed;
    ParticleAssetFieldLife                  mColourGreen;
    ParticleAssetFieldLife                  mColourBlue;
    ParticleAssetFieldLife                  mVisibility;

public:
    ParticleAssetEmitter();
    virtual ~ParticleAssetEmitter();

    static void initPersistFields();
    virtual bool onAdd();
    virtual void onRemove();
    virtual void copyTo(SimObject* object);

    void setEmitterName( const char* pEmitterName );
    inline StringTableEntry getEmitterName( void ) const { return mEmitterName; }
    inline ParticleAsset* getOwner( void ) const { return mOwner; }
    inline void setEmitterType( const EmitterType emitterType ) { mEmitterType = emitterType; refreshAsset(); }
    inline EmitterType getEmitterType( void ) { return mEmitterType; }
    inline void setFixedAspect( const bool fixedAspect ) { mFixedAspect = fixedAspect; refreshAsset(); }
    inline bool getFixedAspect( void ) const { return mFixedAspect; }
    void setFixedForceAngle( const F32 fixedForceAngle );
    inline F32 getFixedForceAngle( void ) const { return mFixedForceAngle; }
    inline void setOrientationType( const ParticleOrientationType particleOrientationType ) { mOrientationType = particleOrientationType; refreshAsset(); }
    inline ParticleOrientationType getOrientationType( void ) const { return mOrientationType; }
    inline void setKeepAligned( const bool keepAligned ) { mKeepAligned = keepAligned; refreshAsset(); }
    inline bool getKeepAligned( void ) const { return mKeepAligned; }
    inline void setAlignedAngleOffset( const F32 angleOffset ) { mAlignedAngleOffset = angleOffset; refreshAsset(); }
    inline F32 getAlignedAngleOffset( void ) const { return mAlignedAngleOffset; }
    inline void setRandomAngleOffset( const F32 angleOffset ) { mRandomAngleOffset = angleOffset; refreshAsset(); };
    inline F32 getRandomAngleOffset( void ) const { return mRandomAngleOffset; }
    inline void setRandomArc( const F32 arc ) { mRandomArc = arc; refreshAsset(); }
    inline F32 getRandomArc( void ) const { return mRandomArc; }
    inline void setFixedAngleOffset( const F32 angleOffset ) { mFixedAngleOffset = angleOffset; refreshAsset(); }
    inline F32 getFixedAngleOffset( void ) const { return mFixedAngleOffset; }
    inline void setPivotPoint( const Vector2& pivotPoint ) { mPivotPoint = pivotPoint; refreshAsset(); }
    inline const Vector2& getPivotPoint( void ) const { return mPivotPoint; }

    inline void setEmitterEmission( const bool emitterEmission ) { mEmitterEmission = emitterEmission; refreshAsset(); }
    inline bool getEmitterEmission( void ) const { return mEmitterEmission; }
    inline void setLinkEmissionRotation( const bool linkEmissionRotation ) { mLinkEmissionRotation = linkEmissionRotation; refreshAsset(); }
    inline bool getLinkEmissionRotation( void ) const { return mLinkEmissionRotation; }
    inline void setIntenseParticles( const bool intenseParticles ) { mIntenseParticles = intenseParticles; refreshAsset(); }
    inline bool getIntenseParticles( void ) const { return mIntenseParticles; }
    inline void setSingleParticle( const bool singleParticle ) { mSingleParticle = singleParticle; refreshAsset(); }
    inline bool getSingleParticle( void ) const { return mSingleParticle; }
    inline void setAttachPositionToEmitter( const bool attachPositionToEmitter ) { mAttachPositionToEmitter = attachPositionToEmitter; refreshAsset(); }
    inline bool getAttachPositionToEmitter( void ) const { return mAttachPositionToEmitter; }
    inline void setAttachRotationToEmitter( const bool attachRotationToEmitter ) { mAttachRotationToEmitter = attachRotationToEmitter; refreshAsset(); }
    inline bool getAttachRotationToEmitter( void ) const { return mAttachRotationToEmitter; }
    inline void setOrderedParticles( const bool ordered ) { mOrderedParticles = ordered; refreshAsset(); }
    inline bool getOrderedParticles( void ) const { return mOrderedParticles; }
    inline void setFirstInFront( const bool firstInFront ) { mFirstInFront = firstInFront;refreshAsset();  }
    inline bool getFirstInFront( void ) const { return mFirstInFront; }
   
    bool setImage( const char* imageMapName, const U32 frame = 0 );
    bool setAnimation( const char* animationName );
    inline void setBlendMode( bool blendMode ) { mBlendMode = blendMode; refreshAsset(); }
    inline bool getBlendMode( void ) const { return mBlendMode; };
    inline void setSrcBlendFactor( const S32 blendFactor ) { mSrcBlendFactor = blendFactor; refreshAsset(); }
    inline S32 getSrcBlendFactor( void ) const { return mSrcBlendFactor; };
    inline void setDstBlendFactor( const S32 blendFactor ) { mDstBlendFactor = blendFactor; refreshAsset(); }
    inline S32 getDstBlendFactor( void ) const { return mDstBlendFactor; };

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAssetEmitter);

private:
    void setOwner( ParticleAsset* pParticleAsset );
    inline void refreshAsset( void );

protected:
    void onTamlCustomWrite( TamlCollection& customCollection );
    void onTamlCustomRead( const TamlCollection& customCollection );
};

#endif // _PARTICLE_ASSET_EMITTER_H_