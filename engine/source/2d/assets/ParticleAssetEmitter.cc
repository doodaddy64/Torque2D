//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/assets/particleAssetEmitter.h"

//------------------------------------------------------------------------------

static EnumTable::Enums particleOrientationLookup[] =
                {
                { ParticleAssetEmitter::ALIGNED_ORIENTATION,  "ALIGNED" },
                { ParticleAssetEmitter::FIXED_ORIENTATION,    "FIXED" },
                { ParticleAssetEmitter::RANDOM_ORIENTATION,   "RANDOM" },
                };

//------------------------------------------------------------------------------

ParticleAssetEmitter::ParticleOrientationMode getParticleOrientationMode(const char* label)
{
    // Search for Mnemonic.
    for(U32 i = 0; i < (sizeof(particleOrientationLookup) / sizeof(EnumTable::Enums)); i++)
        if( dStricmp(particleOrientationLookup[i].label, label) == 0)
            return((ParticleAssetEmitter::ParticleOrientationMode)particleOrientationLookup[i].index);

    // Invalid Orientation!
    AssertFatal(false, "ParticleAssetEmitter::getParticleOrientationMode() - Invalid Orientation Mode!");
    // Bah!
    return ParticleAssetEmitter::INVALID_ORIENTATION;
}

//------------------------------------------------------------------------------

static EnumTable::Enums emitterTypeLookup[] =
                {
                { ParticleAssetEmitter::POINT_EMITTER,    "POINT" },
                { ParticleAssetEmitter::LINEX_EMITTER,    "LINEX" },
                { ParticleAssetEmitter::LINEY_EMITTER,    "LINEY" },
                { ParticleAssetEmitter::AREA_EMITTER,     "AREA" },
                };

//------------------------------------------------------------------------------

ParticleAssetEmitter::EmitterType getEmitterType(const char* label)
{
    // Search for Mnemonic.
    for(U32 i = 0; i < (sizeof(emitterTypeLookup) / sizeof(EnumTable::Enums)); i++)
        if( dStricmp(emitterTypeLookup[i].label, label) == 0)
            return((ParticleAssetEmitter::EmitterType)emitterTypeLookup[i].index);

    // Invalid Emitter-Type!
    AssertFatal(false, "ParticleAssetEmitter::getEmitterType() - Invalid Emitter-Type!");

    // Bah!
    return ParticleAssetEmitter::INVALID_EMITTER_TYPE;
}

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticleAssetEmitter);

//------------------------------------------------------------------------------

