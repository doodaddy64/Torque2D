//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/sceneobject/particlePlayer.h"

// Script bindings.
#include "2d/sceneobject/particlePlayer_ScriptBinding.h"


//------------------------------------------------------------------------------

ParticleSystem::ParticleNode* ParticlePlayer::EmitterNode::createParticle( void )
{
    // Sanity!
    AssertFatal( mOwner != NULL, "ParticlePlayer::EmitterNode::createParticle() - Cannot create a particle with a NULL owner." );
  
    // Fetch a free node,
    ParticleSystem::ParticleNode* pFreeParticleNode = ParticleSystem::Instance->createParticle();

    // Insert node into emitter chain.
    pFreeParticleNode->mNextNode        = mParticleNodeHead.mNextNode;
    pFreeParticleNode->mPreviousNode    = &mParticleNodeHead;
    mParticleNodeHead.mNextNode         = pFreeParticleNode;
    pFreeParticleNode->mNextNode->mPreviousNode = pFreeParticleNode;

    // Configure the node.
    mOwner->configureParticle( this, pFreeParticleNode );

    return pFreeParticleNode;
}

//------------------------------------------------------------------------------

void ParticlePlayer::EmitterNode::freeParticle( ParticleSystem::ParticleNode* pParticleNode )
{
    // Sanity!
    AssertFatal( mOwner != NULL, "ParticlePlayer::EmitterNode::freeParticle() - Cannot free a particle with a NULL owner." );

    // Remove the node from the emitter chain.
    pParticleNode->mPreviousNode->mNextNode = pParticleNode->mNextNode;
    pParticleNode->mNextNode->mPreviousNode = pParticleNode->mPreviousNode;
   
    // Free the node.
    ParticleSystem::Instance->freeParticle( pParticleNode );
}

//------------------------------------------------------------------------------

void ParticlePlayer::EmitterNode::freeAllParticles( void )
{
    // Sanity!
    AssertFatal( mOwner != NULL, "ParticlePlayer::EmitterNode::freeAllParticles() - Cannot free all particles with a NULL owner." );

    // Free all the nodes,
    while( mParticleNodeHead.mNextNode != &mParticleNodeHead )
    {
        freeParticle( mParticleNodeHead.mNextNode );
    }
}

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticlePlayer);

//------------------------------------------------------------------------------

ParticlePlayer::ParticlePlayer() :
                    mPlaying( false ),
                    mPaused( false ),
                    mAge( 0.0f ),
                    mParticleInterpolation( false ),
                    mCameraIdleDistance( 0.0f ),
                    mCameraIdle( false ),
                    mWaitingForParticles( false ),
                    mWaitingForDelete( false )
{
    // Fetch the particle engine quantity scale.
    mParticleEngineQuantityScale = Con::getFloatVariable( "$pref::T2D::particleEngineQuantityScale", 1.0f );

    // Register for refresh notifications.
    mParticleAsset.registerRefreshNotify( this );
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

    addProtectedField( "Particle", TypeParticleAssetPtr, Offset(mParticleAsset, ParticlePlayer), &setParticle, &defaultProtectedGetFn, defaultProtectedWriteFn, "" );
    addProtectedField( "CameraIdleDistance", TypeF32, Offset(mCameraIdleDistance, ParticlePlayer),&defaultProtectedSetFn, &defaultProtectedGetFn, &writeCameraIdleDistance,"" );
    addProtectedField( "ParticleInterpolation", TypeBool, Offset(mParticleInterpolation, ParticlePlayer), &defaultProtectedSetFn, &defaultProtectedGetFn, &writeParticleInterpolation,"" );
}

//------------------------------------------------------------------------------

void ParticlePlayer::copyTo(SimObject* object)
{
    // Fetch particle asset object.
   ParticlePlayer* pParticlePlayer = static_cast<ParticlePlayer*>( object );

   // Sanity!
   AssertFatal( pParticlePlayer != NULL, "ParticlePlayer::copyTo() - Object is not the correct type.");

   // Copy parent.
   Parent::copyTo( object );

   // Copy the fields.
   pParticlePlayer->setParticle( getParticle() );
   pParticlePlayer->setCameraIdleDistance( getCameraIdleDistance() );
   pParticlePlayer->setParticleInterpolation( getParticleInterpolation() );
}

//------------------------------------------------------------------------------

void ParticlePlayer::onAssetRefreshed( AssetPtrBase* pAssetPtrBase )
{
    // Initialize the particle asset.
    initializeParticleAsset();
}

//-----------------------------------------------------------------------------

void ParticlePlayer::safeDelete( void )
{
    // Finish if we already waiting for delete.
    if ( mWaitingForDelete )
        return;

    // Is the player plating?
    if ( mPlaying )
    {
        // Yes, so stop playing and allow it to kill itself.
        stop(true, true);
        return;
    }

    // Call parent which will deal with the deletion.
    Parent::safeDelete();
}

//-----------------------------------------------------------------------------

void ParticlePlayer::OnRegisterScene( Scene* pScene )
{
    // Call parent.
    Parent::OnRegisterScene( pScene );

    // Add always in scope.
    pScene->getWorldQuery()->addAlwaysInScope( this );
}

//-----------------------------------------------------------------------------

void ParticlePlayer::OnUnregisterScene( Scene* pScene )
{
    // Remove always in scope.
    pScene->getWorldQuery()->removeAlwaysInScope( this );

    // Call parent.
    Parent::OnUnregisterScene( pScene );
}

//------------------------------------------------------------------------------

