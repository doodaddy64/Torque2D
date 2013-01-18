//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "console/consoleTypes.h"

#include "2d/assets/ParticleAsset.h"

// Script bindings.
#include "ParticleAsset_ScriptBinding.h"

// Debug Profiling.
#include "debug/profiler.h"

//------------------------------------------------------------------------------

static EnumTable::Enums particleAssetLifeModeLookup[] =
{
    { ParticleAsset::INFINITE,  "INFINITE" },
    { ParticleAsset::CYCLE,     "CYCLE" },
    { ParticleAsset::KILL,      "KILL" },
    { ParticleAsset::STOP,      "STOP" },
};

//-----------------------------------------------------------------------------

static EnumTable LifeModeTable(sizeof(particleAssetLifeModeLookup) / sizeof(EnumTable::Enums), &particleAssetLifeModeLookup[0]);

//-----------------------------------------------------------------------------

ParticleAsset::LifeMode ParticleAsset::getParticleAssetLifeModeEnum( const char* label )
{
   // Search for Mnemonic.
   for(U32 i = 0; i < (sizeof(particleAssetLifeModeLookup) / sizeof(EnumTable::Enums)); i++)
      if( dStricmp(particleAssetLifeModeLookup[i].label, label) == 0)
          return((ParticleAsset::LifeMode)particleAssetLifeModeLookup[i].index);

   // Warn.
   Con::warnf( "ParticleAsset::getParticleAssetLifeModeEnum() - Invalid life mode '%s'.", label );

   return ParticleAsset::INVALID_LIFEMODE;
}

//-----------------------------------------------------------------------------

const char* ParticleAsset::getParticleAssetLifeModeDescription( const ParticleAsset::LifeMode lifeMode )
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(particleAssetLifeModeLookup) / sizeof(EnumTable::Enums)); i++)
    {
        if( particleAssetLifeModeLookup[i].index == (S32)lifeMode )
            return particleAssetLifeModeLookup[i].label;
    }

    // Warn.
    Con::warnf( "ParticleAsset::getParticleAssetLifeModeDescription() - Invalid life-mode." );

    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

ConsoleType( particleAssetPtr, TypeParticleAssetPtr, sizeof(AssetPtr<ParticleAsset>), ASSET_ID_FIELD_PREFIX )

//-----------------------------------------------------------------------------

ConsoleGetType( TypeParticleAssetPtr )
{
    // Fetch asset Id.
    return (*((AssetPtr<ParticleAsset>*)dptr)).getAssetId();
}

//-----------------------------------------------------------------------------

ConsoleSetType( TypeParticleAssetPtr )
{
    // Was a single argument specified?
    if( argc == 1 )
    {
        // Yes, so fetch field value.
        const char* pFieldValue = argv[0];

        // Fetch asset pointer.
        AssetPtr<ParticleAsset>* pAssetPtr = dynamic_cast<AssetPtr<ParticleAsset>*>((AssetPtrBase*)(dptr));

        // Is the asset pointer the correct type?
        if ( pAssetPtr == NULL )
        {
            // No, so fail.
            Con::warnf( "(TypeParticleAssetPtr) - Failed to set asset Id '%d'.", pFieldValue );
            return;
        }

        // Set asset.
        pAssetPtr->setAssetId( pFieldValue );

        return;
   }

    // Warn.
    Con::warnf( "(TypeParticleAssetPtr) - Cannot set multiple args to a single asset." );
}

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticleAsset);

//------------------------------------------------------------------------------

ParticleAsset::ParticleAsset() :
                    mLifetime( 0.0f ),
                    mLifeMode( INFINITE )

