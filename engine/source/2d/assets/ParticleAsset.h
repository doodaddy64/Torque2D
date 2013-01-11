//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_H_
#define _PARTICLE_ASSET_H_

#ifndef _ASSET_BASE_H_
#include "assets/assetBase.h"
#endif

//-----------------------------------------------------------------------------

class ParticleAsset : public AssetBase
{
private:
    typedef AssetBase  Parent;


public:
    ParticleAsset();
    virtual ~ParticleAsset();

    static void initPersistFields();
    virtual bool onAdd();
    virtual void onRemove();

    // Asset validation.
    virtual bool isAssetValid( void ) const;

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAsset);

protected:
    virtual void initializeAsset( void );
    virtual void onAssetRefresh( void );
};

#endif // _PARTICLE_ASSET_H_