//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/assets/particleAssetEmitter.h"

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticleAssetEmitter);

//------------------------------------------------------------------------------

ParticleAssetEmitter::ParticleAssetEmitter()
{
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
