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
    enum ParticleOrientationMode
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
    ParticleOrientationMode                 mParticleOrientationMode;
    F32                                     mAlignAngleOffset;
    bool                                    mAlignKeepAligned;
    F32                                     mRandomAngleOffset;
    F32                                     mRandomArc;
    F32                                     mFixedAngleOffset;
    Vector2                                 mPivotPoint;

    bool                                    mEmitterEmission;
    bool                                    mLinkEmissionRotation;
    bool                                    mIntenseParticles;
    bool                                    mSingleParticle;
    bool                                    mAttachPositionToEmitter;
    bool                                    mAttachRotationToEmitter;
    bool                                    mOrderedParticles;
    bool                                    mFirstInFrontOrder;

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
    inline void setEmitterType( const EmitterType emitterType ) { mEmitterType = emitterType; }
    inline EmitterType getEmitterType( void ) { return mEmitterType; }
    inline void setFixedAspect( const bool fixedAspect ) { mFixedAspect = fixedAspect; }
    inline bool getFixedAspect( void ) const { return mFixedAspect; }
    void setFixedForceAngle( const F32 fixedForceAngle );
    inline F32 getFixedForceAngle( void ) const { return mFixedForceAngle; }
    inline void setOrientationMode( const ParticleOrientationMode particleOrientationMode ) { mParticleOrientationMode = particleOrientationMode; }
    inline ParticleOrientationMode getOrientationMode( void ) const { return mParticleOrientationMode; }
    inline void setAlignAngleOffset( const F32 angleOffset ) { mAlignAngleOffset = angleOffset; }
    inline F32 getAlignAngleOffset( void ) const { return mAlignAngleOffset; }
    inline void setAlignKeepAligned( const bool keepAligned ) { mAlignKeepAligned = keepAligned; }
    inline bool getAlignKeepAligned( void ) const { return mAlignKeepAligned; }
    inline void setRandomAngleOffset( const F32 angleOffset ) { mRandomAngleOffset = angleOffset; };
    inline F32 getRandomAngleOffset( void ) const { return mRandomAngleOffset; }
    inline void setRandomArc( const F32 arc ) { mRandomArc = arc; }
    inline F32 getRandomArc( void ) const { return mRandomArc; }
    inline void setFixedAngleOffset( const F32 angleOffset ) { mFixedAngleOffset = angleOffset; }
    inline F32 getFixedAngleOffset( void ) const { return mFixedAngleOffset; }
    inline void setPivotPoint( const Vector2& pivotPoint ) { mPivotPoint = pivotPoint; }
    inline const Vector2& getPivotPoint( void ) const { return mPivotPoint; }

    inline void setEmitterEmission( const bool emitterEmission ) { mEmitterEmission = emitterEmission; }
    inline bool getEmitterEmission( void ) const { return mEmitterEmission; }
    inline void setLinkEmissionRotation( const bool linkEmissionRotation ) { mLinkEmissionRotation = linkEmissionRotation; }
    inline bool getLinkEmissionRotation( void ) const { return mLinkEmissionRotation; }
    inline void setIntenseParticles( const bool intenseParticles ) { mIntenseParticles = intenseParticles; }
    inline bool getIntenseParticles( void ) const { return mIntenseParticles; }
    inline void setSingleParticle( const bool singleParticle ) { mSingleParticle = singleParticle; }
    inline bool getSingleParticle( void ) const { return mSingleParticle; }
    inline void setAttachPositionToEmitter( const bool attachPositionToEmitter ) { mAttachPositionToEmitter = attachPositionToEmitter; }
    inline bool getAttachPositionToEmitter( void ) const { return mAttachPositionToEmitter; }
    inline void setAttachRotationToEmitter( const bool attachRotationToEmitter ) { mAttachRotationToEmitter = attachRotationToEmitter; }
    inline bool getAttachRotationToEmitter( void ) const { return mAttachRotationToEmitter; }
    inline void setOrderedParticles( const bool ordered ) { mOrderedParticles = ordered; }
    inline bool getOrderedParticles( void ) const { return mOrderedParticles; }
    inline void setFirstInFrontOrder( const bool firstInFrontOrder ) { mFirstInFrontOrder = firstInFrontOrder; }
    inline bool getFirstInFrontOrder( void ) const { return mFirstInFrontOrder; }
   
    bool setImage( const char* imageMapName, const U32 frame = 0 );
    bool setAnimation( const char* animationName );
    inline void setBlendMode( bool blendMode ) { mBlendMode = blendMode; }
    inline bool getBlendMode( void ) const { return mBlendMode; };
    inline void setSrcBlendFactor( const S32 blendFactor ) { mSrcBlendFactor = blendFactor; }
    inline S32 getSrcBlendFactor( void ) const { return mSrcBlendFactor; };
    inline void setDstBlendFactor( const S32 blendFactor ) { mDstBlendFactor = blendFactor; }
    inline S32 getDstBlendFactor( void ) const { return mDstBlendFactor; };

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAssetEmitter);

private:
    void setOwner( ParticleAsset* pParticleAsset );
};

#endif // _PARTICLE_ASSET_EMITTER_H_