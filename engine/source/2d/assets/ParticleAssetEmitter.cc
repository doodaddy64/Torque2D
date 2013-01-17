//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/assets/particleAssetEmitter.h"

#ifndef _PARTICLE_ASSET_H_
#include "2d/assets/particleAsset.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

// Script bindings.
#include "ParticleAssetEmitter_ScriptBinding.h"

//------------------------------------------------------------------------------

static EnumTable::Enums emitterTypeLookup[] =
                {
                { ParticleAssetEmitter::POINT_EMITTER,    "POINT" },
                { ParticleAssetEmitter::LINEX_EMITTER,    "LINEX" },
                { ParticleAssetEmitter::LINEY_EMITTER,    "LINEY" },
                { ParticleAssetEmitter::AREA_EMITTER,     "AREA" },
                };

//------------------------------------------------------------------------------

static EnumTable EmitterTypeTable(sizeof(emitterTypeLookup) / sizeof(EnumTable::Enums), &emitterTypeLookup[0]);

//------------------------------------------------------------------------------

ParticleAssetEmitter::EmitterType ParticleAssetEmitter::getEmitterTypeEnum(const char* label)
{
    // Search for Mnemonic.
    for(U32 i = 0; i < (sizeof(emitterTypeLookup) / sizeof(EnumTable::Enums)); i++)
        if( dStricmp(emitterTypeLookup[i].label, label) == 0)
            return((ParticleAssetEmitter::EmitterType)emitterTypeLookup[i].index);

    // Warn.
    Con::warnf(  "ParticleAssetEmitter::getEmitterTypeEnum() - Invalid emitter-type '%s'.", label );

    return ParticleAssetEmitter::INVALID_EMITTER_TYPE;
}

//------------------------------------------------------------------------------

static EnumTable::Enums particleOrientationTypeLookup[] =
                {
                { ParticleAssetEmitter::ALIGNED_ORIENTATION,  "ALIGNED" },
                { ParticleAssetEmitter::FIXED_ORIENTATION,    "FIXED" },
                { ParticleAssetEmitter::RANDOM_ORIENTATION,   "RANDOM" },
                };

//------------------------------------------------------------------------------

static EnumTable OrientationTypeTable(sizeof(particleOrientationTypeLookup) / sizeof(EnumTable::Enums), &particleOrientationTypeLookup[0]);

//------------------------------------------------------------------------------

ParticleAssetEmitter::ParticleOrientationType ParticleAssetEmitter::getOrientationTypeEnum(const char* label)
{
    // Search for Mnemonic.
    for(U32 i = 0; i < (sizeof(particleOrientationTypeLookup) / sizeof(EnumTable::Enums)); i++)
        if( dStricmp(particleOrientationTypeLookup[i].label, label) == 0)
            return((ParticleAssetEmitter::ParticleOrientationType)particleOrientationTypeLookup[i].index);

    // Warn.
    Con::warnf( "ParticleAssetEmitter::getOrientationTypeEnum() - Invalid orientation type '%s'.", label );

    return ParticleAssetEmitter::INVALID_ORIENTATION;
}

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticleAssetEmitter);

//------------------------------------------------------------------------------

