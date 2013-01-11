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

ParticleAsset::ParticleAsset()
{
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