void ParticlePlayer::integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats )
{
    // Finish if no need to integrate.
    if (    !mPlaying ||
            mCameraIdle ||
            mPaused ||
            mEmitters.size() == 0 )
        return;

    // Yes, so update the particle player age.
    mAge += elapsedTime;

    // Fetch particle asset.
    ParticleAsset* pParticleAsset = mParticleAsset;

    // Finish if no particle asset assigned.
    if ( pParticleAsset == NULL )
        return;

    // Iterate the emitters.
    for( typeEmitterVector::iterator emitterItr = mEmitters.begin(); emitterItr != mEmitters.end(); ++emitterItr )
    {
        // Fetch the emitter node.
        EmitterNode* pEmitterNode = *emitterItr;

        // Fetch the asset emitter.
        ParticleAssetEmitter* pParticleAssetEmitter = pEmitterNode->getAssetEmitter();

        // Skip if the emitter is not visible.
        if ( !pEmitterNode->getVisible() )
            continue;

        // Fetch the first particle node.
        ParticleSystem::ParticleNode* pParticleNode = pEmitterNode->getFirstParticle();

        // Fetch the particle node head.
        ParticleSystem::ParticleNode* pParticleNodeHead = pEmitterNode->getParticleNodeHead();

        // Process All particle nodes.
        while ( pParticleNode != pParticleNodeHead )
        {
            // Update the particle age.
            pParticleNode->mParticleAge += elapsedTime;

            // Has the particle expired?
            // NOTE:-   If we're in single-particle mode then the particle lives as long as the particle player does.
            if (    ( !pParticleAssetEmitter->getSingleParticle() && pParticleNode->mParticleAge > pParticleNode->mParticleLifetime ) ||
                    ( mIsZero(pParticleNode->mParticleLifetime) ) )
            {
                // Yes, so fetch next particle before we kill it.
                pParticleNode = pParticleNode->mNextNode;

                // Kill the particle.
                // NOTE:-   Because we move to the next particle the particle to kill is now the previous!
                pEmitterNode->freeParticle( pParticleNode->mPreviousNode );
            }
            else
            {
                // No, so integrate the particle.
                integrateParticle( pEmitterNode, pParticleNode, pParticleNode->mParticleAge / pParticleNode->mParticleLifetime, elapsedTime );

                // Move to the next particle node.
                pParticleNode = pParticleNode->mNextNode;
            }
        };

        // Skip generating new particles if the emitter is paused.
        if ( pEmitterNode->getPaused() )
            continue;

        // Are we in single-particle mode?
        if ( pParticleAssetEmitter->getSingleParticle() )
        {
            // Yes, so do we have a single particle yet?
            if ( pParticleNodeHead->mNextNode == pParticleNodeHead )
            {
                // No, so generate a single particle.
                pEmitterNode->createParticle();
            }
        }
        else
        {
            // Accumulate the last generation time as we need to handle very small time-integrations correctly.
            //
            // NOTE:    We need to do this if there's an emission target but the time-integration is so small
            //          that rounding results in no emission.  Downside to good FPS!
            pEmitterNode->setTimeSinceLastGeneration( pEmitterNode->getTimeSinceLastGeneration() + elapsedTime );

            // Fetch the particle player age.
            const F32 particlePlayerAge = mAge;

            // Fetch the quantity base and variation fields.
            const ParticleAssetField& quantityBaseField = pParticleAssetEmitter->getQuantityBaseField();
            const ParticleAssetField& quantityVaritationField = pParticleAssetEmitter->getQuantityBaseField();

            // Fetch the emissions.
            const F32 baseEmission = quantityBaseField.getFieldValue( particlePlayerAge );
            const F32 varEmission = quantityVaritationField.getFieldValue( particlePlayerAge ) * 0.5f;

            // Fetch the emission scale.
            const F32 effectEmission = pParticleAsset->getQuantityScaleField().getFieldValue( particlePlayerAge ) * mParticleEngineQuantityScale;

            // Calculate the local emission.
            const F32 localEmission = mClampF(  (baseEmission + CoreMath::mGetRandomF(-varEmission, varEmission)) * effectEmission,
                                                quantityBaseField.getMinValue(),
                                                quantityBaseField.getMaxValue() );

            // Calculate the final time-independent emission count.
            const U32 emissionCount = U32(mFloor( localEmission * pEmitterNode->getTimeSinceLastGeneration() ));

            // Do we have an emission?
            if ( emissionCount > 0 )
            {
                // Yes, so remove this emission from accumulated time.
                pEmitterNode->setTimeSinceLastGeneration( getMax(0.0f, pEmitterNode->getTimeSinceLastGeneration() - (emissionCount / localEmission) ) );

                // Suppress Precision Errors.
                if ( mIsZero( pEmitterNode->getTimeSinceLastGeneration() ) )
                    pEmitterNode->setTimeSinceLastGeneration( 0.0f );

                // Generate the required emission.
                for ( U32 n = 0; n < emissionCount; n++ )
                    pEmitterNode->createParticle();
            }
            // No, so was there a calculated emission?
            else if ( localEmission == 0 )
            {
                // No, so reset accumulated time.
                //mTimeSinceLastGeneration = 0.0f;
            }
        }
    }
}

//------------------------------------------------------------------------------

void ParticlePlayer::interpolateObject( const F32 timeDelta )
{    
    // Finish if no need to interpolate.
    if ( !mParticleInterpolation || !mPlaying || mCameraIdle || mPaused )
        return;

    // Iterate the emitters.
    for( typeEmitterVector::iterator emitterItr = mEmitters.begin(); emitterItr != mEmitters.end(); ++emitterItr )
    {
        // Fetch the emitter node.
        EmitterNode* pEmitterNode = *emitterItr;

        // Fetch First Particle Node.
        ParticleSystem::ParticleNode* pParticleNode = pEmitterNode->getFirstParticle();

        // Fetch the particle node head.
        ParticleSystem::ParticleNode* pParticleNodeHead = pEmitterNode->getParticleNodeHead();

        // Fetch the asset emitter.
        ParticleAssetEmitter* pParticleAssetEmitter = pEmitterNode->getAssetEmitter();

        // Fetch the local AABB..
        const Vector2& localAABB0 = pParticleAssetEmitter->getLocalPivotAABB0();
        const Vector2& localAABB1 = pParticleAssetEmitter->getLocalPivotAABB1();
        const Vector2& localAABB2 = pParticleAssetEmitter->getLocalPivotAABB2();
        const Vector2& localAABB3 = pParticleAssetEmitter->getLocalPivotAABB3();

        // Process All particle nodes.
        while ( pParticleNode != pParticleNodeHead )
        {
            // Interpolate the position.
            pParticleNode->mRenderTickPosition = (timeDelta * pParticleNode->mPreTickPosition) + ((1.0f-timeDelta) * pParticleNode->mPostTickPosition);

            // Set the transform.
            pParticleNode->mRotationTransform.p = pParticleNode->mRenderTickPosition;

            // Fetch the render size.
            const Vector2& renderSize = pParticleNode->mRenderSize;

            // Calculate the scaled AABB.
            Vector2 scaledAABB[4];
            scaledAABB[0] = localAABB0 * renderSize;
            scaledAABB[1] = localAABB1 * renderSize;
            scaledAABB[2] = localAABB2 * renderSize;
            scaledAABB[3] = localAABB3 * renderSize;

            // Calculate the world OOBB..
            CoreMath::mCalculateOOBB( scaledAABB, pParticleNode->mRotationTransform, pParticleNode->mRenderOOBB );

            // Move to the next particle.
            pParticleNode = pParticleNode->mNextNode;
        }
    }
}

//-----------------------------------------------------------------------------

