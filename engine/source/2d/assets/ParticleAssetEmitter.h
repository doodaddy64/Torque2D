//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_EMITTER_H_
#define _PARTICLE_ASSET_EMITTER_H_

#ifndef _SIM_OBJECT_H_
#include "sim/simObject.h"
#endif

//-----------------------------------------------------------------------------

class ParticleAssetEmitter : public SimObject
{
private:
    typedef SimObject Parent;

public:
    ParticleAssetEmitter();
    virtual ~ParticleAssetEmitter();

    static void initPersistFields();
    virtual bool onAdd();
    virtual void onRemove();

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAssetEmitter);
};

#endif // _PARTICLE_ASSET_H_