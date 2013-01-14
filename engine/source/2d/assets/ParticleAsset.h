//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_H_
#define _PARTICLE_ASSET_H_

#ifndef _ASSET_BASE_H_
#include "assets/assetBase.h"
#endif

#ifndef _PARTICLE_ASSET_EMITTER_H_
#include "2d/assets/particleAssetEmitter.h"
#endif

//-----------------------------------------------------------------------------

class ParticleAsset : public AssetBase
{
private:
    typedef AssetBase  Parent;

    Vector<ParticleAssetEmitter*> mEmitters;

public:
    ParticleAsset();
    virtual ~ParticleAsset();

    static void initPersistFields();
    virtual bool onAdd();
    virtual void onRemove();

    // Asset validation.
    virtual bool isAssetValid( void ) const;

    virtual U32 getTamlChildCount( void ) const
    {
        return (U32)mEmitters.size();
    }

    virtual SimObject* getTamlChild( const U32 childIndex ) const
    {
        // Sanity!
        AssertFatal( childIndex < (U32)mEmitters.size(), "ParticleAsset::getTamlChild() - Child index is out of range." );

        // For when the assert is not used.
        if ( childIndex >= (U32)mEmitters.size() )
            return NULL;

        return mEmitters[ childIndex ];
    }

    virtual void addTamlChild( SimObject* pSimObject )
    {
        // Sanity!
        AssertFatal( pSimObject != NULL, "ParticleAsset::addTamlChild() - Cannot add a NULL child object." );

        // Fetch as particle emitter.
        ParticleAssetEmitter* pEmitter = dynamic_cast<ParticleAssetEmitter*>( pSimObject );

        // Is it a particle emitter?
        if ( pEmitter == NULL )
        {
            // No, so warn.
            Con::warnf( "ParticleAsset::addTamlChild() - Cannot add a child object that isn't a particle emitter." );
            return;
        }

        mEmitters.push_back( pEmitter );
    }

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAsset);

protected:
    virtual void initializeAsset( void );
    virtual void onAssetRefresh( void );
};

#endif // _PARTICLE_ASSET_H_