void ParticlePlayer::sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer )
{
    // Finish if we can't render.
    if ( !mPlaying || mCameraIdle )
        return;

    // Fetch emitter count.
    const U32 emitterCount = mEmitters.size();

    // Render all the emitters.
    for ( U32 emitterIndex = 0; emitterIndex < emitterCount; ++emitterIndex )
    {
        // Fetch the emitter node.
        EmitterNode* pEmitterNode = mEmitters[emitterIndex];

        // Fetch the particle emitter.
        ParticleAssetEmitter* pParticleAssetEmitter = pEmitterNode->getAssetEmitter();

        // Skip if the emitter is not visible.
        if ( !pEmitterNode->getVisible() )
            continue;

        // Skip if there are no active particles.
        if ( !pEmitterNode->getActiveParticles() )
            continue;       

        // Fetch both image and animation assets.
        const AssetPtr<ImageAsset>& imageAsset = pParticleAssetEmitter->getImageAsset();
        const AssetPtr<AnimationAsset>& animationAsset = pParticleAssetEmitter->getAnimationAsset();

        // Fetch static mode.
        const bool isStaticMode = pParticleAssetEmitter->isStaticMode();

        // Are we in static mode?
        if ( isStaticMode )
        {
            // Yes, so skip if no image available.
            if ( imageAsset.isNull() )
                continue;
        }
        else
        {
            // No, so skip if no animation available.
            if ( animationAsset.isNull() )
                continue;
        }

        // Flush.
        pBatchRenderer->flush( getScene()->getDebugStats().batchIsolatedFlush );

        // Intense particles?
        if ( pParticleAssetEmitter->getIntenseParticles() )
        {
            // Yes, so set additive blending.
            pBatchRenderer->setBlendMode( GL_SRC_ALPHA, GL_ONE );
        }
        else
        {
            // No, so set standard blend options.
            if ( mBlendMode )
            {
                pBatchRenderer->setBlendMode( mSrcBlendFactor, mDstBlendFactor );
            }
            else
            {
                pBatchRenderer->setBlendOff();
            }
        }

        // Set alpha-testing.
        pBatchRenderer->setAlphaTestMode( pParticleAssetEmitter->getAlphaTest() );

        // Is the Position attached to the emitter?
        if ( pParticleAssetEmitter->getAttachPositionToEmitter() )
        {
            // Yes, so get player position.
            const Vector2 renderPosition = getRenderPosition();

            // Move into emitter-space.
            glTranslatef( renderPosition.x, renderPosition.y, 0.0f );

            // Is the rotation attached to the emitter?
            if ( pParticleAssetEmitter->getAttachRotationToEmitter() )
            {
                // Yes, so rotate into emitter-space.
                // NOTE:- We need clockwise rotation here.
                glRotatef( getRenderAngle(), 0.0f, 0.0f, 1.0f );
            }
        }

        // Frame texture.
        TextureHandle frameTexture;

        // Frame area.
        ImageAsset::FrameArea::TexelArea texelFrameArea;

        // Are we in static mode?
        if ( isStaticMode )
        {
            // Yes, so fetch the frame texture.
            frameTexture = imageAsset->getImageTexture();

            // Are we using a random image frame?
            if ( !pParticleAssetEmitter->getRandomImageFrame() )
            {
                // No, so fetch frame area.
                texelFrameArea = imageAsset->getImageFrameArea( pParticleAssetEmitter->getImageFrame() ).mTexelArea;
            }
        }

        // Fetch ordered particles flag.
        const bool orderedParticles = pParticleAssetEmitter->getOrderedParticles();

        // Fetch the oldest-in-front flag.
        const bool oldestInFront = pParticleAssetEmitter->getOldestInFront();

        // Fetch the starting particle (using appropriate particle order).
        ParticleSystem::ParticleNode* pParticleNode = orderedParticles || oldestInFront ? pEmitterNode->getFirstParticle() : pEmitterNode->getLastParticle();

        // Fetch the particle node head.
        ParticleSystem::ParticleNode* pParticleNodeHead = pEmitterNode->getParticleNodeHead();

        // Process All particle nodes.
        while ( pParticleNode != pParticleNodeHead )
        {
            // Are we in static mode are using a random image frame?
            if ( isStaticMode && pParticleAssetEmitter->getRandomImageFrame() )
            {
                // Yes, so fetch frame area.
                texelFrameArea = imageAsset->getImageFrameArea( pParticleNode->mImageFrame ).mTexelArea;
            }

            // Are we using an animation?
            if ( !isStaticMode )
            {
                // Yes, so fetch current frame area.
                texelFrameArea = pParticleNode->mAnimationController.getCurrentImageFrameArea().mTexelArea;
                frameTexture = pParticleNode->mAnimationController.getImageTexture();
            }

            // Fetch the particle render OOBB.
            Vector2* renderOOBB = pParticleNode->mRenderOOBB;

            // Fetch lower/upper texture coordinates.
            const Vector2& texLower = texelFrameArea.mTexelLower;
            const Vector2& texUpper = texelFrameArea.mTexelUpper;

            // Submit batched quad.
            pBatchRenderer->SubmitQuad(
                renderOOBB[0],
                renderOOBB[1],
                renderOOBB[2],
                renderOOBB[3],
                Vector2( texLower.x, texUpper.y ),
                Vector2( texUpper.x, texUpper.y ),
                Vector2( texUpper.x, texLower.y ),
                Vector2( texLower.x, texLower.y ),
                frameTexture,
                pParticleNode->mColor );

            // Move to next Particle ( using appropriate sort-order ).
            pParticleNode = orderedParticles || oldestInFront ? pParticleNode->mNextNode : pParticleNode->mPreviousNode;
        };
    }
}

//-----------------------------------------------------------------------------

void ParticlePlayer::sceneRenderOverlay( const SceneRenderState* sceneRenderState )
{
    // Call parent.
    Parent::sceneRenderOverlay( sceneRenderState );

    // Get Scene.
    Scene* pScene = getScene();

    // Finish if no scene.
    if ( !pScene )
        return;

    // Finish if we shouldn't be drawing the debug overlay.
    if ( !pScene->getIsEditorScene() || mLessThanOrEqual( mCameraIdleDistance, 0.0f ) || !isEnabled() || !getVisible() )
        return;

    // Draw camera pause distance.
    pScene->mDebugDraw.DrawCircle( getRenderPosition(), mCameraIdleDistance, b2Color(1.0f, 1.0f, 0.0f ) );
}

//-----------------------------------------------------------------------------

void ParticlePlayer::setParticle( const char* pAssetId )
{
    // Sanity!
    AssertFatal( pAssetId != NULL, "ParticlePlayer::setParticle() - Cannot use a NULL asset Id." );

    // Set asset Id.
    mParticleAsset = pAssetId;

    // Initialize the particle.
    initializeParticleAsset();
}

