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

IMPLEMENT_CONOBJECT(ParticleAsset);

//------------------------------------------------------------------------------

ParticleAsset::ParticleAsset() :
                    mpSelectedField( NULL ),
                    mLifetime( 0.0f ),
                    mLifeMode( INFINITE )

{
   // Set Vector Associations.
   VECTOR_SET_ASSOCIATION( mEmitters );  

   // Initialize particle fields.
   addParticleField( mParticleLife.getBase(), "life.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mQuantity.getBase(), "quantity.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mSizeX.getBase(), "size.x.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mSizeY.getBase(), "size.y.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mSpeed.getBase(), "speed.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mSpin.getBase(), "spin.scale", 1000.0f, -100.0f, 100.0f, 1.0f );
   addParticleField( mFixedForce.getBase(), "fixedforce.scale", 1000.0f, -100.0f, 100.0f, 1.0f  );
   addParticleField( mRandomMotion.getBase(), "randommotion.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mVisibility.getBase(), "visibility.scale", 1000.0f, 0.0f, 100.0f, 1.0f );
   addParticleField( mEmissionForce.getBase(), "emission.force.base", 1000.0f, -100.0f, 100.0f, 5.0f );
   addParticleField( mEmissionForce.getVaritation(), "emission.force.variation", 1000.0f, 0.0f, 200.0f, 0.0f );
   addParticleField( mEmissionAngle.getBase(), "emission.angle.base", 1000.0f, -180.0f, 180.0f, 0.0f );
   addParticleField( mEmissionAngle.getVaritation(), "emission.angle.variation", 1000.0f, 0.0f, 360.0f, 0.0f );
   addParticleField( mEmissionArc.getBase(), "emission.arc.base", 1000.0f, 0.0f, 360.0f, 360.0f );
   addParticleField( mEmissionArc.getVaritation(), "emission.arc.variation", 1000.0f, 0.0f, 720.0f, 0.0f );
}

//------------------------------------------------------------------------------

ParticleAsset::~ParticleAsset()
{
}

//------------------------------------------------------------------------------

void ParticleAsset::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();
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

void ParticleAsset::initializeAsset( void )
{
    // Call parent.
    Parent::initializeAsset();

    // Currently there is no specific initialization required.
}

//------------------------------------------------------------------------------

void ParticleAsset::addParticleField( ParticleAssetField& particleAssetField, const char* pFieldName, F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue )
{
    // Sanity!
    AssertFatal( pFieldName != NULL && pFieldName != StringTable->EmptyString, "ParticleAsset::addParticleField() - Field name cannot be NULL or empty." );

    // Set the field name.
    particleAssetField.setName( pFieldName );

    // Sanity!
    AssertFatal( !mFields.contains( particleAssetField.getName() ), "ParticleAsset::addParticleField() - The particle field name already exists." );

    // Add to fields.
    mFields.insert( particleAssetField.getName(), &particleAssetField );

    // Set value bounds.
    particleAssetField.setValueBounds( maxTime, minValue, maxValue, defaultValue );
}

