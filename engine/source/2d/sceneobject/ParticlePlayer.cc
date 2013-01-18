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
    mOwner->configureParticle( pFreeParticleNode );

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
    addProtectedField( "CameraIdleDistance", TypeF32, Offset(mCameraIdleDistance, ParticlePlayer), &setCameraIdleDistance, &defaultProtectedGetFn, &writeCameraIdleDistance,"" );

}

//------------------------------------------------------------------------------

bool ParticlePlayer::onAdd()
{
    // Call Parent.
    if(!Parent::onAdd())
        return false;

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

void ParticlePlayer::onRemove()
{
    // Call Parent.
    Parent::onRemove();
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
    if ( !mPlaying || mCameraIdle || mPaused || mEmitters.size() == 0 )
        return;

    // Yes, so update the particle player age.
    mAge += elapsedTime;

    // Fetch particle asset.
    ParticleAsset* pParticleAsset = mParticleAsset;

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
                integrateParticle( pParticleNode, pParticleNode->mParticleAge / pParticleNode->mParticleLifetime, elapsedTime );

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

            // Calculate the final time-independent emission.
            const U32 emission = U32(mFloor( localEmission * pEmitterNode->getTimeSinceLastGeneration() ));

            // Do we have an emission?
            if ( emission > 0 )
            {
                // Yes, so remove this emission from accumulated time.
                pEmitterNode->setTimeSinceLastGeneration( getMax(0.0f, pEmitterNode->getTimeSinceLastGeneration() - (emission / localEmission) ) );

                // Suppress Precision Errors.
                if ( mIsZero( pEmitterNode->getTimeSinceLastGeneration() ) )
                    pEmitterNode->setTimeSinceLastGeneration( 0.0f );

                // Generate the required emission.
                for ( U32 n = 0; n < emission; n++ )
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
    //// Fetch First Particle Node.
    //ParticleNode* pParticleNode = mParticleNodeHead.mNextNode;

    //// Process All particle nodes.
    //while ( pParticleNode != &mParticleNodeHead )
    //{
    //    // Interpolate Particle.
    //    pParticleNode->mRenderTickPosition = (timeDelta * pParticleNode->mPreTickPosition) + ((1.0f-timeDelta) * pParticleNode->mPostTickPosition);

    //    pParticleNode->mRotationTransform.p = pParticleNode->mRenderTickPosition;

    //    Vector2 renderSize = pParticleNode->mRenderSize;
    //    Vector2 clipBoundary[4];
    //    clipBoundary[0] = mGlobalClipBoundary[0] * renderSize;
    //    clipBoundary[1] = mGlobalClipBoundary[1] * renderSize;
    //    clipBoundary[2] = mGlobalClipBoundary[2] * renderSize;
    //    clipBoundary[3] = mGlobalClipBoundary[3] * renderSize;

    //    // Calculate Clip Boundary.
    //    CoreMath::mCalculateOOBB( clipBoundary, pParticleNode->mRotationTransform, pParticleNode->mOOBB );

    //    // Move to next Particle Node.
    //    pParticleNode = pParticleNode->mNextNode;
    //}
}

//-----------------------------------------------------------------------------

void ParticlePlayer::sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer )
{
    //// Render particles?
    //if ( !mEffectPlaying || mCameraIdle )
    //    return;

    //// Render all emitters.
    //for ( S32 n = mParticleEmitterList.size()-1; n >= 0 ; n-- )
    //{
    //    ParticleEmitter *pEmitter = dynamic_cast<ParticleEmitter*>( mParticleEmitterList[n].mpSceneObject );
    //    if ( pEmitter && pEmitter->getEmitterVisible() )
    //    {
    //        pEmitter->sceneRender( pSceneRenderState, pSceneRenderRequest, pBatchRenderer );
    //    }
    //}
}

//-----------------------------------------------------------------------------

void ParticlePlayer::sceneRenderOverlay( const SceneRenderState* sceneRenderState )
{
    //// Call parent.
    //Parent::sceneRenderOverlay( sceneRenderState );

    //// Get Scene.
    //Scene* pScene = getScene();

    //// Cannot do anything without scene!
    //if ( !pScene )
    //    return;

    //// Finish if we shouldn't be drawing the debug overlay.
    //if ( !pScene->getIsEditorScene() || mCameraIdleDistance <= 0.0f || !isEnabled() || !getVisible() )
    //    return;

    //// Draw camera pause distance.
    //pScene->mDebugDraw.DrawCircle( getRenderPosition(), mCameraIdleDistance, b2Color(1.0f, 1.0f, 0.0f ) );
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
    //// Ignore if we're not playing and there's no kill command.
    //if ( !mEffectPlaying && !killEffect )
    //    return;

    //// Are we waiting for particles to end?
    //if ( waitForParticles )
    //{
    //    // Yes, so pause all emitters.
    //    for ( U32 n = 0; n < (U32)mParticleEmitterList.size(); n++ )
    //    {
    //        ParticleEmitter* pEmitter = dynamic_cast<ParticleEmitter*>( Sim::findObject( mParticleEmitterList[n].mObjectId ) );
    //        if( pEmitter )
    //        pEmitter->pauseEmitter();
    //    }

    //    // Flag Waiting for Particles.
    //    mWaitingForParticles = true;

    //    // Flag as waiting for deletion if killing effect.
    //    if ( killEffect )
    //        mWaitingForDelete = true;
    //}
    //else
    //{
    //    // No, so stop All Emitters.
    //    for ( U32 n = 0; n < (U32)mParticleEmitterList.size(); n++ )
    //    {
    //        // Fetch Particle Emitter Pointer.
    //        ParticleEmitter* pEmitter = dynamic_cast<ParticleEmitter*>( Sim::findObject( mParticleEmitterList[n].mObjectId ) );
    //        if( pEmitter )
    //        pEmitter->stopEmitter();
    //    }

    //    // Reset Effect Age.
    //    mEffectAge = 0.0f;

    //    // Flag as Stopped and not waiting.
    //    mEffectPlaying = mWaitingForParticles = mWaitingForDelete = false;

    //    // Turn off effect pause.
    //    mEffectPaused = false;

    //    // Set Safe Delete.
    //    setSafeDelete(true);

    //    // Perform "OnStopEffect" Callback.
    //    if( isMethod( "onStopEffect" ) )
    //        Con::executef( this, 1, "onStopEffect" );

    //    // Flag for immediate Deletion if killing.
    //    if ( killEffect )
    //        safeDelete();
    //}
}

//------------------------------------------------------------------------------

void ParticlePlayer::configureParticle( ParticleSystem::ParticleNode* pParticleNode )
{
    //// Fetch Effect Age.
    //const F32 effectAge = pParentEffectObject->mEffectAge;

    //// Fetch Effect Position.
    //const Vector2 effectPos = pParentEffectObject->getPosition();


    //// Default to not suppressing movement.
    //pParticleNode->mSuppressMovement = false;


    //// **********************************************************************************************************************
    //// Calculate Particle Position.
    //// **********************************************************************************************************************

    //// Are we using Single Particle?
    //if ( mSingleParticle )
    //{
    //    // Yes, so if use Effect Position ( or origin if attached to emitter).
    //    if ( mAttachPositionToEmitter )
    //        pParticleNode->mPosition.Set(0.0f, 0.0f);
    //    else
    //        pParticleNode->mPosition = effectPos;
    //}
    //else
    //{
    //    // No, so select Emitter-Type.
    //    switch( mEmitterType )
    //    {
    //        // Use Pivot-Point.
    //        case POINT:
    //        {
    //            // Yes, so if use Effect Position ( or origin if attached to emitter).
    //            if ( mAttachPositionToEmitter )
    //                pParticleNode->mPosition.Set(0.0f, 0.0f);
    //            else
    //                pParticleNode->mPosition = effectPos;

    //        } break;

    //        // Use X-Size and Pivot-Y.
    //        case LINEX:
    //        {
    //            // Fetch Local Clip Boundary.
    //            const b2Vec2* pLocalSizeVertice = pParentEffectObject->getLocalSizeVertices();

    //            // Choose Random Position along line within Boundary with @ Pivot-Y.
    //            F32 minX = pLocalSizeVertice[0].x;
    //            F32 maxX = pLocalSizeVertice[1].x;
    //            F32 midY = (pLocalSizeVertice[0].y + pLocalSizeVertice[3].y) * 0.5f;

    //            Vector2 tempPos;

    //            // Normalise.
    //            if ( minX > maxX )
    //                mSwap( minX, maxX );

    //            // Potential Vertical Line.
    //            if ( minX == maxX )
    //                tempPos.Set( minX, midY );
    //            else
    //                // Normalised.
    //                tempPos.Set( CoreMath::mGetRandomF( minX, maxX ), midY );

    //            // Rotate into Emitter-Space.
    //            //
    //            // NOTE:-   If we're attached to the emitter, then we keep the particle coordinates
    //            //          in local-space and use the hardware to render them in emitter-space.
    //            b2Transform xform;
    //            xform.Set( mAttachPositionToEmitter ? b2Vec2_zero : effectPos, (mAttachPositionToEmitter && mAttachRotationToEmitter) ? 0.0f : pParentEffectObject->getAngle() );
    //            pParticleNode->mPosition = b2Mul( xform, tempPos );

    //        } break;

    //        // Use Y-Size and Pivot-X.
    //        case LINEY:
    //        {
    //            // Fetch Local Clip Boundary.
    //            const b2Vec2* pLocalSizeVertice = pParentEffectObject->getLocalSizeVertices();

    //            // Choose Random Position along line within Boundary with @ Pivot-Y.
    //            F32 midX = (pLocalSizeVertice[0].x + pLocalSizeVertice[1].x) * 0.5f;
    //            F32 minY = pLocalSizeVertice[0].y;
    //            F32 maxY = pLocalSizeVertice[3].y;

    //            Vector2 tempPos;

    //            // Normalise.
    //            if ( minY > maxY )
    //                mSwap( minY, maxY );

    //            // Potential Horizontal Line.
    //            if ( minY == maxY )
    //                tempPos.Set( midX, minY );
    //            else
    //                // Normalised.
    //                tempPos.Set( midX, CoreMath::mGetRandomF( minY, maxY ) );

    //            // Rotate into Emitter-Space.
    //            //
    //            // NOTE:-   If we're attached to the emitter, then we keep the particle coordinates
    //            //          in local-space and use the hardware to render them in emitter-space.
    //            b2Transform xform;
    //            xform.Set( mAttachPositionToEmitter ? b2Vec2_zero : effectPos, (mAttachPositionToEmitter && mAttachRotationToEmitter) ? 0.0f : pParentEffectObject->getAngle() );
    //            pParticleNode->mPosition = b2Mul( xform, tempPos );

    //        } break;

    //        // Use X/Y Sizes.
    //        case AREA:
    //        {
    //            // Fetch Local Clip Boundary.
    //            const b2Vec2* pLocalSizeVertice = pParentEffectObject->getLocalSizeVertices();

    //            // Choose a random position in the area.
    //            F32 minX = pLocalSizeVertice[0].x;
    //            F32 maxX = pLocalSizeVertice[1].x;
    //            F32 minY = pLocalSizeVertice[0].y;
    //            F32 maxY = pLocalSizeVertice[3].y;

    //            Vector2 tempPos;

    //            // Normalise.
    //            if ( minX > maxX )
    //                mSwap( minX, maxX );
    //            // Normalise.
    //            if ( minY > maxY )
    //                mSwap( minY, maxY );

    //            // Normalised.
    //            tempPos.Set( (minX == maxX) ? minX : CoreMath::mGetRandomF( minX, maxX ), (minY == maxY) ? minY : CoreMath::mGetRandomF( minY, maxY ) );

    //            // Rotate into Emitter-Space.
    //            //
    //            // NOTE:-   If we're attached to the emitter, then we keep the particle coordinates
    //            //          in local-space and use the hardware to render them in emitter-space.
    //            b2Transform xform;
    //            xform.Set( mAttachPositionToEmitter ? b2Vec2_zero : effectPos, (mAttachPositionToEmitter && mAttachRotationToEmitter) ? 0.0f : pParentEffectObject->getAngle() );
    //            pParticleNode->mPosition = b2Mul( xform, tempPos );

    //        } break;
    //    }
    //}


    //// **********************************************************************************************************************
    //// Calculate Particle Lifetime.
    //// **********************************************************************************************************************
    //pParticleNode->mParticleAge = 0.0f;
    //pParticleNode->mParticleLifetime = ParticleAssetField::calculateFieldBVE( mParticleLife.mBase,
    //                                                                    mParticleLife.mVariation,
    //                                                                    pParentEffectObject->mParticleLife.mBase,
    //                                                                    effectAge );


    //// **********************************************************************************************************************
    //// Calculate Particle Size-X.
    //// **********************************************************************************************************************
    //pParticleNode->mSize.x = ParticleAssetField::calculateFieldBVE(  mSizeX.mBase,
    //                                                        mSizeX.mVariation,
    //                                                        pParentEffectObject->mSizeX.mBase,
    //                                                        effectAge );

    //// Is the Particle Aspect-Locked?
    //if ( mFixedAspect )
    //{
    //    // Yes, so simply copy Size-X.
    //    pParticleNode->mSize.y = pParticleNode->mSize.x;
    //}
    //else
    //{
    //    // No, so calculate Particle Size-Y.
    //    pParticleNode->mSize.y = ParticleAssetField::calculateFieldBVE(  mSizeY.mBase,
    //                                                            mSizeY.mVariation,
    //                                                            pParentEffectObject->mSizeY.mBase,
    //                                                            effectAge );
    //}

    //// Reset Render Size.
    //pParticleNode->mRenderSize.Set(-1,-1);


    //// **********************************************************************************************************************
    //// Calculate Speed.
    //// **********************************************************************************************************************

    //// Ignore if we're using Single-Particle.
    //if ( !mSingleParticle )
    //{
    //    pParticleNode->mSpeed = ParticleAssetField::calculateFieldBVE(    mSpeed.mBase,
    //                                                            mSpeed.mVariation,
    //                                                            pParentEffectObject->mSpeed.mBase,
    //                                                            effectAge );
    //}


    //// **********************************************************************************************************************
    //// Calculate Spin.
    //// **********************************************************************************************************************
    //pParticleNode->mSpin = ParticleAssetField::calculateFieldBVE( mSpin.mBase,
    //                                                        mSpin.mVariation,
    //                                                        pParentEffectObject->mSpin.mBase,
    //                                                        effectAge );


    //// **********************************************************************************************************************
    //// Calculate Fixed-Force.
    //// **********************************************************************************************************************
    //pParticleNode->mFixedForce = ParticleAssetField::calculateFieldBVE(   mFixedForce.mBase,
    //                                                            mFixedForce.mVariation,
    //                                                            pParentEffectObject->mFixedForce.mBase,
    //                                                            effectAge );


    //// **********************************************************************************************************************
    //// Calculate Random Motion..
    //// **********************************************************************************************************************

    //// Ignore if we're using Single-Particle.
    //if ( !mSingleParticle )
    //{
    //    pParticleNode->mRandomMotion = ParticleAssetField::calculateFieldBVE( mRandomMotion.mBase,
    //                                                                    mRandomMotion.mVariation,
    //                                                                    pParentEffectObject->mRandomMotion.mBase,
    //                                                                    effectAge );
    //}


    //// **********************************************************************************************************************
    //// Calculate Emission Angle.
    //// **********************************************************************************************************************

    //// Note:- We reset the emission angle/arc in-case we're using Single-Particle mode as this is the default.
    //F32 emissionForce = 0;
    //F32 emissionAngle = 0;
    //F32 emissionArc = 0;

    //// Ignore if we're using Single-Particle.
    //if ( !mSingleParticle )
    //{
    //    // Are we using the Effects Emission?
    //    if ( mUseEffectEmission )
    //    {
    //        // Yes, so calculate emission from effect...

    //        // Calculate Emission Force.
    //        emissionForce = ParticleAssetField::calculateFieldBV( pParentEffectObject->mEmissionForce.mBase,
    //                                                        pParentEffectObject->mEmissionForce.mVariation,
    //                                                        effectAge);

    //        // Calculate Emission Angle.
    //        emissionAngle = ParticleAssetField::calculateFieldBV( pParentEffectObject->mEmissionAngle.mBase,
    //                                                        pParentEffectObject->mEmissionAngle.mVariation,
    //                                                        effectAge);

    //        // Calculate Emission Arc.
    //        // NOTE:-   We're actually interested in half the emission arc!
    //        emissionArc = ParticleAssetField::calculateFieldBV(   pParentEffectObject->mEmissionArc.mBase,
    //                                                    pParentEffectObject->mEmissionArc.mVariation,
    //                                                    effectAge ) * 0.5f;
    //    }
    //    else
    //    {
    //        // No, so calculate emission from emitter...

    //        // Calculate Emission Force.
    //        emissionForce = ParticleAssetField::calculateFieldBV( mEmissionForce.mBase,
    //                                                        mEmissionForce.mVariation,
    //                                                        effectAge);

    //        // Calculate Emission Angle.
    //        emissionAngle = ParticleAssetField::calculateFieldBV( mEmissionAngle.mBase,
    //                                                    mEmissionAngle.mVariation,
    //                                                    effectAge );

    //        // Calculate Emission Arc.
    //        // NOTE:-   We're actually interested in half the emission arc!
    //        emissionArc = ParticleAssetField::calculateFieldBV(   mEmissionArc.mBase,
    //                                                    mEmissionArc.mVariation,
    //                                                    effectAge ) * 0.5f;
    //    }

    //    // Is the Emission Rotation linked?
    //    if ( mLinkEmissionRotation )
    //        // Yes, so add Effect Object-Rotation.
    //        emissionAngle += pParentEffectObject->getAngle();

    //    // Calculate Final Emission Angle by choosing random Arc.
    //    emissionAngle = mFmod( CoreMath::mGetRandomF( emissionAngle-emissionArc, emissionAngle+emissionArc ), 360.0f ) ;

    //    // Calculate Normalised Velocity.
    //    pParticleNode->mVelocity.Set( emissionForce * mSin( mDegToRad(emissionAngle) ), emissionForce * mCos( mDegToRad(emissionAngle) ) );
    //}

    //// **********************************************************************************************************************
    //// Calculate Orientation Angle.
    //// **********************************************************************************************************************

    //// Handle Particle Orientation Mode.
    //switch( mOrientationType )
    //{
    //    // Aligned to Initial Emission.
    //    case ALIGNED:
    //    {
    //        // Just use the emission angle plus offset.
    //        pParticleNode->mOrientationAngle = mFmod( emissionAngle - mAlignedAngleOffset, 360.0f );

    //    } break;

    //    // Fixed Orientation.
    //    case FIXED:
    //    {
    //        // Use Fixed Angle.
    //        pParticleNode->mOrientationAngle = mFmod( mFixedAngleOffset, 360.0f );

    //    } break;

    //    // Random with Constraints.
    //    case RANDOM:
    //    {
    //        // Used Random Angle/Arc.
    //        F32 randomArc = mRandomArc * 0.5f;
    //        pParticleNode->mOrientationAngle = mFmod( CoreMath::mGetRandomF( mRandomAngleOffset - randomArc, mRandomAngleOffset + randomArc ), 360.0f );

    //    } break;

    //}

    //// **********************************************************************************************************************
    //// Calculate RGBA Components.
    //// **********************************************************************************************************************

    //pParticleNode->mColour.set( mClampF( mRedChannel.mLife.getGraphValue( 0.0f ), mRedChannel.mLife.getMinValue(), mRedChannel.mLife.getMaxValue() ),
    //                            mClampF( mGreenChannel.mLife.getGraphValue( 0.0f ), mGreenChannel.mLife.getMinValue(), mGreenChannel.mLife.getMaxValue() ),
    //                            mClampF( mBlueChannel.mLife.getGraphValue( 0.0f ), mBlueChannel.mLife.getMinValue(), mBlueChannel.mLife.getMaxValue() ),
    //                            mClampF( mAlphaChannel.mLife.getGraphValue( 0.0f ) * pParentEffectObject->mAlphaChannel.mBase.getGraphValue( 0.0f ), mAlphaChannel.mLife.getMinValue(), mAlphaChannel.mLife.getMaxValue() ) );


    //// **********************************************************************************************************************
    //// Animation Controller.
    //// **********************************************************************************************************************

    //// If an animation is selected, start it playing.
    //if ( !mStaticMode && mAnimationAsset.notNull() )
    //    pParticleNode->mAnimationController.playAnimation( mAnimationAsset.getAssetId(), false );


    //// Reset Last Render Size.
    //pParticleNode->mLastRenderSize.Set(-1, -1);


    //// **********************************************************************************************************************
    //// Reset Tick Position.
    //// **********************************************************************************************************************
    //pParticleNode->mPreTickPosition = pParticleNode->mPostTickPosition = pParticleNode->mRenderTickPosition = pParticleNode->mPosition;


    //// **********************************************************************************************************************
    //// Do a Single Particle Integration to get things going.
    //// **********************************************************************************************************************
    //integrateParticle( pParticleNode, 0.0f, 0.0f );
}

//------------------------------------------------------------------------------

void ParticlePlayer::integrateParticle( ParticleSystem::ParticleNode* pParticleNode, F32 particleAge, F32 elapsedTime )
{
    //// **********************************************************************************************************************
    //// Copy Old Tick Position.
    //// **********************************************************************************************************************
    //pParticleNode->mRenderTickPosition = pParticleNode->mPreTickPosition = pParticleNode->mPostTickPosition;


    //// **********************************************************************************************************************
    //// Scale Size.
    //// **********************************************************************************************************************

    //// Scale Size-X.
    //pParticleNode->mRenderSize.x = mClampF(    pParticleNode->mSize.x * mSizeX.mLife.getGraphValue( particleAge ),
    //                                            mSizeX.mBase.getMinValue(),
    //                                            mSizeX.mBase.getMaxValue() );

    //// Is the Particle Aspect-Locked?
    //if ( mFixedAspect )
    //{
    //    // Yes, so simply copy Size-X.
    //    pParticleNode->mRenderSize.y = pParticleNode->mRenderSize.x;
    //}
    //else
    //{
    //    // No, so Scale Size-Y.
    //    pParticleNode->mRenderSize.y = mClampF(    pParticleNode->mSize.y * mSizeY.mLife.getGraphValue( particleAge ),
    //                                                mSizeY.mBase.getMinValue(),
    //                                                mSizeY.mBase.getMaxValue() );
    //}


    //// **********************************************************************************************************************
    //// Scale Speed.
    //// **********************************************************************************************************************
    //pParticleNode->mRenderSpeed = mClampF(  pParticleNode->mSpeed * mSpeed.mLife.getGraphValue( particleAge ),
    //                                        mSpeed.mBase.getMinValue(),
    //                                        mSpeed.mBase.getMaxValue() );


    //// **********************************************************************************************************************
    //// Scale Spin (if Keep Aligned is not selected)
    //// **********************************************************************************************************************
    //if ( !(mOrientationType == ALIGNED && mKeepAligned) )
    //    pParticleNode->mRenderSpin = pParticleNode->mSpin * mSpin.mLife.getGraphValue( particleAge );



    //// **********************************************************************************************************************
    //// Scale Fixed-Force.
    //// **********************************************************************************************************************
    //pParticleNode->mRenderFixedForce = mClampF( pParticleNode->mFixedForce * mFixedForce.mLife.getGraphValue( particleAge ),
    //                                            mFixedForce.mBase.getMinValue(),
    //                                            mFixedForce.mBase.getMaxValue() );


    //// **********************************************************************************************************************
    //// Scale Random-Motion.
    //// **********************************************************************************************************************
    //pParticleNode->mRenderRandomMotion = mClampF(   pParticleNode->mRandomMotion * mRandomMotion.mLife.getGraphValue( particleAge ),
    //                                                mRandomMotion.mBase.getMinValue(),
    //                                                mRandomMotion.mBase.getMaxValue() );


    //// **********************************************************************************************************************
    //// Scale Colour.
    //// **********************************************************************************************************************

    //// Red.
    //pParticleNode->mColour.red = mClampF(   mRedChannel.mLife.getGraphValue( particleAge ),
    //                                        mRedChannel.mLife.getMinValue(),
    //                                        mRedChannel.mLife.getMaxValue() );

    //// Green.
    //pParticleNode->mColour.green = mClampF( mGreenChannel.mLife.getGraphValue( particleAge ),
    //                                        mGreenChannel.mLife.getMinValue(),
    //                                        mGreenChannel.mLife.getMaxValue() );

    //// Blue.
    //pParticleNode->mColour.blue = mClampF(  mBlueChannel.mLife.getGraphValue( particleAge ),
    //                                        mBlueChannel.mLife.getMinValue(),
    //                                        mBlueChannel.mLife.getMaxValue() );

    //// Alpha.
    //pParticleNode->mColour.alpha = mClampF( mAlphaChannel.mLife.getGraphValue( particleAge ) * pParentEffectObject->mAlphaChannel.mBase.getGraphValue( particleAge ),
    //                                        mAlphaChannel.mLife.getMinValue(),
    //                                        mAlphaChannel.mLife.getMaxValue() );




    //// **********************************************************************************************************************
    //// Integrate Particle.
    //// **********************************************************************************************************************


    //// Update Animation Controller (if used).
    //if ( !mStaticMode )
    //    pParticleNode->mAnimationController.updateAnimation( elapsedTime );


    //// **********************************************************************************************************************
    //// Calculate New Velocity...
    //// **********************************************************************************************************************

    //// Only Calculate Velocity if not a Single Particle.
    //if ( !mSingleParticle )
    //{
    //    // Calculate Random Motion (if we've got any).
    //    if ( mNotZero( pParticleNode->mRenderRandomMotion ) )
    //    {
    //        // Fetch Random Motion.
    //        F32 randomMotion = pParticleNode->mRenderRandomMotion * 0.5f;
    //        // Add Time-Integrated Random-Motion into Velocity.
    //        pParticleNode->mVelocity += elapsedTime * Vector2( CoreMath::mGetRandomF(-randomMotion, randomMotion), CoreMath::mGetRandomF(-randomMotion, randomMotion) );
    //    }

    //    // Add Time-Integrated Fixed-Force into Velocity ( if we've got any ).
    //    if ( mNotZero( pParticleNode->mRenderFixedForce ) )
    //        pParticleNode->mVelocity += (mFixedForceDirection * pParticleNode->mRenderFixedForce * elapsedTime);

    //    // Suppress Movement?
    //    if ( !pParticleNode->mSuppressMovement )
    //    {
    //        // No, so adjust Particle Position.
    //        pParticleNode->mPosition += (pParticleNode->mVelocity * pParticleNode->mRenderSpeed * elapsedTime);
    //    }

    //}


    //// **********************************************************************************************************************
    //// Are we Aligning to motion?
    //// **********************************************************************************************************************
    //if ( mOrientationType == ALIGNED && mKeepAligned )
    //{
    //    // Yes, so calculate last movement direction.
    //    F32 movementAngle = mRadToDeg( mAtan( pParticleNode->mVelocity.x, -pParticleNode->mVelocity.y ) );
    //    // Adjust for Negative ArcTan-Quadrants.
    //    if ( movementAngle < 0.0f )
    //        movementAngle += 360.0f;

    //    // Set new Orientation Angle.
    //    pParticleNode->mOrientationAngle = -movementAngle - mAlignedAngleOffset;


    //    // **********************************************************************************************************************
    //    // Calculate Local Clip-Boundary.
    //    // **********************************************************************************************************************

    //    // Calculate Rotation Matrix.
    //    pParticleNode->mRotationTransform.Set( pParticleNode->mPosition, mDegToRad(pParticleNode->mOrientationAngle) );
    //}
    //else
    //{
    //    // Have we got some Spin?
    //    if ( mNotZero(pParticleNode->mRenderSpin) )
    //    {
    //        // Yes, so add into Orientation.
    //        pParticleNode->mOrientationAngle += pParticleNode->mRenderSpin * elapsedTime;
    //        // Keep within range.
    //        pParticleNode->mOrientationAngle = mFmod( pParticleNode->mOrientationAngle, 360.0f );
    //    }


    //    // If the size has changed or we have some Spin then we need to recalculate the Local Clip-Boundary.
    //    if ( mNotZero(pParticleNode->mRenderSpin) || pParticleNode->mRenderSize != pParticleNode->mLastRenderSize )
    //    {
    //        // **********************************************************************************************************************
    //        // Calculate Local Clip-Boundary.
    //        // **********************************************************************************************************************

    //        // Calculate Rotation Matrix.
    //        pParticleNode->mRotationTransform.Set( pParticleNode->mPosition, mDegToRad(pParticleNode->mOrientationAngle) );
    //    }

    //    // We've dealt with a potential Size change so store current size for next time.
    //    pParticleNode->mLastRenderSize = pParticleNode->mRenderSize;
    //}

    //// Calculate Clip Boundary.
    //Vector2 renderSize = pParticleNode->mRenderSize;
    //Vector2 clipBoundary[4];
    //clipBoundary[0] = mGlobalClipBoundary[0] * renderSize;
    //clipBoundary[1] = mGlobalClipBoundary[1] * renderSize;
    //clipBoundary[2] = mGlobalClipBoundary[2] * renderSize;
    //clipBoundary[3] = mGlobalClipBoundary[3] * renderSize;
    //CoreMath::mCalculateOOBB( clipBoundary, pParticleNode->mRotationTransform, pParticleNode->mOOBB );


    //// **********************************************************************************************************************
    //// Set Post Tick Position.
    //// **********************************************************************************************************************
    //pParticleNode->mPostTickPosition = pParticleNode->mPosition;
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
        ParticleAssetEmitter* pEmitter = pParticleAsset->getEmitter( emitterIndex );

        // Store new emitter node.
        mEmitters.push_back( new EmitterNode( this, pEmitter ) );
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