//-----------------------------------------------------------------------------

void ParticlePlayer::setEmitterPaused( const bool paused, const U32 emitterIndex )
{
    // Is the emitter index valid?
    if ( emitterIndex >= getEmitterCount() )
    {
        // No, so warn.
        Con::warnf( "ParticlePlayer::setEmitterPaused() - Emitter index is out of bounds." );
        return;
    }

    mEmitters[emitterIndex]->setPaused( paused );
}

//-----------------------------------------------------------------------------

bool ParticlePlayer::getEmitterPaused( const U32 emitterIndex )
{
    // Is the emitter index valid?
    if ( emitterIndex >= getEmitterCount() )
    {
        // No, so warn.
        Con::warnf( "ParticlePlayer::getEmitterPaused() - Emitter index is out of bounds." );
        return false;
    }

    return mEmitters[emitterIndex]->getPaused();
}

//-----------------------------------------------------------------------------

void ParticlePlayer::setEmitterVisible( const bool visible, const U32 emitterIndex )
{
    // Is the emitter index valid?
    if ( emitterIndex >= getEmitterCount() )
    {
        // No, so warn.
        Con::warnf( "ParticlePlayer::setEmitterVisible() - Emitter index is out of bounds." );
        return;
    }

    mEmitters[emitterIndex]->setVisible( visible );
}

//-----------------------------------------------------------------------------

bool ParticlePlayer::getEmitterVisible( const U32 emitterIndex )
{
    // Is the emitter index valid?
    if ( emitterIndex >= getEmitterCount() )
    {
        // No, so warn.
        Con::warnf( "ParticlePlayer::getEmitterVisible() - Emitter index is out of bounds." );
        return false;
    }

    return mEmitters[emitterIndex]->getVisible();
}

//-----------------------------------------------------------------------------

bool ParticlePlayer::play( const bool resetParticles )
{
    // Cannot do anything if we've not got any emitters!
    if ( mParticleAsset.isNull() || mParticleAsset->getEmitterCount() == 0 )
    {
        // Warn.
        Con::warnf("ParticlePlayer::play() - Cannot play; no emitters!");
        return false;
    }

    // Are we in a scene?
    if ( getScene() == NULL )
    {
        // No, so warn.
        Con::warnf("ParticlePlayer::play() - Cannot play when not in a scene!");
        return false;
    }

    // Reset the age.
    mAge = 0.0f;

    // Iterate the emitters.
    for( typeEmitterVector::iterator emitterItr = mEmitters.begin(); emitterItr != mEmitters.end(); ++emitterItr )
    {
        // Fetch the emitter node.
        EmitterNode* pEmitterNode = *emitterItr;

        // Reset the time since last generation.
        pEmitterNode->setTimeSinceLastGeneration( 0.0f );
    }

    // Reset Waiting for Particles.
    mWaitingForParticles = false;

    // Reset Waiting for delete.
    mWaitingForDelete = false;

    // Flag as playing.
    mPlaying = true;

    // Turn-off paused.
    mPaused = false;

    // Set unsafe delete status.
    setSafeDelete(false);

    return true;
}

//-----------------------------------------------------------------------------

void ParticlePlayer::stop( const bool waitForParticles, const bool killEffect )
{
    // Finish if we're not playing and there's no kill command.
    if ( !mPlaying && !killEffect )
        return;

    // Fetch emitter count.
    const U32 emitterCount = mEmitters.size();

    // Are we waiting for particles to end?
    if ( waitForParticles )
    {
        // Yes, so pause all the emitters.
        for ( U32 emitterIndex = 0; emitterIndex < emitterCount; ++emitterIndex )
        {
            // Fetch the emitter.
            mEmitters[emitterIndex]->setPaused( true );
        }

        // Set waiting for particles.
        mWaitingForParticles = true;

        // Flag as waiting for deletion if killing effect.
        if ( killEffect )
            mWaitingForDelete = true;

        return;
    }

    // No, so free all particles.
    for ( U32 emitterIndex = 0; emitterIndex < emitterCount; ++emitterIndex )
    {
        mEmitters[emitterIndex]->freeAllParticles();
    }

    // Reset the age.
    mAge = 0.0f;

    // Flag as stopped and not waiting.
    mPlaying = mWaitingForParticles = mWaitingForDelete = false;

    // Turn off paused.
    mPaused = false;

    // Set safe deletion.
    setSafeDelete(true);

    // Perform the callback.
    if( isMethod( "onStopParticlePlayer" ) )
        Con::executef( this, 1, "onStopParticlePlayer" );

    // Flag for immediate deletion if killing.
    if ( killEffect )
        safeDelete();
}

//------------------------------------------------------------------------------

