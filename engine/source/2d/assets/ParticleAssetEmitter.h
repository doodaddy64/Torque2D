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

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneobject/sceneObject.h"
#endif

//-----------------------------------------------------------------------------

class ParticleAsset;

//-----------------------------------------------------------------------------

class ParticleAssetEmitter : public SimObject, public AssetPtrCallback
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
    ParticleAssetFieldCollection            mParticleFields;
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
    inline StringTableEntry getImage( void ) const { return mImageAsset.getAssetId(); }
    bool setImageFrame( const U32 frame );
    inline U32 getImageFrame( void ) const { return mImageFrame; }
    bool setAnimation( const char* animationName );
    inline StringTableEntry getAnimation( void ) const { return mAnimationAsset.getAssetId(); }
    inline void setBlendMode( bool blendMode ) { mBlendMode = blendMode; refreshAsset(); }
    inline bool getBlendMode( void ) const { return mBlendMode; };
    inline void setSrcBlendFactor( const S32 blendFactor ) { mSrcBlendFactor = blendFactor; refreshAsset(); }
    inline S32 getSrcBlendFactor( void ) const { return mSrcBlendFactor; };
    inline void setDstBlendFactor( const S32 blendFactor ) { mDstBlendFactor = blendFactor; refreshAsset(); }
    inline S32 getDstBlendFactor( void ) const { return mDstBlendFactor; };
    inline void setAlphaTest( const F32 alphaTest ) { mAlphaTest = alphaTest; refreshAsset(); }
    inline F32 getAlphaTest( void ) const { return mAlphaTest; }

    inline ParticleAssetFieldCollection& getParticleFields( void ) { return mParticleFields; }

    static EmitterType getEmitterTypeEnum(const char* label);
    static ParticleOrientationType getOrientationTypeEnum(const char* label);

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAssetEmitter);

private:
    void setOwner( ParticleAsset* pParticleAsset );
    inline void refreshAsset( void );
    virtual void onAssetRefreshed( AssetPtrBase* pAssetPtrBase );
    inline bool isStaticMode( void ) const { return mStaticMode; }