ParticleAssetEmitter::ParticleAssetEmitter() :
                            mEmitterName( StringTable->EmptyString ),
                            mOwner( NULL ),
                            mEmitterType( POINT_EMITTER ),
                            mFixedAspect( true ),
                            mFixedForceAngle( 0.0f ),
                            mFixedForceDirection( 0.0f, 0.0f ),
                            mOrientationType( FIXED_ORIENTATION ),
                            mKeepAligned( false ),
                            mAlignedAngleOffset( 0.0f ),
                            mRandomAngleOffset( 0.0f ),
                            mRandomArc( 360.0f ),
                            mFixedAngleOffset( 0.0f ),
                            mPivotPoint( 0.0f, 0.0f ),
                            mEmitterEmission( false ),
                            mLinkEmissionRotation( false ),
                            mIntenseParticles( false ),
                            mOrderedParticles( false ),
                            mSingleParticle( false ),
                            mAttachPositionToEmitter( false ),
                            mAttachRotationToEmitter( false ),
                            mFirstInFront( false ),
                            mStaticMode( true ),
                            mImageAsset( NULL ),
                            mImageFrame( 0 ),
                            mAnimationAsset( NULL ),
                            mBlendMode( true ),
                            mSrcBlendFactor( GL_SRC_ALPHA ),
                            mDstBlendFactor( GL_ONE_MINUS_SRC_ALPHA ),
                            mAlphaTest( -1.0f )
{
    // Initialize particle fields.
    mParticleFields.addField( mParticleLife.getBase(), "ParticleLife", 1000.0f, 0.0f, 1000.0f, 2.0f );
    mParticleFields.addField( mParticleLife.getVariation(), "ParticleLifeVariation", 1000.0f, 0.0f, 2000.0f, 0.0f  );
    mParticleFields.addField( mQuantity.getBase(), "Quantity", 1000.0f, 0.0f, 1000.0f, 10.0f );
    mParticleFields.addField( mQuantity.getVariation(), "QuantityVariation", 1000.0f, 0.0f, 1000.0f, 0.0f );
    mParticleFields.addField( mSizeX.getBase(), "SizeX",  1000.0f, 0.0f, 100.0f, 2.0f );
    mParticleFields.addField( mSizeX.getVariation(), "SizeXVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mParticleFields.addField( mSizeX.getLife(), "SizeXLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSizeY.getBase(), "SizeY",  1000.0f, 0.0f, 100.0f, 2.0f );
    mParticleFields.addField( mSizeY.getVariation(), "SizeYVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mParticleFields.addField( mSizeY.getLife(), "SizeYLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSpeed.getBase(), "Speed", 1000.0f, 0.0f, 100.0f, 10.0f );
    mParticleFields.addField( mSpeed.getVariation(), "SpeedVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mParticleFields.addField( mSpeed.getLife(), "SpeedLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSpin.getBase(), "Spin", 1000.0f, -1000.0f, 1000.0f, 0.0f );
    mParticleFields.addField( mSpin.getVariation(), "SpinVariation", 1000.0f, 0.0f, 2000.0f, 0.0f );
    mParticleFields.addField( mSpin.getLife(), "SpinLife", 1.0f, -1000.0f, 1000.0f, 1.0f );
    mParticleFields.addField( mFixedForce.getBase(), "FixedForce", 1000.0f, -100.0f, 100.0f, 0.0f );
    mParticleFields.addField( mFixedForce.getVariation(), "FixedForceVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mParticleFields.addField( mFixedForce.getLife(), "FixedForceLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mParticleFields.addField( mRandomMotion.getBase(), "RandomMotion", 1000.0f, 0.0f, 100.0f, 0.0f );
    mParticleFields.addField( mRandomMotion.getVariation(), "RandomMotionVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mParticleFields.addField( mRandomMotion.getLife(), "RandomMotionLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mParticleFields.addField( mEmissionForce.getBase(), "EmissionForce", 1000.0f, -100.0f, 100.0f, 5.0f );
    mParticleFields.addField( mEmissionForce.getVariation(), "EmissionForceVariation", 1000.0f, -100.0f, 100.0f, 5.0f );
    mParticleFields.addField( mEmissionAngle.getBase(), "EmissionAngle", 1000.0f, -180.0f, 180.0f, 0.0f );
    mParticleFields.addField( mEmissionAngle.getVariation(), "EmissionAngleVariation", 1000.0f, 0.0f, 360.0f, 0.0f );
    mParticleFields.addField( mEmissionArc.getBase(), "EmissionArc", 1000.0f, 0.0f, 360.0f, 360.0f );
    mParticleFields.addField( mEmissionArc.getVariation(), "EmissionArcVariation", 1000.0f, 0.0f, 720.0f, 0.0f );
    mParticleFields.addField( mColourRed.getLife(), "RedChannel", 1.0f, 0.0f, 1.0f, 1.0f );
    mParticleFields.addField( mColourGreen.getLife(), "GreenChannel", 1.0f, 0.0f, 1.0f, 1.0f );
    mParticleFields.addField( mColourBlue.getLife(), "BlueChannel", 1.0f, 0.0f, 1.0f, 1.0f );
    mParticleFields.addField( mVisibility.getLife(), "AlphaChannel", 1.0f, 0.0f, 1.0f, 1.0f );

    // Register for refresh notifications.
    mImageAsset.registerRefreshNotify( this );
    mAnimationAsset.registerRefreshNotify( this );
}

//------------------------------------------------------------------------------

ParticleAssetEmitter::~ParticleAssetEmitter()
{
}

//------------------------------------------------------------------------------

void ParticleAssetEmitter::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();

    addProtectedField("EmitterType", TypeEnum, Offset(mEmitterType, ParticleAssetEmitter), &setEmitterType, &defaultProtectedGetFn, &writeEmitterType, 1, &EmitterTypeTable);
    addProtectedField("FixedAspect", TypeBool, Offset(mFixedAspect, ParticleAssetEmitter), &setFixedAspect, &defaultProtectedGetFn, &writeFixedAspect, "");
    addProtectedField("FixedForceAngle", TypeF32, Offset(mFixedForceAngle, ParticleAssetEmitter), &setFixedForceAngle, &defaultProtectedGetFn, &writeFixedForceAngle, "");
    addProtectedField("OrientationType", TypeEnum, Offset(mOrientationType, ParticleAssetEmitter), &setOrientationType, &defaultProtectedGetFn, &writeOrientationType, 1, &OrientationTypeTable);
    addProtectedField("KeepAligned", TypeBool, Offset(mKeepAligned, ParticleAssetEmitter), &setKeepAligned, &defaultProtectedGetFn, &writeKeepAligned, "");
    addProtectedField("AlignedAngleOffset", TypeF32, Offset(mAlignedAngleOffset, ParticleAssetEmitter), &setAlignedAngleOffset, &defaultProtectedGetFn, &writeAlignedAngleOffset, "");
    addProtectedField("RandomAngleOffset", TypeF32, Offset(mRandomAngleOffset, ParticleAssetEmitter), &setRandomAngleOffset, &defaultProtectedGetFn, &writeRandomAngleOffset, "");
    addProtectedField("RandomArc", TypeF32, Offset(mRandomArc, ParticleAssetEmitter), &setRandomArc, &defaultProtectedGetFn, &writeRandomArc, "");
    addProtectedField("FixedAngleOffset", TypeF32, Offset(mFixedAngleOffset, ParticleAssetEmitter), &setFixedAngleOffset, &defaultProtectedGetFn, &writeFixedAngleOffset, "");
    addProtectedField("PivotPoint", TypeVector2, Offset(mPivotPoint, ParticleAssetEmitter), &setPivotPoint, &defaultProtectedGetFn, &writePivotPoint, "");
    addProtectedField("EmitterEmission", TypeBool, Offset(mEmitterEmission, ParticleAssetEmitter), &setEmitterEmission, &defaultProtectedGetFn, &writeEmitterEmission, "");
    addProtectedField("LinkEmissionRotation", TypeBool, Offset(mLinkEmissionRotation, ParticleAssetEmitter), &setLinkEmissionRotation, &defaultProtectedGetFn, &writeLinkEmissionRotation, "");
    addProtectedField("IntenseParticles", TypeBool, Offset(mIntenseParticles, ParticleAssetEmitter), &setIntenseParticles, &defaultProtectedGetFn, &writeIntenseParticles, "");
    addProtectedField("OrderedParticles", TypeBool, Offset(mOrderedParticles, ParticleAssetEmitter), &setOrderedParticles, &defaultProtectedGetFn, &writeOrderedParticles, "");
    addProtectedField("SingleParticle", TypeBool, Offset(mSingleParticle, ParticleAssetEmitter), &setSingleParticle, &defaultProtectedGetFn, &writeSingleParticle, "");
    addProtectedField("AttachPositionToEmitter", TypeBool, Offset(mAttachPositionToEmitter, ParticleAssetEmitter), &setAttachPositionToEmitter, &defaultProtectedGetFn, &writeAttachPositionToEmitter, "");
    addProtectedField("AttachRotationToEmitter", TypeBool, Offset(mAttachRotationToEmitter, ParticleAssetEmitter), &setAttachRotationToEmitter, &defaultProtectedGetFn, &writeAttachRotationToEmitter, "");
    addProtectedField("FirstInFront", TypeBool, Offset(mFirstInFront, ParticleAssetEmitter), &setFirstInFront, &defaultProtectedGetFn, &writeFirstInFront, "");

    addProtectedField("Image", TypeImageAssetPtr, Offset(mImageAsset, ParticleAssetEmitter), &setImage, &getImage, &writeImage, "");
    addProtectedField("Frame", TypeS32, Offset(mImageFrame, ParticleAssetEmitter), &setFrame, &defaultProtectedGetFn, &writeFrame, "");
    addProtectedField("Animation", TypeAnimationAssetPtr, Offset(mAnimationAsset, ParticleAssetEmitter), &setAnimation, &getAnimation, &writeAnimation, "");

    addProtectedField("BlendMode", TypeBool, Offset(mBlendMode, ParticleAssetEmitter), &setBlendMode, &defaultProtectedGetFn, &writeBlendMode, "");
    addProtectedField("SrcBlendFactor", TypeEnum, Offset(mSrcBlendFactor, ParticleAssetEmitter), &setSrcBlendFactor, &defaultProtectedGetFn, &writeSrcBlendFactor, 1, &srcBlendFactorTable, "");
    addProtectedField("DstBlendFactor", TypeEnum, Offset(mDstBlendFactor, ParticleAssetEmitter), &setDstBlendFactor, &defaultProtectedGetFn, &writeDstBlendFactor, 1, &dstBlendFactorTable, "");
    addProtectedField("AlphaTest", TypeF32, Offset(mAlphaTest, ParticleAssetEmitter), &setAlphaTest, &defaultProtectedGetFn, &writeAlphaTest, "");
}

//------------------------------------------------------------------------------

bool ParticleAssetEmitter::onAdd()
{
    // Call Parent.
    if(!Parent::onAdd())
        return false;

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

void ParticleAssetEmitter::onRemove()
{
    // Call Parent.
    Parent::onRemove();
}

//------------------------------------------------------------------------------

void ParticleAssetEmitter::copyTo(SimObject* object)
{
    // Fetch particle asset emitter object.
   ParticleAssetEmitter* pParticleAssetEmitter = static_cast<ParticleAssetEmitter*>( object );

   // Sanity!
   AssertFatal(dynamic_cast<ParticleAssetEmitter*>(object), "ParticleAssetEmitter::copyTo() - Object is not the correct type.");

   // Copy parent.
   Parent::copyTo( object );

   // Copy fields.
   pParticleAssetEmitter->setEmitterName( getEmitterName() );
   pParticleAssetEmitter->setEmitterType( getEmitterType() );
   pParticleAssetEmitter->setFixedAspect( getFixedAspect() );
   pParticleAssetEmitter->setFixedForceAngle( getFixedForceAngle() );
   pParticleAssetEmitter->setOrientationType( getOrientationType() );
   pParticleAssetEmitter->setAlignedAngleOffset( getAlignedAngleOffset() );
   pParticleAssetEmitter->setKeepAligned( getKeepAligned() );
   pParticleAssetEmitter->setRandomAngleOffset( getRandomAngleOffset() );
   pParticleAssetEmitter->setRandomArc( getRandomArc() );
   pParticleAssetEmitter->setFixedAngleOffset( getFixedAngleOffset() );
   pParticleAssetEmitter->setPivotPoint( getPivotPoint() );
   pParticleAssetEmitter->setEmitterEmission( getEmitterEmission() );
   pParticleAssetEmitter->setLinkEmissionRotation( getLinkEmissionRotation() );
   pParticleAssetEmitter->setIntenseParticles( getIntenseParticles() );
   pParticleAssetEmitter->setSingleParticle( getSingleParticle() );
   pParticleAssetEmitter->setAttachPositionToEmitter( getAttachPositionToEmitter() );
   pParticleAssetEmitter->setAttachRotationToEmitter( getAttachPositionToEmitter() );
   pParticleAssetEmitter->setOrderedParticles( getOrderedParticles() );
   pParticleAssetEmitter->setFirstInFront( getFirstInFront() );

   // Copy particle fields.
   mParticleFields.copyTo( pParticleAssetEmitter->mParticleFields );
}

//-----------------------------------------------------------------------------

void ParticleAssetEmitter::setEmitterName( const char* pEmitterName )
{
    // Sanity!
    AssertFatal( mEmitterName != NULL, "ParticleAssetEmitter::setEmitterName() - Cannot set a NULL particle asset emitter name." );

    mEmitterName = StringTable->insert( pEmitterName );

    // Refresh the asset.
    refreshAsset();
}

//-----------------------------------------------------------------------------

void ParticleAssetEmitter::setOwner( ParticleAsset* pParticleAsset )
{
    // Sanity!
    AssertFatal( pParticleAsset != NULL, "ParticleAssetEmitter::setOwner() - Cannot set a NULL owner." );
    AssertFatal( mOwner == NULL, "ParticleAssetEmitter::setOwner() - Cannot set an owner when one is already assigned." );

    // Set owner.
    mOwner = pParticleAsset;
}

//------------------------------------------------------------------------------

void ParticleAssetEmitter::setFixedForceAngle( F32 fixedForceAngle )
{
    // Set Fixed-Force Angle.
    mFixedForceAngle = fixedForceAngle;

    // Refresh the asset.
    refreshAsset();

    // Set Fixed-Force Direction.
    mFixedForceDirection.Set( mSin(mDegToRad(mFixedForceAngle)), mCos(mDegToRad(mFixedForceAngle)) );
}

//------------------------------------------------------------------------------

bool ParticleAssetEmitter::setImage( const char* pAssetId, U32 frame )
{
    // Sanity!
    AssertFatal( pAssetId != NULL, "ParticleAssetEmitter::setImage() - Cannot use a NULL asset Id." );

    // Set static mode.
    mStaticMode = true;

    // Clear animation asset.
    mAnimationAsset.clear();

    // Set asset Id.
    mImageAsset = pAssetId;

    // Is there an asset?
    if ( mImageAsset.notNull() )
    {
        // Yes, so is the frame valid?
        if ( frame >= mImageAsset->getFrameCount() )
        {
            // No, so warn.
            Con::warnf( "ParticleAssetEmitter::setImage() - Invalid frame '%d' for ImageAsset '%s'.", frame, mImageAsset.getAssetId() );
        }
        else
        {
            // Yes, so set the frame.
            mImageFrame = frame;
        }
    }
    else
    {
        // No, so reset the image frame.
        mImageFrame = 0;
    }

    // Refresh the asset.
    refreshAsset();

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

bool ParticleAssetEmitter::setImageFrame( const U32 frame )
{
    // Check Existing ImageMap.
    if ( mImageAsset.isNull() )
    {
        // Warn.
        Con::warnf("ParticleAssetEmitter::setImageFrame() - Cannot set Frame without existing asset Id.");

        // Return Here.
        return false;
    }

    // Check Frame Validity.
    if ( frame >= mImageAsset->getFrameCount() )
    {
        // Warn.
        Con::warnf( "ParticleAssetEmitter::setImageFrame() - Invalid Frame #%d for asset Id '%s'.", frame, mImageAsset.getAssetId() );
        // Return Here.
        return false;
    }

    // Set Frame.
    mImageFrame = frame;

    // Refresh the asset.
    refreshAsset();

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

bool ParticleAssetEmitter::setAnimation( const char* pAnimationAssetId )
{
    // Sanity!
    AssertFatal( pAnimationAssetId != NULL, "ParticleAssetEmitter::setAnimation() - Cannot use NULL asset Id." );

    // Set animated mode.
    mStaticMode = false;

    // Clear static asset.
    mImageAsset.clear();

    // Set animation asset.
    mAnimationAsset = pAnimationAssetId;

    // Refresh the asset.
    refreshAsset();

    return true;
}

//------------------------------------------------------------------------------

inline void ParticleAssetEmitter::refreshAsset( void )
{
    // Finish if no owner.
    if ( mOwner == NULL )
        return;

    // Refresh the asset.
    mOwner->refreshAsset();
}

//------------------------------------------------------------------------------

void ParticleAssetEmitter::onAssetRefreshed( AssetPtrBase* pAssetPtrBase )
{
    // Either the image or animation asset has been refreshed to just refresh the
    // asset that this emitter may belong to.
    ParticleAssetEmitter::refreshAsset();
}

//------------------------------------------------------------------------------

void ParticleAssetEmitter::onTamlCustomWrite( TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAssetEmitter_OnTamlCustomWrite);

    // Write the fields.
    mParticleFields.onTamlCustomWrite( customCollection );
}

//-----------------------------------------------------------------------------

void ParticleAssetEmitter::onTamlCustomRead( const TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAssetEmitter_OnTamlCustomRead);

    // Read the fields.
    mParticleFields.onTamlCustomRead( customCollection );

}

//-----------------------------------------------------------------------------

bool ParticleAssetEmitter::setEmitterType(void* obj, const char* data)
{
    static_cast<ParticleAssetEmitter*>(obj)->setEmitterType( getEmitterTypeEnum(data) );
    return false;
}

//-----------------------------------------------------------------------------

bool ParticleAssetEmitter::setOrientationType(void* obj, const char* data)
{
    static_cast<ParticleAssetEmitter*>(obj)->setOrientationType( getOrientationTypeEnum(data) );
    return false;
}
