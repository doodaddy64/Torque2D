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

    bool                        mCameraIdle;
    F32                         mCameraIdleDistance;

    bool						mParticleInterpolation;

    bool                        mPlaying;
    bool                        mPaused;
    F32                         mAge;

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

    virtual bool canRender( void ) const { return mPlaying && mParticleAsset.notNull() && mParticleAsset->isAssetValid(); }
    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );
    virtual void sceneRenderOverlay( const SceneRenderState* sceneRenderState );

    void setParticle( const char* pAssetId );
    const char* getParticle( void ) const { return mParticleAsset->getAssetId(); }

    inline void setCameraIdleDistance( const F32 idleDistance ) { mCameraIdleDistance = idleDistance; mCameraIdle = false; }
    inline F32 getCameraIdleDistance( void ) const { return mCameraIdleDistance; }

    bool play( const bool resetParticles );
    void stop( const bool waitForParticles, const bool killEffect );
    inline bool getIsPlaying( void ) const { return mPlaying; };
    inline void setPaused( const bool paused ) { mPaused = paused; }
    inline bool getPaused( void ) const { return mPaused; }

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

    static bool     setParticle(void* obj, const char* data)                                { PREFAB_WRITE_CHECK(ParticlePlayer); pCastObject->setParticle(data); return false; };
    static bool     setCameraIdleDistance(void* obj, const char* data)                      { static_cast<ParticlePlayer*>(obj)->setCameraIdleDistance(dAtof(data)); return false; };
    static bool     writeCameraIdleDistance( void* obj, StringTableEntry pFieldName )       { PREFAB_WRITE_CHECK(ParticlePlayer); return pCastObject->getCameraIdleDistance() > 0.0f; }


private:
    virtual void onAssetRefreshed( AssetPtrBase* pAssetPtrBase );

    void initializeParticleAsset( void );
    void destroyParticleAsset( void );
};

#endif // _PARTICLE_PLAYER_H_