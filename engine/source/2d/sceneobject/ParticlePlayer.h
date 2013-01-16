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

#ifndef _ANIMATION_CONTROLLER_H_
#include "2d/assets/AnimationController.h"
#endif

//-----------------------------------------------------------------------------

class ParticlePlayer : public SceneObject
{
private:
    typedef SceneObject Parent;

public:
    /// Particle Node.
    struct ParticleNode
    {
        /// Particle Node Linkages.
        ParticleNode*           mPreviousNode;
        ParticleNode*           mNextNode;

        /// Suppress Movement.
        bool                    mSuppressMovement;

        /// Particle Components.
        F32                     mParticleLifetime;
        F32                     mParticleAge;
        Vector2                 mPosition;
        Vector2                 mVelocity;
        F32                     mOrientationAngle;
        Vector2                 mOOBB[4];
        b2Transform             mRotationTransform;
        AnimationController*    mAnimationController;

        /// Render Properties.
        Vector2                 mLastRenderSize;
        Vector2                 mRenderSize;
        F32                     mRenderSpeed;
        F32                     mRenderSpin;
        F32                     mRenderFixedForce;
        F32                     mRenderRandomMotion;

        /// Base Properties.
        Vector2                 mSize;
        F32                     mSpeed;
        F32                     mSpin;
        F32                     mFixedForce;
        F32                     mRandomMotion;
        ColorF                  mColour;    

        /// Interpolated Tick Position.
        Vector2                 mPreTickPosition;
        Vector2                 mPostTickPosition;
        Vector2                 mRenderTickPosition;
    };

private:
    /// Particle Data.
    const U32                   mParticlePoolBlockSize;
    static Vector<ParticleNode*> mParticlePool;
    static ParticleNode*        mpFreeParticleNodes;
    static ParticleNode         mParticleNodeHead;
    static U32                  mActiveParticles;

    AssetPtr<ParticleAsset>     mParticleAsset;

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
    virtual void safeDelete( void );

    virtual void OnRegisterScene( Scene* pScene );
    virtual void OnUnregisterScene( Scene* pScene );

    void integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    void interpolateObject( const F32 timeDelta );

    virtual bool canRender( void ) const { return mEffectPlaying && mParticleAsset.notNull() && mParticleAsset->isAssetValid(); }
    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );
    virtual void sceneRenderOverlay( const SceneRenderState* sceneRenderState );

    bool playEffect( bool resetParticles );
    void stopEffect( bool waitForParticles, bool killEffect );
    bool getIsEffectPlaying() { return mEffectPlaying; };
    bool moveEffectTo( const F32 moveTime, const F32 timeStep, U32& peakCount, F32& peakTime );
    bool findParticlePeak( const F32 searchTime, const F32 timeStep, const U32 peakLimit, U32& peakCount, F32& peakTime );
    inline void setEffectPaused( bool effectPaused ) { mEffectPaused = effectPaused; }
    inline bool getEffectPaused( void ) const { return mEffectPaused; }
    inline void setCameraIdleDistance( const F32 idleDistance ) { mCameraIdleDistance = idleDistance; mCameraIdle = false; }
    inline F32 getCameraIdleDistance( void ) const { return mCameraIdleDistance; }

    /// Get particle metrics.
    inline U32 getActiveParticles( void ) const { return mActiveParticles; };
    inline U32 getAllocatedParticles( void ) const { return mParticlePool.size() * mParticlePoolBlockSize; }

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticlePlayer);

protected:
    /// Particle generation.
    ParticleNode* createParticle( void );
    void freeParticle( ParticleNode* pParticleNode );
    void freeAllParticles( void );
    void createParticlePoolBlock( void );
    void destroyParticlePool( void );

    /// Particle Creation/Integration.
    void configureParticle( ParticleNode* pParticleNode );
    void integrateParticle( ParticleNode* pParticleNode, const F32 particleAge, const F32 elapsedTime );

    /// Persistence.
    virtual void onTamlAddParent( SimObject* pParentObject );

};

#endif // _PARTICLE_PLAYER_H_