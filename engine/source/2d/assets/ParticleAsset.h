//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_H_
#define _PARTICLE_ASSET_H_

#ifndef _PARTICLE_ASSET_EMITTER_H_
#include "2d/assets/particleAssetEmitter.h"
#endif

#ifndef _ASSET_BASE_H_
#include "assets/assetBase.h"
#endif

//-----------------------------------------------------------------------------

class ParticleAsset : public AssetBase
{
private:
    typedef AssetBase  Parent;
    typedef Vector<ParticleAssetEmitter*> typeEmitterVector;

public:
    /// Life Mode.
    enum LifeMode
    {
        INVALID_LIFEMODE,

        INFINITE,
        CYCLE,
        KILL,
        STOP
    };

private:
    typeEmitterVector                       mEmitters;

    F32                                     mLifetime;
    LifeMode                                mLifeMode;

    /// Particle fields.
    ParticleAssetFieldCollection            mFields;
    ParticleAssetFieldBase                  mParticleLife;
    ParticleAssetFieldBase                  mQuantity;
    ParticleAssetFieldBase                  mSizeX;
    ParticleAssetFieldBase                  mSizeY;
    ParticleAssetFieldBase                  mSpeed;
    ParticleAssetFieldBase                  mSpin;
    ParticleAssetFieldBase                  mFixedForce;
    ParticleAssetFieldBase                  mRandomMotion;
    ParticleAssetFieldBaseVariation         mEmissionForce;
    ParticleAssetFieldBaseVariation         mEmissionAngle;
    ParticleAssetFieldBaseVariation         mEmissionArc;
    ParticleAssetFieldBase                  mVisibility;

public:
    ParticleAsset();
    virtual ~ParticleAsset();

    static void initPersistFields();
    virtual bool onAdd();
    virtual void onRemove();
    virtual void copyTo(SimObject* object);

    // Asset validation.
    virtual bool isAssetValid( void ) const;

    void setLifetime( const F32 lifetime );
    F32 getLifetime( void ) const { return mLifetime; }
    void setLifeMode( const LifeMode lifemode );
    LifeMode getLifeMode( void ) const { return mLifeMode; }

    void addEmitter( ParticleAssetEmitter* pParticleAssetEmitter );
    void clearEmitters( void );
    U32 getEmitterCount( void ) const { return (U32)mEmitters.size(); }
    ParticleAssetEmitter* getEmitter( const U32 emitterIndex ) const;
    ParticleAssetEmitter* findEmitter( const char* pEmitterName ) const;
    void moveEmitter( S32 fromIndex, S32 toIndex );

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
        ParticleAssetEmitter* pParticleAssetEmitter = dynamic_cast<ParticleAssetEmitter*>( pSimObject );

        // Is it a particle emitter?
        if ( pParticleAssetEmitter == NULL )
        {
            // No, so warn.
            Con::warnf( "ParticleAsset::addTamlChild() - Cannot add a child object that isn't a particle emitter." );
            return;
        }

        // Add the emitter.
        addEmitter( pParticleAssetEmitter );
    }

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleAsset);

protected:
    virtual void initializeAsset( void );
    virtual void onAssetRefresh( void );

    static bool setLifetime(void* obj, const char* data)                    { static_cast<ParticleAsset*>(obj)->setLifetime(dAtof(data)); return false; }
    static bool writeLifetime( void* obj, StringTableEntry pFieldName )     { return mNotZero( static_cast<ParticleAsset*>(obj)->getLifetime() ); }

    static bool setLifeMode(void* obj, const char* data);
    static bool writeLifeMode( void* obj, StringTableEntry pFieldName )     { return static_cast<ParticleAsset*>(obj)->getLifeMode() != INFINITE; }
};

//-----------------------------------------------------------------------------

extern ParticleAsset::LifeMode getParticleAssetLifeMode(const char* label);

#endif // _PARTICLE_ASSET_H_