//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/sceneobject/particlePlayer.h"

// Script bindings.
#include "2d/sceneobject/particlePlayer_ScriptBinding.h"

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticlePlayer);

//------------------------------------------------------------------------------

ParticlePlayer::ParticlePlayer() :
                    mEffectPaused(false),
                    mEffectPlaying(false),
                    mParticleInterpolation(false),
                    mCameraIdleDistance(0.0f),
                    mCameraIdle(false),
                    mWaitingForParticles(false),
                    mWaitingForDelete(false)
{
}

//------------------------------------------------------------------------------

ParticlePlayer::~ParticlePlayer()
{
}

//------------------------------------------------------------------------------

void ParticlePlayer::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();
}

//------------------------------------------------------------------------------

bool ParticlePlayer::onAdd()
{
    // Call Parent.
    if(!Parent::onAdd())
        return false;

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

void ParticlePlayer::onRemove()
{
    // Call Parent.
    Parent::onRemove();
}