{
    // Set Vector Associations.
    VECTOR_SET_ASSOCIATION( mEmitters );  

    // Initialize particle fields.
    mParticleFields.addField( mParticleLife.getBase(), "LifeScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mQuantity.getBase(), "QuantityScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSizeX.getBase(), "SizeXScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSizeY.getBase(), "SizeYScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSpeed.getBase(), "SpeedScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mSpin.getBase(), "SpinScale", 1000.0f, -100.0f, 100.0f, 1.0f );
    mParticleFields.addField( mFixedForce.getBase(), "FixedForceScale", 1000.0f, -100.0f, 100.0f, 1.0f  );
    mParticleFields.addField( mRandomMotion.getBase(), "RandomMotionScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mAlphaChannel.getBase(), "VisibilityScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mParticleFields.addField( mEmissionForce.getBase(), "EmissionForceBase", 1000.0f, -100.0f, 100.0f, 5.0f );
    mParticleFields.addField( mEmissionForce.getVariation(), "EmissionForceVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mParticleFields.addField( mEmissionAngle.getBase(), "EmissionAngleBase", 1000.0f, -180.0f, 180.0f, 0.0f );
    mParticleFields.addField( mEmissionAngle.getVariation(), "EmissionAngleVariation", 1000.0f, 0.0f, 360.0f, 0.0f );
    mParticleFields.addField( mEmissionArc.getBase(), "EmissionArcBase", 1000.0f, 0.0f, 360.0f, 360.0f );
    mParticleFields.addField( mEmissionArc.getVariation(), "EmissionArcVariation", 1000.0f, 0.0f, 720.0f, 0.0f );
}

//------------------------------------------------------------------------------

ParticleAsset::~ParticleAsset()
{
    // Clear the emitters.
    clearEmitters();
}

//------------------------------------------------------------------------------

void ParticleAsset::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();

    addProtectedField("Lifetime", TypeF32, Offset(mLifetime, ParticleAsset), &setLifetime, &defaultProtectedGetFn, &writeLifetime, "");
    addProtectedField("LifeMode", TypeEnum, Offset(mLifeMode, ParticleAsset), &setLifeMode, &defaultProtectedGetFn, &writeLifeMode, 1, &LifeModeTable);
}

//------------------------------------------------------------------------------

bool ParticleAsset::onAdd()
{
    // Call Parent.
    if(!Parent::onAdd())
        return false;

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

void ParticleAsset::onRemove()
{
    // Call Parent.
    Parent::onRemove();
}

//------------------------------------------------------------------------------

void ParticleAsset::copyTo(SimObject* object)
{
    // Fetch particle asset object.
   ParticleAsset* pParticleAsset = static_cast<ParticleAsset*>( object );

   // Sanity!
   AssertFatal( pParticleAsset != NULL, "ParticleAsset::copyTo() - Object is not the correct type.");

   // Copy parent.
   Parent::copyTo( object );

   // Copy fields.
   pParticleAsset->setLifetime( getLifetime() );
   pParticleAsset->setLifeMode( getLifeMode() );

   // Copy particle fields.
   mParticleFields.copyTo( pParticleAsset->mParticleFields );

   // Copy the emitters.
   pParticleAsset->clearEmitters();
   const U32 emitterCount = getEmitterCount();
   for ( U32 index = 0; index < emitterCount; ++index )
   {
       // Fetch emitter.
       ParticleAssetEmitter* pParticleAssetEmitter = getEmitter( index );

       // Create a new emitter.
       ParticleAssetEmitter* pNewEmitter = new ParticleAssetEmitter();
       pParticleAsset->addEmitter( pNewEmitter );

       // Copy emitter.
       pParticleAssetEmitter->copyTo( pNewEmitter );
   }
}

//------------------------------------------------------------------------------

void ParticleAsset::onDeleteNotify( SimObject* object )
{
    // Fetch emitter.
    ParticleAssetEmitter* pParticleAssetEmitter = dynamic_cast<ParticleAssetEmitter*>( object );

    // Ignore if not an emitter.
    if ( pParticleAssetEmitter == NULL )
        return;

    // Iterate emitters.
    for ( typeEmitterVector::iterator emitterItr = mEmitters.begin(); emitterItr != mEmitters.end(); ++emitterItr )
    {
        // Is this the emitter being deleted?
        if ( *emitterItr == object )
        {
            // Yes, so remove it.
            mEmitters.erase( emitterItr );
            return;
        }
    }
}

//------------------------------------------------------------------------------

void ParticleAsset::onAssetRefresh( void ) 
{
    // Ignore if not yet added to the sim.
    if ( !isProperlyAdded() )
        return;

    // Call parent.
    Parent::onAssetRefresh();
}


//------------------------------------------------------------------------------

bool ParticleAsset::isAssetValid( void ) const
{
    return true;
}

//------------------------------------------------------------------------------

void ParticleAsset::setLifetime( const F32 lifetime )
{
    // Ignore no change.
    if ( mIsEqual( lifetime, mLifetime ) )
        return;

    // Is lifetime valid?
    if ( lifetime < 0.0f )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::setLifetime() - Lifetime cannot be negative." );
        return;
    }

    mLifetime = lifetime;

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void ParticleAsset::setLifeMode( const LifeMode lifemode )
{
    // Ignore no change.
    if ( lifemode == mLifeMode )
        return;

    // Is life mode valid?
    if ( lifemode == INVALID_LIFEMODE )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::setLifeMode() - Life mode is invalid." );
        return;
    }

    mLifeMode = lifemode;

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void ParticleAsset::initializeAsset( void )
{
    // Call parent.
    Parent::initializeAsset();

    // Currently there is no specific initialization required.
}

//-----------------------------------------------------------------------------

ParticleAssetEmitter* ParticleAsset::createEmitter( void )
{
    // Create an emitter.
    ParticleAssetEmitter* pParticleAssetEmitter = new ParticleAssetEmitter();

    // Add the emitter.
    if ( addEmitter( pParticleAssetEmitter ) )
        return pParticleAssetEmitter;

    // Error.
    delete pParticleAssetEmitter;
    return NULL;
}

//-----------------------------------------------------------------------------

bool ParticleAsset::addEmitter( ParticleAssetEmitter* pParticleAssetEmitter )
{
    // Sanity!
    AssertFatal( pParticleAssetEmitter != NULL, "Cannot add a NULL particle asset emitter." );

    // Does the particle already have an owner?
    if ( pParticleAssetEmitter->getOwner() != NULL )
    {
        Con::warnf( "ParticleAsset::addEmitter() - Cannot add a particle asset emitter that already has an owner." );
        return false;
    }

    // Is the emitter registered?
    if ( !pParticleAssetEmitter->isProperlyAdded() )
    {
        // No, so register it.
        if ( !pParticleAssetEmitter->registerObject() )
        {
            // Failed so warn.
            Con::warnf( "ParticleAsset::addEmitter() - Failed to register emitter." );
            return false;
        }
    }

    // Set the owner.
    pParticleAssetEmitter->setOwner( this );

    // Add the emitter.
    mEmitters.push_back( pParticleAssetEmitter );

    // Start delete notify.
    deleteNotify( pParticleAssetEmitter );

    return true;
}

//------------------------------------------------------------------------------

void ParticleAsset::removeEmitter( ParticleAssetEmitter* pParticleAssetEmitter, const bool deleteEmitter )
{
    // Sanity!
    AssertFatal( pParticleAssetEmitter != NULL, "Cannot remove a NULL particle asset emitter." );

    // Is this emitter owned by this asset?
    if ( pParticleAssetEmitter->getOwner() != this )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::removeEmitter() - Cannot remove the particle emitter as it is not owned by this particle asset." );
        return;
    }

    // Iterate emitters.
    for ( typeEmitterVector::iterator emitterItr = mEmitters.begin(); emitterItr != mEmitters.end(); ++emitterItr )
    {
        if ( *emitterItr == pParticleAssetEmitter )
        {
            // Remove emitter.
            mEmitters.erase( emitterItr );

            // Remove owner.
            pParticleAssetEmitter->setOwner( NULL );

            // Stop delete notify.
            clearNotify( pParticleAssetEmitter );

            // If requested, delete the emitter.
            if ( deleteEmitter )
                pParticleAssetEmitter->deleteObject();

            return;
        }
    }

    // Warn.
    Con::warnf( "ParticleAsset::removeEmitter() - Cannot remove the particle emitter as it is not part of this particle asset." );
}

//------------------------------------------------------------------------------

void ParticleAsset::clearEmitters( void )
{
    // Remove all emitters.
    while( mEmitters.size() > 0 )
    {
        mEmitters.last()->deleteObject();
        mEmitters.pop_back();
    }
}

//------------------------------------------------------------------------------

ParticleAssetEmitter* ParticleAsset::getEmitter( const U32 emitterIndex ) const
{
    // Is emitter index valid?
    if ( emitterIndex >= (U32)mEmitters.size() )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::getEmitter() - Invalid emitter index." );
        return NULL;
    }

    return mEmitters[emitterIndex];
}

//------------------------------------------------------------------------------

ParticleAssetEmitter* ParticleAsset::findEmitter( const char* pEmitterName ) const
{
    // Sanity!
    AssertFatal( pEmitterName != NULL, "ParticleAsset::findEmitter() - Cannot find a NULL emitter name." );

   // Finish if there are no emitters.
   if ( getEmitterCount() == 0 )
       return NULL;

    // Fetch emitter name.
    StringTableEntry emitterName = StringTable->insert( pEmitterName );

    // Search for emitter..
    for( typeEmitterVector::const_iterator emitterItr = mEmitters.begin(); emitterItr != mEmitters.end(); ++emitterItr )
    {
        if ( (*emitterItr)->getEmitterName() == emitterName )
            return *emitterItr;
    }

    // Not found.
    return NULL;
}

//-----------------------------------------------------------------------------

void ParticleAsset::moveEmitter( S32 fromIndex, S32 toIndex )
{
   // Check From Emitter Index.
   if ( fromIndex < 0 || fromIndex >= (S32)getEmitterCount() )
   {
      // Warn.
      Con::warnf("ParticleAsset::moveEmitter() - Invalid From-Emitter-Index (%d)", fromIndex);
      return;
   }

   // Check To Emitter Index.
   if ( toIndex < 0 || toIndex >= (S32)getEmitterCount() )
   {
      // Warn.
      Con::warnf("ParticleAsset::moveEmitter() - Invalid To-Emitter-Index (%d)", toIndex);
      return;
   }

   // We need to skip an object if we're inserting above the object.
   if ( toIndex > fromIndex )
      toIndex++;
   else
      fromIndex++;

   // Fetch Emitter to be moved.
   typeEmitterVector::iterator fromItr = (mEmitters.address()+fromIndex);

   // Fetch Emitter to be inserted at.
   typeEmitterVector::iterator toItr = (mEmitters.address()+toIndex);

   // Insert Object at new Position.
   mEmitters.insert( toItr, (*fromItr) );

   // Remove Original Reference.
   mEmitters.erase( fromItr );
}

//------------------------------------------------------------------------------

void ParticleAsset::onTamlCustomWrite( TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAsset_OnTamlCustomWrite);

    // Write the fields.
    mParticleFields.onTamlCustomWrite( customCollection );
}

//-----------------------------------------------------------------------------

void ParticleAsset::onTamlCustomRead( const TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAsset_OnTamlCustomRead);

    // Read the fields.
    mParticleFields.onTamlCustomRead( customCollection );
}