void ParticlePlayer::configureParticle( EmitterNode* pEmitterNode, ParticleSystem::ParticleNode* pParticleNode )
{
    // Fetch the particle player age.
    const F32 particlePlayerAge = mAge;

    // Fetch the particle player position.
    const Vector2& particlePlayerPosition = getPosition();

    // Default to not suppressing movement.
    pParticleNode->mSuppressMovement = false;

    // Fetch particle asset.
    ParticleAsset* pParticleAsset = mParticleAsset;

    // Fetch the asset emitter.
    ParticleAssetEmitter* pParticleAssetEmitter = pEmitterNode->getAssetEmitter();


    // **********************************************************************************************************************
    // Calculate Particle Position.
    // **********************************************************************************************************************

    // Fetch attachment options.
    const bool attachPositionToEmitter = pParticleAssetEmitter->getAttachPositionToEmitter();
    const bool attachRotationToEmitter = pParticleAssetEmitter->getAttachRotationToEmitter();

    // Are we using Single Particle?
    if ( pParticleAssetEmitter->getSingleParticle() )
    {
        // Yes, so if use Effect Position ( or origin if attached to emitter).
        if ( attachPositionToEmitter )
            pParticleNode->mPosition.Set(0.0f, 0.0f);
        else
            pParticleNode->mPosition = particlePlayerPosition;
    }
    else
    {
        // No, so select Emitter-Type.
        switch( pParticleAssetEmitter->getEmitterType() )
        {
            // Use pivot point.
            case ParticleAssetEmitter::POINT_EMITTER:
            {
                // Yes, so if use Effect Position ( or origin if attached to emitter).
                if ( attachPositionToEmitter )
                    pParticleNode->mPosition.Set(0.0f, 0.0f);
                else
                    pParticleNode->mPosition = particlePlayerPosition;

            } break;

            // Use Size-X and Pivot-Y.
            case ParticleAssetEmitter::LINEX_EMITTER:
            {
                // Fetch the local sized OOBB.
                const b2Vec2* pLocalSizedOOBB = getLocalSizedOOBB();

                // Choose a random position along the line-X within the local-sized OOBB at Pivot-Y.
                F32 minX = pLocalSizedOOBB[0].x;
                F32 maxX = pLocalSizedOOBB[1].x;
                F32 midY = (pLocalSizedOOBB[0].y + pLocalSizedOOBB[3].y) * 0.5f;

                // Normalize the X-Axis.
                if ( minX > maxX )
                    mSwap( minX, maxX );

                // Calculate chosen position along the line.
                Vector2 emissionPosition;
                if ( mIsEqual(minX, maxX) )
                {
                    emissionPosition.Set( minX, midY );
                }
                else
                {
                    emissionPosition.Set( CoreMath::mGetRandomF( minX, maxX ), midY );
                }

                // Rotate into Emitter-Space.
                //
                // NOTE:-   If we're attached to the emitter, then we keep the particle coordinates
                //          in local-space and use the hardware to render them in emitter-space.
                b2Transform xform;
                xform.Set( attachPositionToEmitter ? b2Vec2_zero : particlePlayerPosition, (attachPositionToEmitter && attachRotationToEmitter) ? 0.0f : getAngle() );
                pParticleNode->mPosition = b2Mul( xform, emissionPosition );

            } break;

            // Use Y-Size and Pivot-X.
            case ParticleAssetEmitter::LINEY_EMITTER:
            {
                // Fetch the local sized OOBB.
                const b2Vec2* pLocalSizedOOBB = getLocalSizedOOBB();

                // Choose a random position along the line-Y within the lcoal-sized OOBB @ Pivot-X.
                F32 midX = (pLocalSizedOOBB[0].x + pLocalSizedOOBB[1].x) * 0.5f;
                F32 minY = pLocalSizedOOBB[0].y;
                F32 maxY = pLocalSizedOOBB[3].y;

                // Normalize the Y-Axis.
                if ( minY > maxY )
                    mSwap( minY, maxY );

                // Calculate chosen position along the line.
                Vector2 emissionPosition;
                if ( mIsEqual(minY, maxY) )
                {
                    emissionPosition.Set( midX, minY );
                }
                else
                {
                    emissionPosition.Set( midX, CoreMath::mGetRandomF( minY, maxY ) );
                }

                // Rotate into Emitter-Space.
                //
                // NOTE:-   If we're attached to the emitter, then we keep the particle coordinates
                //          in local-space and use the hardware to render them in emitter-space.
                b2Transform xform;
                xform.Set( attachPositionToEmitter ? b2Vec2_zero : particlePlayerPosition, (attachPositionToEmitter && attachRotationToEmitter) ? 0.0f : getAngle() );
                pParticleNode->mPosition = b2Mul( xform, emissionPosition );

            } break;

            // Use X/Y Sizes.
            case ParticleAssetEmitter::AREA_EMITTER:
            {
                // Fetch the local sized OOBB.
                const b2Vec2* pLocalSizedOOBB = getLocalSizedOOBB();

                // Choose a random position in the area.
                F32 minX = pLocalSizedOOBB[0].x;
                F32 maxX = pLocalSizedOOBB[1].x;
                F32 minY = pLocalSizedOOBB[0].y;
                F32 maxY = pLocalSizedOOBB[3].y;

                // Normalize the X-Axis.
                if ( minX > maxX )
                    mSwap( minX, maxX );

                // Normalize the Y-Axis.
                if ( minY > maxY )
                    mSwap( minY, maxY );

                // Calculate chosen area.
                Vector2 emissionPosition( mIsEqual(minX, maxX) ? minX : CoreMath::mGetRandomF( minX, maxX ), mIsEqual(minY, maxY) ? minY : CoreMath::mGetRandomF( minY, maxY ) );

                // Rotate into Emitter-Space.
                //
                // NOTE:-   If we're attached to the emitter, then we keep the particle coordinates
                //          in local-space and use the hardware to render them in emitter-space.
                b2Transform xform;
                xform.Set( attachPositionToEmitter ? b2Vec2_zero : particlePlayerPosition, (attachPositionToEmitter && attachRotationToEmitter) ? 0.0f : getAngle() );
                pParticleNode->mPosition = b2Mul( xform, emissionPosition );

            } break;
        }
    }

    // **********************************************************************************************************************
    // Calculate Particle Lifetime.
    // **********************************************************************************************************************

    pParticleNode->mParticleAge = 0.0f;
    pParticleNode->mParticleLifetime = ParticleAssetField::calculateFieldBVE(   pParticleAssetEmitter->getParticleLifeBaseField(),
                                                                                pParticleAssetEmitter->getParticleLifeVariationField(),
                                                                                pParticleAsset->getParticleLifeScaleField(),
                                                                                particlePlayerAge );


    // **********************************************************************************************************************
    // Calculate Particle Size-X.
    // **********************************************************************************************************************

    pParticleNode->mSize.x = ParticleAssetField::calculateFieldBVE( pParticleAssetEmitter->getSizeXBaseField(),
                                                                    pParticleAssetEmitter->getSizeXVariationField(),
                                                                    pParticleAsset->getSizeXScaleField(),
                                                                    particlePlayerAge );

    // Is the particle using a fixed aspect?
    if ( pParticleAssetEmitter->getFixedAspect() )
    {
        // Yes, so simply copy Size-X.
        pParticleNode->mSize.y = pParticleNode->mSize.x;
    }
    else
    {
        // No, so calculate the particle Size-Y.
        pParticleNode->mSize.y = ParticleAssetField::calculateFieldBVE( pParticleAssetEmitter->getSizeYBaseField(),
                                                                        pParticleAssetEmitter->getSizeYVariationField(),
                                                                        pParticleAsset->getSizeYScaleField(),
                                                                        particlePlayerAge );
    }

    // Reset the render size.
    pParticleNode->mRenderSize.Set(-1.0f, -1.0f);


    // **********************************************************************************************************************
    // Calculate Speed, Random Motion and Emission Angle.
    // **********************************************************************************************************************

    // We reset the emission angle/arc in-case we're using single-particle mode as this is the default.
    F32 emissionForce = 0;
    F32 emissionAngle = 0;
    F32 emissionArc = 0;

    // Ignore if we're using a single-particle.
    if ( !pParticleAssetEmitter->getSingleParticle() )
    {
        pParticleNode->mSpeed = ParticleAssetField::calculateFieldBVE(  pParticleAssetEmitter->getSpeedBaseField(),
                                                                        pParticleAssetEmitter->getSpeedVariationField(),
                                                                        pParticleAsset->getSpeedScaleField(),
                                                                        particlePlayerAge );

        pParticleNode->mRandomMotion = ParticleAssetField::calculateFieldBVE(   pParticleAssetEmitter->getRandomMotionBaseField(),
                                                                                pParticleAssetEmitter->getRandomMotionVariationField(),
                                                                                pParticleAsset->getRandomMotionScaleField(),
                                                                                particlePlayerAge );


        // Are we using the emitter emission?
        if ( pParticleAssetEmitter->getEmitterEmission() )
        {
            // Yes, so calculate the emission force.
            emissionForce = ParticleAssetField::calculateFieldBV(   pParticleAssetEmitter->getEmissionForceForceBaseField(),
                                                                    pParticleAssetEmitter->getEmissionForceVariationField(),
                                                                    particlePlayerAge);

            // Calculate Emission Angle.
            emissionAngle = ParticleAssetField::calculateFieldBV(   pParticleAssetEmitter->getEmissionAngleBaseField(),
                                                                    pParticleAssetEmitter->getEmissionAngleVariationField(),
                                                                    particlePlayerAge );

            // Calculate Emission Arc.
            // NOTE:-   We're actually interested in half the emission arc!
            emissionArc = ParticleAssetField::calculateFieldBV( pParticleAssetEmitter->getEmissionArcBaseField(),
                                                                pParticleAssetEmitter->getEmissionArcVariationField(),
                                                                particlePlayerAge ) * 0.5f;
        }
        else
        {
            // No, so calculate the emission force.
            emissionForce = ParticleAssetField::calculateFieldBV(   pParticleAsset->getEmissionForceBaseField(),
                                                                    pParticleAsset->getEmissionForceVariationField(),
                                                                    particlePlayerAge);

            // Calculate Emission Angle.
            emissionAngle = ParticleAssetField::calculateFieldBV(   pParticleAsset->getEmissionAngleBaseField(),
                                                                    pParticleAsset->getEmissionAngleVariationField(),
                                                                    particlePlayerAge);

            // Calculate Emission Arc.
            // NOTE:-   We're actually interested in half the emission arc!
            emissionArc = ParticleAssetField::calculateFieldBV(     pParticleAsset->getEmissionArcBaseField(),
                                                                    pParticleAsset->getEmissionAngleVariationField(),
                                                                    particlePlayerAge ) * 0.5f;
        }

        // Is the emission rotation linked?
        if ( pParticleAssetEmitter->getLinkEmissionRotation() )
        {
            // Yes, so add the particle player angle.
            emissionAngle += getAngle();
        }

        // Calculate the final emission angle choosing random Arc.
        emissionAngle = mFmod( CoreMath::mGetRandomF( emissionAngle-emissionArc, emissionAngle+emissionArc ), 360.0f );

        // Calculate the particle velocity.
        const F32 emissionAngleRadians = mDegToRad( emissionAngle );
        pParticleNode->mVelocity.Set( emissionForce * mSin( emissionAngleRadians ), emissionForce * mCos( emissionAngleRadians ) );
    }


    // **********************************************************************************************************************
    // Calculate Spin.
    // **********************************************************************************************************************

    pParticleNode->mSpin = ParticleAssetField::calculateFieldBVE(   pParticleAssetEmitter->getSpinBaseField(),
                                                                    pParticleAssetEmitter->getSpinVariationField(),
                                                                    pParticleAsset->getSpinScaleField(),
                                                                    particlePlayerAge );


    // **********************************************************************************************************************
    // Calculate Fixed-Force.
    // **********************************************************************************************************************

    pParticleNode->mFixedForce = ParticleAssetField::calculateFieldBVE( pParticleAssetEmitter->getFixedForceBaseField(),
                                                                        pParticleAssetEmitter->getFixedForceVariationField(),
                                                                        pParticleAsset->getFixedForceScaleField(),
                                                                        particlePlayerAge );


    // **********************************************************************************************************************
    // Calculate Orientation Angle.
    // **********************************************************************************************************************

    // Configure particle orientation.
    switch( pParticleAssetEmitter->getOrientationType() )
    {
        // Aligned to initial emission.
        case ParticleAssetEmitter::ALIGNED_ORIENTATION:
        {
            // Use the emission angle with fixed offset.
            pParticleNode->mOrientationAngle = mFmod( emissionAngle - pParticleAssetEmitter->getAlignedAngleOffset(), 360.0f );

        } break;

        // Fixed orientation.
        case ParticleAssetEmitter::FIXED_ORIENTATION:
        {
            // Use a fixed angle.
            pParticleNode->mOrientationAngle = mFmod( pParticleAssetEmitter->getFixedAngleOffset(), 360.0f );

        } break;

        // Random with constraints.
        case ParticleAssetEmitter::RANDOM_ORIENTATION:
        {
            // Used a random angle/arc.
            const F32 randomArc = pParticleAssetEmitter->getRandomArc() * 0.5f;
            pParticleNode->mOrientationAngle = mFmod( CoreMath::mGetRandomF( pParticleAssetEmitter->getRandomAngleOffset() - randomArc, pParticleAssetEmitter->getRandomAngleOffset() + randomArc ), 360.0f );

        } break;
    }

    // **********************************************************************************************************************
    // Calculate RGBA Components.
    // **********************************************************************************************************************

    // Fetch the channels.
    const ParticleAssetField& redChannel = pParticleAssetEmitter->getRedChannelLifeField();
    const ParticleAssetField& greenChannel = pParticleAssetEmitter->getGreenChannelLifeField();
    const ParticleAssetField& blueChannel = pParticleAssetEmitter->getBlueChannelLifeField();
    const ParticleAssetField& alphaChannel = pParticleAssetEmitter->getAlphaChannelLifeField();
    const ParticleAssetField& alphaChannelScale = pParticleAsset->getAlphaChannelScaleField();

    // Calculate the color.
    pParticleNode->mColor.set(  mClampF( redChannel.getFieldValue( 0.0f ), redChannel.getMinValue(), redChannel.getMaxValue() ),
                                mClampF( greenChannel.getFieldValue( 0.0f ),greenChannel.getMinValue(), greenChannel.getMaxValue() ),
                                mClampF( blueChannel.getFieldValue( 0.0f ), blueChannel.getMinValue(),blueChannel.getMaxValue() ),
                                mClampF( alphaChannel.getFieldValue( 0.0f ) * alphaChannelScale.getFieldValue( 0.0f ), alphaChannel.getMinValue(), alphaChannel.getMaxValue() ) );


    // **********************************************************************************************************************
    // Image, Frame and Animation Controller.
    // **********************************************************************************************************************

    // Is the emitter in static mode?
    if ( pParticleAssetEmitter->isStaticMode() )
    {
        // Yes, so is random image frame active?
        if ( pParticleAssetEmitter->getRandomImageFrame() )
        {
            // Yes, so fetch the frame count for the image asset.
            const U32 frameCount = pParticleAssetEmitter->getImageAsset()->getFrameCount();

            // Choose a random frame.
            pParticleNode->mImageFrame = (U32)CoreMath::mGetRandomI( 0, frameCount-1 );
        }
        else
        {
            // No, so set the emitter image frame.
            pParticleNode->mImageFrame = pParticleAssetEmitter->getImageFrame();
        }
    }
    else
    {
        // No, so fetch the animation asset.
        const AssetPtr<AnimationAsset>& animationAsset = pParticleAssetEmitter->getAnimationAsset();

        // Is an animation available?
        if ( animationAsset.notNull() )
        {
            // Yes, so play it.
            pParticleNode->mAnimationController.playAnimation( animationAsset.getAssetId(), false );
        }
    }


    // Reset the last render size.
    pParticleNode->mLastRenderSize.Set(-1.0f, -1.0f);


    // **********************************************************************************************************************
    // Reset Tick Position.
    // **********************************************************************************************************************
    pParticleNode->mPreTickPosition = pParticleNode->mPostTickPosition = pParticleNode->mRenderTickPosition = pParticleNode->mPosition;


    // **********************************************************************************************************************
    // Do a Single Particle Integration to get things going.
    // **********************************************************************************************************************
    integrateParticle( pEmitterNode, pParticleNode, 0.0f, 0.0f );
}