ParticleAssetEmitter::ParticleAssetEmitter() :
                            mEmitterName( StringTable->EmptyString ),
                            mOwner( NULL ),
                            mEmitterType( POINT_EMITTER ),
                            mFixedAspect( true ),
                            mFixedForceAngle( 90.0f ),
                            mFixedForceDirection( 0.0f, 0.0f ),
                            mParticleOrientationMode( FIXED_ORIENTATION ),
                            mAlignKeepAligned( false ),
                            mAlignAngleOffset( 0.0f ),
                            mRandomAngleOffset( 0.0f ),
                            mRandomArc( 360.0f ),
                            mFixedAngleOffset( 0.0f ),
                            mPivotPoint( 0.0f, 0.0f ),
                            mEmitterEmission( false ),
                            mLinkEmissionRotation( false ),
                            mIntenseParticles( false ),
                            mSingleParticle( false ),
                            mAttachPositionToEmitter( false ),
                            mAttachRotationToEmitter( false ),
                            mOrderedParticles( false ),
                            mFirstInFrontOrder( false ),
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
    mFields.addField( mParticleLife.getBase(), "ParticleLife", 1000.0f, 0.0f, 1000.0f, 2.0f );
    mFields.addField( mParticleLife.getVariation(), "ParticleLifeVariation", 1000.0f, 0.0f, 2000.0f, 0.0f  );
    mFields.addField( mQuantity.getBase(), "Quantity", 1000.0f, 0.0f, 1000.0f, 10.0f );
    mFields.addField( mQuantity.getVariation(), "QuantityVariation", 1000.0f, 0.0f, 1000.0f, 0.0f );
    mFields.addField( mSizeX.getBase(), "SizeX",  1000.0f, 0.0f, 100.0f, 2.0f );
    mFields.addField( mSizeX.getVariation(), "SizeXVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mFields.addField( mSizeX.getLife(), "SizeXLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mFields.addField( mSizeY.getBase(), "SizeY",  1000.0f, 0.0f, 100.0f, 2.0f );
    mFields.addField( mSizeY.getVariation(), "SizeYVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mFields.addField( mSizeY.getLife(), "SizeYLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mFields.addField( mSpeed.getBase(), "Speed", 1000.0f, 0.0f, 100.0f, 10.0f );
    mFields.addField( mSpeed.getVariation(), "SpeedVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mFields.addField( mSpeed.getLife(), "SpeedLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mFields.addField( mSpin.getBase(), "Spin", 1000.0f, -1000.0f, 1000.0f, 0.0f );
    mFields.addField( mSpin.getVariation(), "SpinVariation", 1000.0f, 0.0f, 2000.0f, 0.0f );
    mFields.addField( mSpin.getLife(), "SpinLife", 1.0f, -1000.0f, 1000.0f, 1.0f );
    mFields.addField( mFixedForce.getBase(), "FixedForce", 1000.0f, -100.0f, 100.0f, 0.0f );
    mFields.addField( mFixedForce.getVariation(), "FixedForceVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mFields.addField( mFixedForce.getLife(), "FixedForceLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mFields.addField( mRandomMotion.getBase(), "RandomMotion", 1000.0f, 0.0f, 100.0f, 0.0f );
    mFields.addField( mRandomMotion.getVariation(), "RandomMotionVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mFields.addField( mRandomMotion.getLife(), "RandomMotionLife", 1.0f, -100.0f, 100.0f, 1.0f );
    mFields.addField( mEmissionForce.getBase(), "EmissionForce", 1000.0f, -100.0f, 100.0f, 5.0f );
    mFields.addField( mEmissionForce.getVariation(), "EmissionForceVariation", 1000.0f, -100.0f, 100.0f, 5.0f );
    mFields.addField( mEmissionAngle.getBase(), "EmissionAngle", 1000.0f, -180.0f, 180.0f, 0.0f );
    mFields.addField( mEmissionAngle.getVariation(), "EmissionAngleVariation", 1000.0f, 0.0f, 360.0f, 0.0f );
    mFields.addField( mEmissionArc.getBase(), "EmissionArc", 1000.0f, 0.0f, 360.0f, 360.0f );
    mFields.addField( mEmissionArc.getVariation(), "EmissionArcVariation", 1000.0f, 0.0f, 720.0f, 0.0f );
    mFields.addField( mColourRed.getLife(), "RedChannel", 1.0f, 0.0f, 1.0f, 1.0f );
    mFields.addField( mColourGreen.getLife(), "GreenChannel", 1.0f, 0.0f, 1.0f, 1.0f );
    mFields.addField( mColourBlue.getLife(), "BlueChannel", 1.0f, 0.0f, 1.0f, 1.0f );
    mFields.addField( mVisibility.getLife(), "AlphaChannel", 1.0f, 0.0f, 1.0f, 1.0f );
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
   pParticleAssetEmitter->setOrientationMode( getOrientationMode() );
   pParticleAssetEmitter->setAlignAngleOffset( getAlignAngleOffset() );
   pParticleAssetEmitter->setAlignKeepAligned( getAlignKeepAligned() );
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
   pParticleAssetEmitter->setFirstInFrontOrder( getFirstInFrontOrder() );

   // Copy particle fields.
   mFields.copyTo( pParticleAssetEmitter->mFields );
}

//-----------------------------------------------------------------------------

void ParticleAssetEmitter::setEmitterName( const char* pEmitterName )
{
    // Sanity!
    AssertFatal( mEmitterName != NULL, "ParticleAssetEmitter::setEmitterName() - Cannot set a NULL particle asset emitter name." );

    mEmitterName = StringTable->insert( pEmitterName );
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

    // Finish if no asset.
    if ( mImageAsset.isNull() )
        return false;

    // Is the frame valid?
    if ( frame >= mImageAsset->getFrameCount() )
    {
        // Warn.
        Con::warnf( "ParticleAssetEmitter::setImage() - Invalid frame '%d' for ImageAsset '%s'.", frame, mImageAsset.getAssetId() );
        // Return Here.
        return false;
    }

    // Set Frame.
    mImageFrame = frame;

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

    return true;
}


