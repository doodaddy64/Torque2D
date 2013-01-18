//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_PLAYER_H_
#define _PARTICLE_PLAYER_H_

#ifndef _PARTICLE_ASSET_H_
#include "2d/assets/particleAsset.h"
#endif

#ifndef _PARTICLE_SYSTEM_H_
#include "2d/core/particleSystem.h"
#endif

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneObject/sceneObject.h"
#endif

#ifndef _ANIMATION_CONTROLLER_H_
#include "2d/assets/AnimationController.h"
#endif

//-----------------------------------------------------------------------------

class ParticlePlayer : public SceneObject, public AssetPtrCallback
{
private:
    typedef SceneObject Parent;


    /// Emitter node.
    class EmitterNode
    {
    private:
        ParticlePlayer*                 mOwner;
        ParticleAssetEmitter*           mpAssetEmitter;
        ParticleSystem::ParticleNode    mParticleNodeHead;

    public:
        EmitterNode( ParticlePlayer* pParticlePlayer, ParticleAssetEmitter* pParticleAssetEmitter )
        {
            // Sanity!
            AssertFatal( pParticlePlayer != NULL, "EmitterNode() - Cannot have a NULL owner." );
            AssertFatal( pParticleAssetEmitter != NULL, "EmitterNode() - Cannot have a NULL particle asset emitter." );

            // Set owner.
            mOwner = pParticlePlayer;

            // Set asset emitter.
            mpAssetEmitter = pParticleAssetEmitter;

            // Reset the node head.
            mParticleNodeHead.mNextNode = mParticleNodeHead.mPreviousNode = &mParticleNodeHead;
        }

        ~EmitterNode()
        {
            freeAllParticles();
        }

        ParticleSystem::ParticleNode* createParticle( void );
        void freeParticle( ParticleSystem::ParticleNode* pParticleNode );
        void freeAllParticles( void );        
    };

    typedef Vector<EmitterNode*> typeEmitterVector;

    AssetPtr<ParticleAsset>     mParticleAsset;
    typeEmitterVector           mEmitters;

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
    virtual void copyTo(SimObject* object);
    virtual void safeDelete( void );

    virtual void OnRegisterScene( Scene* pScene );
    virtual void OnUnregisterScene( Scene* pScene );

    void integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    void interpolateObject( const F32 timeDelta );

    virtual bool canRender( void ) const { return mEffectPlaying && mParticleAsset.notNull() && mParticleAsset->isAssetValid(); }
    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );
    virtual void sceneRenderOverlay( const SceneRenderState* sceneRenderState );

    void setParticle( const char* pAssetId );
    const char* getParticle( void ) const { return mParticleAsset->getAssetId(); }

    inline void setEffectPaused( bool effectPaused ) { mEffectPaused = effectPaused; }
    inline bool getEffectPaused( void ) const { return mEffectPaused; }
    inline void setCameraIdleDistance( const F32 idleDistance ) { mCameraIdleDistance = idleDistance; mCameraIdle = false; }
    inline F32 getCameraIdleDistance( void ) const { return mCameraIdleDistance; }

    bool playEffect( bool resetParticles );
    void stopEffect( bool waitForParticles, bool killEffect );
    bool getIsEffectPlaying() { return mEffectPlaying; };
    bool moveEffectTo( const F32 moveTime, const F32 timeStep, U32& peakCount, F32& peakTime );
    bool findParticlePeak( const F32 searchTime, const F32 timeStep, const U32 peakLimit, U32& peakCount, F32& peakTime );

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticlePlayer);

protected:
    /// Particle Creation/Integration.
    void configureParticle( ParticleSystem::ParticleNode* pParticleNode );
    void integrateParticle( ParticleSystem::ParticleNode* pParticleNode, const F32 particleAge, const F32 elapsedTime );

    /// Persistence.
    virtual void onTamlAddParent( SimObject* pParentObject );

    static bool     setParticle(void* obj, const char* data)                               { static_cast<ParticlePlayer*>(obj)->setParticle(data); return false; };


private:
    virtual void onAssetRefreshed( AssetPtrBase* pAssetPtrBase );

    void initializeParticleAsset( void );
    void destroyParticleAsset( void );
};

#endif // _PARTICLE_PLAYER_H_