//------------------------------------------------------------------------------

void ParticlePlayer::integrateParticle( EmitterNode* pEmitterNode, ParticleSystem::ParticleNode* pParticleNode, F32 particleAge, F32 elapsedTime )
{
    // Fetch particle asset.
    ParticleAsset* pParticleAsset = mParticleAsset;

    // Fetch the asset emitter.
    ParticleAssetEmitter* pParticleAssetEmitter = pEmitterNode->getAssetEmitter();


    // **********************************************************************************************************************
    // Copy Old Tick Position.
    // **********************************************************************************************************************
    pParticleNode->mRenderTickPosition = pParticleNode->mPreTickPosition = pParticleNode->mPostTickPosition;


    // **********************************************************************************************************************
    // Scale Size.
    // **********************************************************************************************************************

    // Scale Size-X.
    pParticleNode->mRenderSize.x = mClampF( pParticleNode->mSize.x * pParticleAssetEmitter->getSizeXLifeField().getFieldValue( particleAge ),
                                            pParticleAssetEmitter->getSizeXBaseField().getMinValue(),
                                            pParticleAssetEmitter->getSizeXBaseField().getMaxValue());

    // Is the particle using a fixed aspect?
    if ( pParticleAssetEmitter->getFixedAspect() )
    {
        // Yes, so simply copy Size-X.
        pParticleNode->mRenderSize.y = pParticleNode->mRenderSize.x;
    }
    else
    {
        // No, so Scale Size-Y.
        pParticleNode->mRenderSize.y = mClampF( pParticleNode->mSize.y * pParticleAssetEmitter->getSizeYLifeField().getFieldValue( particleAge ),
                                                pParticleAssetEmitter->getSizeYBaseField().getMinValue(),
                                                pParticleAssetEmitter->getSizeYBaseField().getMaxValue() );
    }


    // **********************************************************************************************************************
    // Scale Speed.
    // **********************************************************************************************************************
    pParticleNode->mRenderSpeed = mClampF(  pParticleNode->mSpeed * pParticleAssetEmitter->getSpeedLifeField().getFieldValue( particleAge ),
                                            pParticleAssetEmitter->getSpeedBaseField().getMinValue(),
                                            pParticleAssetEmitter->getSpeedBaseField().getMaxValue() );


    // **********************************************************************************************************************
    // Scale Fixed-Force.
    // **********************************************************************************************************************
    pParticleNode->mRenderFixedForce = mClampF( pParticleNode->mFixedForce * pParticleAssetEmitter->getFixedForceLifeField().getFieldValue( particleAge ),
                                                pParticleAssetEmitter->getFixedForceBaseField().getMinValue(),
                                                pParticleAssetEmitter->getFixedForceBaseField().getMaxValue() );


    // **********************************************************************************************************************
    // Scale Random-Motion.
    // **********************************************************************************************************************
    pParticleNode->mRenderRandomMotion = mClampF(   pParticleNode->mRandomMotion * pParticleAssetEmitter->getRandomMotionLifeField().getFieldValue( particleAge ),
                                                    pParticleAssetEmitter->getRandomMotionBaseField().getMinValue(),
                                                    pParticleAssetEmitter->getRandomMotionBaseField().getMaxValue() );


    // **********************************************************************************************************************
    // Calculate RGBA Components.
    // **********************************************************************************************************************

    // Fetch the channels.
    const ParticleAssetField& redChannel = pParticleAssetEmitter->getRedChannelLifeField();
    const ParticleAssetField& greenChannel = pParticleAssetEmitter->getGreenChannelLifeField();
    const ParticleAssetField& blueChannel = pParticleAssetEmitter->getBlueChannelLifeField();
    const ParticleAssetField& alphaChannel = pParticleAssetEmitter->getAlphaChannelLifeField();
    const ParticleAssetField& alphaChannelScale = pParticleAsset->getAlphaChannelScaleField();

    // Calculate the color.
    pParticleNode->mColor.set(  mClampF( redChannel.getFieldValue( particleAge ), redChannel.getMinValue(), redChannel.getMaxValue() ),
                                mClampF( greenChannel.getFieldValue( particleAge ),greenChannel.getMinValue(), greenChannel.getMaxValue() ),
                                mClampF( blueChannel.getFieldValue( particleAge ), blueChannel.getMinValue(),blueChannel.getMaxValue() ),
                                mClampF( alphaChannel.getFieldValue( particleAge ) * alphaChannelScale.getFieldValue( 0.0f ), alphaChannel.getMinValue(), alphaChannel.getMaxValue() ) );


    // **********************************************************************************************************************
    // Integrate Particle.
    // **********************************************************************************************************************


    // Is the emitter in static mode?
    if ( !pParticleAssetEmitter->isStaticMode() )
    {
        // No, so update animation.
        pParticleNode->mAnimationController.updateAnimation( elapsedTime );
    }


    // **********************************************************************************************************************
    // Calculate New Velocity...
    // **********************************************************************************************************************

    // Calculate the velocity if not a single particle.
    if ( !pParticleAssetEmitter->getSingleParticle() )
    {
        // Calculate random motion (if we've got any).
        if ( mNotZero( pParticleNode->mRenderRandomMotion ) )
        {
            // Fetch random motion.
            const F32 randomMotion = pParticleNode->mRenderRandomMotion * 0.5f;

            // Add time-integrated random motion into velocity.
            pParticleNode->mVelocity += elapsedTime * Vector2( CoreMath::mGetRandomF(-randomMotion, randomMotion), CoreMath::mGetRandomF(-randomMotion, randomMotion) );
        }

        // Do we have any fixed force?
        if ( mNotZero( pParticleNode->mRenderFixedForce ) )
        {
            // Yes, so time-integrate a fixed force to the velocity.
            pParticleNode->mVelocity += (pParticleAssetEmitter->getFixedForceDirection() * pParticleNode->mRenderFixedForce * elapsedTime);
        }

        // Are we suppressing movement?
        if ( !pParticleNode->mSuppressMovement )
        {
            // No, so adjust particle position.
            pParticleNode->mPosition += (pParticleNode->mVelocity * pParticleNode->mRenderSpeed * elapsedTime);
        }
    }


    // **********************************************************************************************************************
    // Are we Aligning to motion?
    // **********************************************************************************************************************
    if ( pParticleAssetEmitter->getKeepAligned() && pParticleAssetEmitter->getOrientationType() == ParticleAssetEmitter::ALIGNED_ORIENTATION )
    {
        // Yes, so calculate last movement direction.
        F32 movementAngle = mRadToDeg( mAtan( pParticleNode->mVelocity.x, -pParticleNode->mVelocity.y ) );

        // Adjust for negative ArcTan quadrants.
        if ( movementAngle < 0.0f )
            movementAngle += 360.0f;

        // Set new Orientation Angle.
        pParticleNode->mOrientationAngle = -movementAngle - pParticleAssetEmitter->getAlignedAngleOffset();


        // Calculate Rotation Matrix.
        pParticleNode->mRotationTransform.Set( pParticleNode->mPosition, mDegToRad(pParticleNode->mOrientationAngle) );
    }
    else
    {
        // No, so calculate the render spin.
        pParticleNode->mRenderSpin = pParticleNode->mSpin * pParticleAssetEmitter->getSpinLifeField().getFieldValue( particleAge );

        // Have we got some Spin?
        if ( mNotZero(pParticleNode->mRenderSpin) )
        {
            // Yes, so add into Orientation.
            pParticleNode->mOrientationAngle += pParticleNode->mRenderSpin * elapsedTime;

            // Clamp the orientation angle.
            pParticleNode->mOrientationAngle = mFmod( pParticleNode->mOrientationAngle, 360.0f );
        }

        // If the size has changed or we have some spin then we need to recalculate the OOBB.
        if ( mNotZero(pParticleNode->mRenderSpin) || pParticleNode->mRenderSize != pParticleNode->mLastRenderSize )
        {
            // Calculate the rotation transform.
            pParticleNode->mRotationTransform.Set( pParticleNode->mPosition, mDegToRad(pParticleNode->mOrientationAngle) );
        }

        // We've dealt with a potential Size change so store current size for next time.
        pParticleNode->mLastRenderSize = pParticleNode->mRenderSize;
    }

    // Fetch the local AABB..
    const Vector2& localAABB0 = pParticleAssetEmitter->getLocalPivotAABB0();
    const Vector2& localAABB1 = pParticleAssetEmitter->getLocalPivotAABB1();
    const Vector2& localAABB2 = pParticleAssetEmitter->getLocalPivotAABB2();
    const Vector2& localAABB3 = pParticleAssetEmitter->getLocalPivotAABB3();

    // Fetch the render size.
    const Vector2& renderSize = pParticleNode->mRenderSize;

    // Calculate the scaled AABB.
    Vector2 scaledAABB[4];
    scaledAABB[0] = localAABB0 * renderSize;
    scaledAABB[1] = localAABB1 * renderSize;
    scaledAABB[2] = localAABB2 * renderSize;
    scaledAABB[3] = localAABB3 * renderSize;

    // Calculate the world OOBB..
    CoreMath::mCalculateOOBB( scaledAABB, pParticleNode->mRotationTransform, pParticleNode->mRenderOOBB );


    // **********************************************************************************************************************
    // Set Post Tick Position.
    // **********************************************************************************************************************
    pParticleNode->mPostTickPosition = pParticleNode->mPosition;
}

