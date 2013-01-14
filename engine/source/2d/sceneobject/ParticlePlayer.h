//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_PLAYER_H_
#define _PARTICLE_PLAYER_H_

#ifndef _PARTICLE_ASSET_H_
#include "2d/assets/particleAsset.h"
#endif

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneObject/sceneObject.h"
#endif

//-----------------------------------------------------------------------------

class ParticlePlayer : public SceneObject
{
private:
    typedef SceneObject Parent;

    AssetPtr<ParticleAsset>     mAsset;

    bool                        mEffectPlaying;
    bool                        mEffectPaused;
    F32                         mEffectAge;

    bool						mParticleInterpolation;
    bool                        mCameraIdle;
    F32                         mCameraIdleDistance;

    bool                        mWaitingForParticles;
    bool                        mWaitingForDelete;

public:
    ParticlePlayer();
    virtual ~ParticlePlayer();

    static void initPersistFields();
    virtual bool onAdd();
    virtual void onRemove();

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticlePlayer);
};

#endif // _PARTICLE_PLAYER_H_