protected:
    void onTamlCustomWrite( TamlCollection& customCollection );
    void onTamlCustomRead( const TamlCollection& customCollection );

    static bool     setEmitterType(void* obj, const char* data);
    static bool     writeEmitterType( void* obj, StringTableEntry pFieldName )          { return static_cast<ParticleAssetEmitter*>(obj)->getEmitterType() == POINT_EMITTER; }
    static bool     setFixedAspect(void* obj, const char* data)                         { static_cast<ParticleAssetEmitter*>(obj)->setFixedAspect(dAtob(data)); return false; }
    static bool     writeFixedAspect( void* obj, StringTableEntry pFieldName )          { return static_cast<ParticleAssetEmitter*>(obj)->getFixedAspect() == false; }
    static bool     setFixedForceAngle(void* obj, const char* data)                     { static_cast<ParticleAssetEmitter*>(obj)->setFixedForceAngle(dAtof(data)); return false; }
    static bool     writeFixedForceAngle( void* obj, StringTableEntry pFieldName )      { return mNotZero(static_cast<ParticleAssetEmitter*>(obj)->getFixedForceAngle() ); }
    static bool     setOrientationType(void* obj, const char* data);
    static bool     writeOrientationType( void* obj, StringTableEntry pFieldName )      { return static_cast<ParticleAssetEmitter*>(obj)->getOrientationType() == FIXED_ORIENTATION; }
    static bool     setKeepAligned(void* obj, const char* data)                         { static_cast<ParticleAssetEmitter*>(obj)->setKeepAligned(dAtob(data)); return false; }
    static bool     writeKeepAligned( void* obj, StringTableEntry pFieldName )          { return static_cast<ParticleAssetEmitter*>(obj)->getKeepAligned() == true; }
    static bool     setAlignedAngleOffset(void* obj, const char* data)                  { static_cast<ParticleAssetEmitter*>(obj)->setAlignedAngleOffset(dAtof(data)); return false; }
    static bool     writeAlignedAngleOffset( void* obj, StringTableEntry pFieldName )   { return mNotZero(static_cast<ParticleAssetEmitter*>(obj)->getAlignedAngleOffset() ); }
    static bool     setRandomAngleOffset(void* obj, const char* data)                   { static_cast<ParticleAssetEmitter*>(obj)->setRandomAngleOffset(dAtof(data)); return false; }
    static bool     writeRandomAngleOffset( void* obj, StringTableEntry pFieldName )    { return mNotZero(static_cast<ParticleAssetEmitter*>(obj)->getRandomAngleOffset() ); }
    static bool     setRandomArc(void* obj, const char* data)                           { static_cast<ParticleAssetEmitter*>(obj)->setRandomArc(dAtof(data)); return false; }
    static bool     writeRandomArc( void* obj, StringTableEntry pFieldName )            { return mNotEqual(static_cast<ParticleAssetEmitter*>(obj)->getRandomArc(), 360.0f ); }
    static bool     setFixedAngleOffset(void* obj, const char* data)                    { static_cast<ParticleAssetEmitter*>(obj)->setFixedAngleOffset(dAtof(data)); return false; }
    static bool     writeFixedAngleOffset( void* obj, StringTableEntry pFieldName )     { return mNotZero(static_cast<ParticleAssetEmitter*>(obj)->getFixedAngleOffset() ); }
    static bool     setPivotPoint(void* obj, const char* data)                          { static_cast<ParticleAssetEmitter*>(obj)->setPivotPoint(Vector2(data)); return false; }
    static bool     writePivotPoint( void* obj, StringTableEntry pFieldName )           { return static_cast<ParticleAssetEmitter*>(obj)->getPivotPoint().notZero(); }
    static bool     setEmitterEmission(void* obj, const char* data)                     { static_cast<ParticleAssetEmitter*>(obj)->setEmitterEmission(dAtob(data)); return false; }
    static bool     writeEmitterEmission( void* obj, StringTableEntry pFieldName )      { return static_cast<ParticleAssetEmitter*>(obj)->getEmitterEmission() == true; }
    static bool     setLinkEmissionRotation(void* obj, const char* data)                { static_cast<ParticleAssetEmitter*>(obj)->setLinkEmissionRotation(dAtob(data)); return false; }
    static bool     writeLinkEmissionRotation( void* obj, StringTableEntry pFieldName ) { return static_cast<ParticleAssetEmitter*>(obj)->getLinkEmissionRotation() == true; }
    static bool     setIntenseParticles(void* obj, const char* data)                    { static_cast<ParticleAssetEmitter*>(obj)->setIntenseParticles(dAtob(data)); return false; }
    static bool     writeIntenseParticles( void* obj, StringTableEntry pFieldName )     { return static_cast<ParticleAssetEmitter*>(obj)->getIntenseParticles() == true; }
    static bool     setOrderedParticles(void* obj, const char* data)                    { static_cast<ParticleAssetEmitter*>(obj)->setOrderedParticles(dAtob(data)); return false; }
    static bool     writeOrderedParticles( void* obj, StringTableEntry pFieldName )     { return static_cast<ParticleAssetEmitter*>(obj)->getOrderedParticles() == true; }
    static bool     setSingleParticle(void* obj, const char* data)                      { static_cast<ParticleAssetEmitter*>(obj)->setSingleParticle(dAtob(data)); return false; }
    static bool     writeSingleParticle( void* obj, StringTableEntry pFieldName )       { return static_cast<ParticleAssetEmitter*>(obj)->getSingleParticle() == true; }
    static bool     setAttachPositionToEmitter(void* obj, const char* data)             { static_cast<ParticleAssetEmitter*>(obj)->setAttachPositionToEmitter(dAtob(data)); return false; }
    static bool     writeAttachPositionToEmitter( void* obj, StringTableEntry pFieldName ) { return static_cast<ParticleAssetEmitter*>(obj)->getAttachPositionToEmitter() == true; }
    static bool     setAttachRotationToEmitter(void* obj, const char* data)             { static_cast<ParticleAssetEmitter*>(obj)->setAttachRotationToEmitter(dAtob(data)); return false; }
    static bool     writeAttachRotationToEmitter( void* obj, StringTableEntry pFieldName ) { return static_cast<ParticleAssetEmitter*>(obj)->getAttachRotationToEmitter() == true; }
    static bool     setFirstInFront(void* obj, const char* data)                        { static_cast<ParticleAssetEmitter*>(obj)->setFirstInFront(dAtob(data)); return false; }
    static bool     writeFirstInFront( void* obj, StringTableEntry pFieldName )         { return static_cast<ParticleAssetEmitter*>(obj)->getFirstInFront() == true; }

    static bool     setImage(void* obj, const char* data)                               { static_cast<ParticleAssetEmitter*>(obj)->setImage(data); return false; };
    static const char* getImage(void* obj, const char* data)                            { return static_cast<ParticleAssetEmitter*>(obj)->getImage(); }
    static bool     writeImage( void* obj, StringTableEntry pFieldName )                { ParticleAssetEmitter* pCastObject = static_cast<ParticleAssetEmitter*>(obj); if ( !pCastObject->isStaticMode() ) return false; return pCastObject->mImageAsset.notNull(); }
    static bool     setFrame(void* obj, const char* data)                               { static_cast<ParticleAssetEmitter*>(obj)->setImageFrame(dAtoi(data)); return false; };
    static bool     writeFrame( void* obj, StringTableEntry pFieldName )                { ParticleAssetEmitter* pCastObject = static_cast<ParticleAssetEmitter*>(obj); if ( !pCastObject->isStaticMode() ) return false; return pCastObject->mImageAsset.notNull(); }
    static bool     setAnimation(void* obj, const char* data)                           { static_cast<ParticleAssetEmitter*>(obj)->setAnimation(data); return false; };
    static const char* getAnimation(void* obj, const char* data)                        { return static_cast<ParticleAssetEmitter*>(obj)->getAnimation(); }
    static bool     writeAnimation( void* obj, StringTableEntry pFieldName )            { ParticleAssetEmitter* pCastObject = static_cast<ParticleAssetEmitter*>(obj); if ( pCastObject->isStaticMode() ) return false; return pCastObject->mAnimationAsset.notNull(); }

    static bool     setBlendMode(void* obj, const char* data)                           { static_cast<ParticleAssetEmitter*>(obj)->setBlendMode(dAtob(data)); return false; }
    static bool     writeBlendMode( void* obj, StringTableEntry pFieldName )            { return static_cast<ParticleAssetEmitter*>(obj)->getBlendMode() == false; }
    static bool     setSrcBlendFactor(void* obj, const char* data)                      { static_cast<ParticleAssetEmitter*>(obj)->setSrcBlendFactor(SceneObject::getSrcBlendFactorEnum(data)); return false; }
    static bool     writeSrcBlendFactor( void* obj, StringTableEntry pFieldName )       { return static_cast<ParticleAssetEmitter*>(obj)->getSrcBlendFactor() != GL_SRC_ALPHA; }
    static bool     setDstBlendFactor(void* obj, const char* data)                      { static_cast<ParticleAssetEmitter*>(obj)->setDstBlendFactor(SceneObject::getDstBlendFactorEnum(data)); return false; }
    static bool     writeDstBlendFactor( void* obj, StringTableEntry pFieldName )       { return static_cast<ParticleAssetEmitter*>(obj)->getDstBlendFactor() != GL_ONE_MINUS_SRC_ALPHA; }
    static bool     setAlphaTest(void* obj, const char* data)                           { static_cast<ParticleAssetEmitter*>(obj)->setAlphaTest(dAtof(data)); return false; }
    static bool     writeAlphaTest( void* obj, StringTableEntry pFieldName )            { return static_cast<ParticleAssetEmitter*>(obj)->getAlphaTest() >= 0.0f; }

};

#endif // _PARTICLE_ASSET_EMITTER_H_