//-----------------------------------------------------------------------------

void ParticlePlayer::onTamlAddParent( SimObject* pParentObject )
{
    // Call parent.
    Parent::onTamlAddParent( pParentObject );

    // Play  automatically when added to a parent.
    play( true );
}

//-----------------------------------------------------------------------------

void ParticlePlayer::initializeParticleAsset( void )
{
    // Destroy any existing particle asset.
    destroyParticleAsset();

    // Finish if no particle asset.
    if ( mParticleAsset.isNull() )
        return;

    // Fetch the particle asset.
    ParticleAsset* pParticleAsset = mParticleAsset;

    // Fetch the emitter count.
    const U32 emitterCount = pParticleAsset->getEmitterCount();

    // Finish if no emitters found.
    if ( emitterCount == 0 )
        return;

    // Add each emitter reference.
    for( U32 emitterIndex = 0; emitterIndex < emitterCount; ++emitterIndex )
    {
        // Fetch the asset emitter.
        ParticleAssetEmitter* pParticleAssetEmitter = pParticleAsset->getEmitter( emitterIndex );

        // Fetch both image and animation assets.
        const AssetPtr<ImageAsset>& imageAsset = pParticleAssetEmitter->getImageAsset();
        const AssetPtr<AnimationAsset>& animationAsset = pParticleAssetEmitter->getAnimationAsset();

        // Skip if the emitter does not have a valid assigned asset to render.
        if (( pParticleAssetEmitter->isStaticMode() && (imageAsset.isNull() || imageAsset->getFrameCount() == 0 ) ) ||
            ( !pParticleAssetEmitter->isStaticMode() && (animationAsset.isNull() || animationAsset->getValidatedAnimationFrames().size() == 0 ) ) )
            continue;

        // Create a new emitter node.
        EmitterNode* pEmitterNode = new EmitterNode( this, pParticleAssetEmitter );

        // Store new emitter node.
        mEmitters.push_back( pEmitterNode );
    }
}

//-----------------------------------------------------------------------------

void ParticlePlayer::destroyParticleAsset( void )
{
    // Stop playing.
    stop( false, false );

    // Destroy all emitters.
    while( mEmitters.size() > 0 )
    {
        delete mEmitters[0];
    }
    mEmitters.clear();
}