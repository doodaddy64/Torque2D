//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

#ifndef _ANIMATION_CONTROLLER_H_
#include "2d/assets/AnimationController.h"
#endif

//-----------------------------------------------------------------------------

class ParticleSystem
{
public:
    /// Particle node.
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
        Vector2                 mRenderOOBB[4];
        U32                     mImageFrame;
        b2Transform             mRotationTransform;
        AnimationController     mAnimationController;

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
        ColorF                  mColor;    

        /// Interpolated Tick Position.
        Vector2                 mPreTickPosition;
        Vector2                 mPostTickPosition;
        Vector2                 mRenderTickPosition;
    };

private:
    const U32               mParticlePoolBlockSize;
    Vector<ParticleNode*>   mParticlePool;
    ParticleNode*           mpFreeParticleNodes;
    U32                     mActiveParticleCount;

public:
    static void Init( void );
    static void destroy( void );
    static ParticleSystem* Instance;

    ParticleSystem();
    ~ParticleSystem();

    ParticleNode* createParticle( void );
    void freeParticle( ParticleNode* pParticleNode );

    inline U32 getActiveParticleCount( void ) const { return mActiveParticleCount; };
    inline U32 getAllocatedParticleCount( void ) const { return mParticlePool.size() * mParticlePoolBlockSize; }
};

#endif // _PARTICLE_SYSTEM_H_