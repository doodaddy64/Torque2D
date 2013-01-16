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

ParticleAsset::LifeMode getParticleAssetLifeMode(const char* label)
{
   // Search for Mnemonic.
   for(U32 i = 0; i < (sizeof(particleAssetLifeModeLookup) / sizeof(EnumTable::Enums)); i++)
      if( dStricmp(particleAssetLifeModeLookup[i].label, label) == 0)
          return((ParticleAsset::LifeMode)particleAssetLifeModeLookup[i].index);

   // Invalid Effect Life-Mode!
   AssertFatal(false, "ParticleAsset::getParticleAssetLifeMode() - Invalid life mode.");

   // Invalid.
   return ParticleAsset::INVALID_LIFEMODE;
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
    mFields.addField( mParticleLife.getBase(), "LifeScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mQuantity.getBase(), "QuantityScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mSizeX.getBase(), "SizeXScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mSizeY.getBase(), "SizeYScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mSpeed.getBase(), "SpeedScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mSpin.getBase(), "SpinScale", 1000.0f, -100.0f, 100.0f, 1.0f );
    mFields.addField( mFixedForce.getBase(), "FixedForceScale", 1000.0f, -100.0f, 100.0f, 1.0f  );
    mFields.addField( mRandomMotion.getBase(), "RandomMotionScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mVisibility.getBase(), "VisibilityScale", 1000.0f, 0.0f, 100.0f, 1.0f );
    mFields.addField( mEmissionForce.getBase(), "EmissionForceBase", 1000.0f, -100.0f, 100.0f, 5.0f );
    mFields.addField( mEmissionForce.getVariation(), "EmissionForceVariation", 1000.0f, 0.0f, 200.0f, 0.0f );
    mFields.addField( mEmissionAngle.getBase(), "EmissionAngleBase", 1000.0f, -180.0f, 180.0f, 0.0f );
    mFields.addField( mEmissionAngle.getVariation(), "EmissionAngleVariation", 1000.0f, 0.0f, 360.0f, 0.0f );
    mFields.addField( mEmissionArc.getBase(), "EmissionArcBase", 1000.0f, 0.0f, 360.0f, 360.0f );
    mFields.addField( mEmissionArc.getVariation(), "EmissionArcVariation", 1000.0f, 0.0f, 720.0f, 0.0f );
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
   AssertFatal(dynamic_cast<ParticleAsset*>(object), "ParticleAsset::copyTo() - Object is not the correct type.");

   // Copy parent.
   Parent::copyTo( object );

   // Copy fields.
   pParticleAsset->setLifetime( getLifetime() );
   pParticleAsset->setLifeMode( getLifeMode() );

   // Copy particle fields.
   mFields.copyTo( pParticleAsset->mFields );

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

void ParticleAsset::addEmitter( ParticleAssetEmitter* pParticleAssetEmitter )
{
    // Sanity!
    AssertFatal( pParticleAssetEmitter != NULL, "Cannot add a NULL particle asset emitter." );
    AssertFatal( pParticleAssetEmitter->getOwner() == NULL, "Cannot add a particle asset emitter that already has an owner." );

    // Is the emitter registered?
    if ( !pParticleAssetEmitter->isProperlyAdded() )
    {
        // No, so register it.
        if ( !pParticleAssetEmitter->registerObject() )
        {
            // Failed so warn.
            Con::warnf( "ParticleAsset::addEmitter() - Failed to register emitter." );
            return;
        }
    }

    // Set the owner.
    pParticleAssetEmitter->setOwner( this );

    // Add the emitter.
    mEmitters.push_back( pParticleAssetEmitter );
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

//-----------------------------------------------------------------------------

bool ParticleAsset::setLifeMode(void* obj, const char* data)
{
    static_cast<ParticleAsset*>(obj)->setLifeMode(getParticleAssetLifeMode(data));
    return false;
}

//------------------------------------------------------------------------------

void ParticleAsset::onTamlCustomWrite( TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAsset_OnTamlCustomWrite);

    // Write the fields.
    mFields.onTamlCustomWrite( customCollection );
}

//-----------------------------------------------------------------------------

void ParticleAsset::onTamlCustomRead( const TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAsset_OnTamlCustomRead);

    // Read the fields.
    mFields.onTamlCustomRead( customCollection );
}
