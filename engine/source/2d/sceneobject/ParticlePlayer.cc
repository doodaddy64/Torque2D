//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/sceneobject/particlePlayer.h"

// Script bindings.
#include "2d/sceneobject/particlePlayer_ScriptBinding.h"

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(ParticlePlayer);

//------------------------------------------------------------------------------

Vector<ParticlePlayer::ParticleNode*> ParticlePlayer::mParticlePool;
ParticlePlayer::ParticleNode ParticlePlayer::mParticleNodeHead;
ParticlePlayer::ParticleNode* ParticlePlayer::mpFreeParticleNodes = NULL;
U32 ParticlePlayer::mActiveParticles = 0;

//------------------------------------------------------------------------------

ParticlePlayer::ParticlePlayer() :
                    mParticlePoolBlockSize(64),
                    mEffectPaused(false),
                    mEffectPlaying(false),
                    mParticleInterpolation(false),
                    mCameraIdleDistance(0.0f),
                    mCameraIdle(false),
                    mWaitingForParticles(false),
                    mWaitingForDelete(false)
{
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

//-----------------------------------------------------------------------------

void ParticlePlayer::safeDelete( void )
{
   // Are we already waiting for delete?
   if ( mWaitingForDelete )
      // Yes, nothing to do!
      return;

   // Is effect playing?
   if ( mEffectPlaying )
   {
      // Yes, so stop the effect and allow it to kill itself.
      stopEffect(true, true);
   }
   else
   {
       // Call parent which will deal with the deletion.
       Parent::safeDelete();
   }
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
    //// Fetch First Particle Node.
    //ParticleNode* pParticleNode = mParticleNodeHead.mNextNode;

    //// Process All particle nodes.
    //while ( pParticleNode != &mParticleNodeHead )
    //{
    //    // Update Particle Life.
    //    pParticleNode->mParticleAge += elapsedTime;

    //    // Has the particle expired?
    //    // NOTE:-   If we're in Single-Particle mode then the particle
    //    //          lives as long as the emitter does!
    //    if ( (!mSingleParticle && pParticleNode->mParticleAge > pParticleNode->mParticleLifetime) || (pParticleNode->mParticleLifetime == 0.0f) )
    //    {
    //        // Yes, so fetch next particle before we kill it.
    //        pParticleNode = pParticleNode->mNextNode;

    //        // Kill Particle.
    //        // NOTE:-   Because we move to the next particle,
    //        //          the particle to kill is now the previous!
    //        freeParticle( pParticleNode->mPreviousNode );
    //    }
    //    else
    //    {
    //        // Integrate Particle.
    //        integrateParticle( pParticleNode, pParticleNode->mParticleAge / pParticleNode->mParticleLifetime, elapsedTime );

    //        // **********************************************************************************************************************
    //        // Move to next Particle Node.
    //        // **********************************************************************************************************************
    //        pParticleNode = pParticleNode->mNextNode;
    //    }
    //};


    //// Generate New Particles.

    //// Only Generate particles if we're not pause.
    //if ( !mPauseEmitter )
    //{
    //    // Are we in Single-Particle Mode?
    //    if ( mSingleParticle )
    //    {
    //        // Yes, so do we have a single particle yet?
    //        if ( mParticleNodeHead.mNextNode == &mParticleNodeHead )
    //        {
    //            // No, so generate Single Particle.
    //            createParticle();
    //        }
    //    }
    //    else
    //    {
    //        // No, so fetch Effect Age.
    //        F32 effectAge = pParentEffectObject->mEffectAge;

    //        // Accumulate Last Generation Time as we need to handle very small time-integrations correctly.
    //        //
    //        // NOTE:-   We need to do this if there's an emission target but the
    //        //          time-integration is so small that rounding results in
    //        //          no emission.  Downside to good FPS!
    //        mTimeSinceLastGeneration += elapsedTime;

    //        // Calculate Local Emission Quantity.
    //        const F32 baseEmission = mQuantity.mBase.getGraphValue( effectAge );
    //        const F32 varEmission = mQuantity.mVariation.getGraphValue( effectAge ) * 0.5f;
    //        const F32 effectEmission = pParentEffectObject->mQuantity.mBase.getGraphValue( effectAge ) * mParticlePref;

    //        const F32 localEmission = mClampF(    (baseEmission + CoreMath::mGetRandomF(-varEmission, varEmission)) * effectEmission,
    //                                              mQuantity.mBase.getMinValue(),
    //                                              mQuantity.mBase.getMaxValue() );
    //        const U32 emission = U32(mFloor(localEmission * mTimeSinceLastGeneration));

    //        // Do we have an emission?
    //        if ( emission > 0 )
    //        {
    //            // Yes, so remove this emission from accumulated time.
    //            mTimeSinceLastGeneration = getMax(0.0f, mTimeSinceLastGeneration - (emission / localEmission));

    //            // Suppress Precision Errors.
    //            if ( mIsZero( mTimeSinceLastGeneration ) )
    //                mTimeSinceLastGeneration = 0.0f;

    //            // Generate Emission.
    //            for ( U32 n = 0; n < emission; n++ )
    //                createParticle();
    //        }
    //        // No, so was there a calculated emission?
    //        else if ( localEmission == 0 )
    //        {
    //            // No, so reset accumulated time.
    //            //mTimeSinceLastGeneration = 0.0f;
    //        }
    //    }
    //}
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

bool ParticlePlayer::playEffect( bool resetParticles )
{
    // Cannot do anything if we've not got any emitters!
    if ( mParticleAsset.isNull() || mParticleAsset->getEmitterCount() == 0 )
    {
        // Warn.
        Con::warnf("ParticlePlayer::playEffect() - Cannot play; no emitters!");
        return false;
    }

    // Are we in a Scene?
    if ( getScene() == NULL )
    {
        // No, so warn.
        Con::warnf("ParticlePlayer::playEffect() - Cannot play; not in a scene!");
        return false;
    }

    // Reset Effect Age.
    mEffectAge = 0.0f;

    // Fetch particle asset.
    ParticleAsset* pParticleAsset = mParticleAsset;

    // Fetch emitter count.
    const U32 emitterCount = pParticleAsset->getEmitterCount();

    // Play All Emitters.
    for ( U32 n = 0; n < emitterCount; n++ )
    {
        //ParticleAssetEmitter* pEmitter = pParticleAsset->getEmitter( emitterCount );
        //if( pEmitter )
            //pEmitter->playEmitter( resetParticles );
    }

    // Reset Waiting for Particles.
    mWaitingForParticles = false;

    // Reset Waiting for delete.
    mWaitingForDelete = false;

    // Flag as Playing.
    mEffectPlaying = true;

    // Turn off effect pause.
    mEffectPaused = false;

    // Set Unsafe Delete!
    setSafeDelete(false);

    // Return Okay.
    return true;
}

//-----------------------------------------------------------------------------

void ParticlePlayer::stopEffect( bool waitForParticles, bool killEffect )
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

ParticlePlayer::ParticleNode* ParticlePlayer::createParticle( void )
{
    // Have we got any free particle nodes?
    if ( mpFreeParticleNodes == NULL )
        // No, so create a new block.
        createParticlePoolBlock();

    // Fetch Free Node.
    ParticleNode* pFreeParticleNode = mpFreeParticleNodes;

    // Reposition Free Node Reference.
    mpFreeParticleNodes = mpFreeParticleNodes->mNextNode;

    // Insert Particle into Head Chain.
    pFreeParticleNode->mNextNode        = mParticleNodeHead.mNextNode;
    pFreeParticleNode->mPreviousNode    = &mParticleNodeHead;
    mParticleNodeHead.mNextNode         = pFreeParticleNode;
    pFreeParticleNode->mNextNode->mPreviousNode = pFreeParticleNode;

    // Increase Active Particle Count.
    mActiveParticles++;

    // Configure Particle.
    configureParticle( pFreeParticleNode );

    // Return New Particle.
    return pFreeParticleNode;
}

//------------------------------------------------------------------------------

void ParticlePlayer::freeParticle( ParticleNode* pParticleNode )
{
    // Remove Particle from Head Chain.
    pParticleNode->mPreviousNode->mNextNode = pParticleNode->mNextNode;
    pParticleNode->mNextNode->mPreviousNode = pParticleNode->mPreviousNode;

    // Reset Extraneous Data.
    pParticleNode->mPreviousNode = NULL;
    
    // Insert into Free Pool.
    pParticleNode->mNextNode = mpFreeParticleNodes;
    mpFreeParticleNodes = pParticleNode;

    // Decrease Active Particle Count.
    mActiveParticles--;
}

//------------------------------------------------------------------------------

void ParticlePlayer::freeAllParticles( void )
{
    // Free All Allocated Particles.
    while( mParticleNodeHead.mNextNode != &mParticleNodeHead )
        freeParticle( mParticleNodeHead.mNextNode );
}

//------------------------------------------------------------------------------

void ParticlePlayer::createParticlePoolBlock( void )
{
    // Generate Free Pool Block.
    ParticleNode* pFreePoolBlock = new ParticleNode[mParticlePoolBlockSize];

    // Generate/Add New Pool Block.
    mParticlePool.push_back( pFreePoolBlock );

    // Initialise Free Pool Block.
    for ( U32 n = 0; n < (mParticlePoolBlockSize-1); n++ )
    {
        pFreePoolBlock[n].mPreviousNode = NULL;
        pFreePoolBlock[n].mNextNode     = pFreePoolBlock+n+1;
    }

    // Insert Last Node Preceding any existing free nodes.
    pFreePoolBlock[mParticlePoolBlockSize-1].mPreviousNode = NULL;
    pFreePoolBlock[mParticlePoolBlockSize-1].mNextNode = mpFreeParticleNodes;

    // Set Free References.
    mpFreeParticleNodes = pFreePoolBlock;
}

//------------------------------------------------------------------------------

void ParticlePlayer::destroyParticlePool( void )
{
    // Free All Particles.
    freeAllParticles();

    // Destroy All Blocks.
    for ( U32 n = 0; n < (U32)mParticlePool.size(); n++ )
        delete [] mParticlePool[n];

    // Clear particle pool.
    mParticlePool.clear();

    // Reset Free Particles.
    mpFreeParticleNodes = NULL;

    // Reset particle node head.
    mParticleNodeHead.mNextNode = mParticleNodeHead.mPreviousNode = &mParticleNodeHead;
}

//------------------------------------------------------------------------------

void ParticlePlayer::configureParticle( ParticleNode* pParticleNode )
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

    //pParticleNode->mColour.set( mClampF( mColourRed.mLife.getGraphValue( 0.0f ), mColourRed.mLife.getMinValue(), mColourRed.mLife.getMaxValue() ),
    //                            mClampF( mColourGreen.mLife.getGraphValue( 0.0f ), mColourGreen.mLife.getMinValue(), mColourGreen.mLife.getMaxValue() ),
    //                            mClampF( mColourBlue.mLife.getGraphValue( 0.0f ), mColourBlue.mLife.getMinValue(), mColourBlue.mLife.getMaxValue() ),
    //                            mClampF( mVisibility.mLife.getGraphValue( 0.0f ) * pParentEffectObject->mVisibility.mBase.getGraphValue( 0.0f ), mVisibility.mLife.getMinValue(), mVisibility.mLife.getMaxValue() ) );


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

void ParticlePlayer::integrateParticle( ParticleNode* pParticleNode, F32 particleAge, F32 elapsedTime )
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
    //pParticleNode->mColour.red = mClampF(   mColourRed.mLife.getGraphValue( particleAge ),
    //                                        mColourRed.mLife.getMinValue(),
    //                                        mColourRed.mLife.getMaxValue() );

    //// Green.
    //pParticleNode->mColour.green = mClampF( mColourGreen.mLife.getGraphValue( particleAge ),
    //                                        mColourGreen.mLife.getMinValue(),
    //                                        mColourGreen.mLife.getMaxValue() );

    //// Blue.
    //pParticleNode->mColour.blue = mClampF(  mColourBlue.mLife.getGraphValue( particleAge ),
    //                                        mColourBlue.mLife.getMinValue(),
    //                                        mColourBlue.mLife.getMaxValue() );

    //// Alpha.
    //pParticleNode->mColour.alpha = mClampF( mVisibility.mLife.getGraphValue( particleAge ) * pParentEffectObject->mVisibility.mBase.getGraphValue( particleAge ),
    //                                        mVisibility.mLife.getMinValue(),
    //                                        mVisibility.mLife.getMaxValue() );




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
    Parent::onTamlAddParent( pParentObject );

    playEffect( true );
}

