//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "console/consoleTypes.h"
#include "2d/assets/ImageAsset.h"
#include "2d/assets/AnimationAsset.h"

// Script bindings.
#include "AnimationAsset_ScriptBinding.h"

// Debug Profiling.
#include "debug/profiler.h"

//------------------------------------------------------------------------------

IMPLEMENT_CONOBJECT(AnimationAsset);

//------------------------------------------------------------------------------

AnimationAsset::AnimationAsset() :    mAnimationTime(1.0f),
                                                    mAnimationCycle(true),
                                                    mRandomStart(false),
                                                    mAnimationIntegration(0.0f)
{
    // Set Vector Associations.
    VECTOR_SET_ASSOCIATION( mAnimationFrames );
    VECTOR_SET_ASSOCIATION( mValidatedFrames );    
}

//------------------------------------------------------------------------------

AnimationAsset::~AnimationAsset()
{
}

//------------------------------------------------------------------------------

void AnimationAsset::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();

    addProtectedField("Image", TypeImageAssetPtr, Offset(mImageAsset, AnimationAsset), &setImage, &defaultProtectedGetFn, &writeImage, "");
    addProtectedField("AnimationFrames", TypeS32Vector, Offset(mAnimationFrames, AnimationAsset), &setAnimationFrames, &defaultProtectedGetFn, &writeAnimationFrames, "");
    addProtectedField("AnimationTime", TypeF32, Offset(mAnimationTime, AnimationAsset), &setAnimationTime, &defaultProtectedGetFn, &defaultProtectedWriteFn, "");
    addProtectedField("AnimationCycle", TypeBool, Offset(mAnimationCycle, AnimationAsset), &setAnimationCycle, &defaultProtectedGetFn, &writeAnimationCycle, "");
    addProtectedField("RandomStart", TypeBool, Offset(mRandomStart, AnimationAsset), &setRandomStart, &defaultProtectedGetFn, &writeRandomStart, "");
}

//------------------------------------------------------------------------------

bool AnimationAsset::onAdd()
{
    // Call Parent.
    if(!Parent::onAdd())
        return false;

    // Return Okay.
    return true;
}

//------------------------------------------------------------------------------

void AnimationAsset::onRemove()
{
    // Call Parent.
    Parent::onRemove();
}

//------------------------------------------------------------------------------

void AnimationAsset::onAssetRefresh( void ) 
{
    // Ignore if not yet added to the sim.
    if ( !isProperlyAdded() )
        return;

    // Call parent.
    Parent::onAssetRefresh();
}

//------------------------------------------------------------------------------

void AnimationAsset::setImage( const char* pAssetId )
{
    // Ignore no change.
    if ( mImageAsset.getAssetId() == StringTable->insert( pAssetId ) )
        return;

    // Update.
    mImageAsset = pAssetId;

    // Validate frames.
    validateFrames();

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void AnimationAsset::setAnimationFrames( const char* pAnimationFrames )
{
    // Debug Profiling.
    PROFILE_SCOPE(AnimationAsset_SetAnimationFrames);

    // Clear any existing frames.
    mAnimationFrames.clear();

    // Fetch frame count.
    const U32 frameCount = StringUnit::getUnitCount( pAnimationFrames, " \t\n" );

    // Iterate frames.
    for( U32 frameIndex = 0; frameIndex < frameCount; ++frameIndex )
    {
        // Store frame.
        mAnimationFrames.push_back( dAtoi( StringUnit::getUnit( pAnimationFrames, frameIndex, " \t\n" ) ) );
    }

    // Validate frames.
    validateFrames();

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void AnimationAsset::setAnimationTime( const F32 animationTime )
{
    // Ignore no change,
    if ( mIsEqual( animationTime, mAnimationTime ) )
        return;

    // Update.
    mAnimationTime = animationTime;

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void AnimationAsset::setAnimationCycle( const bool animationCycle )
{
    // Ignore no change.
    if ( animationCycle == mAnimationCycle )
        return;

    // Update.
    mAnimationCycle = animationCycle;

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void AnimationAsset::setRandomStart( const bool randomStart )
{
    // Ignore no change.
    if ( randomStart == mRandomStart )
        return;

    // Update.
    mRandomStart = randomStart;

    // Refresh the asset.
    refreshAsset();
}

//------------------------------------------------------------------------------

void AnimationAsset::validateFrames( void )
{
    // Debug Profiling.
    PROFILE_SCOPE(AnimationAsset_ValidateFrames);

    // Clear validated frames.
    mValidatedFrames.clear();

    // Finish if we don't have a valid image asset.
    if ( mImageAsset.isNull() )
        return;

    // Fetch Animation Frame Count.
    const U32 animationFrameCount = (U32)mAnimationFrames.size();

    // Finish if no animation frames are specified.
    if ( animationFrameCount == 0 )
        return;

    // Fetch image asset frame count.
    const S32 imageAssetFrameCount = (S32)mImageAsset->getFrameCount();

    // Finish if the image has no frames.
    if ( imageAssetFrameCount == 0 )
        return;

    // Validate each specified frame.
    for ( U32 frameIndex = 0; frameIndex < animationFrameCount; ++frameIndex )
    {
        // Fetch frame.
        S32 frame = mAnimationFrames[frameIndex];

        // Valid Frame?
        if ( frame < 0 || frame >= imageAssetFrameCount )
        {
            // No, warn.
            Con::warnf( "AnimationAsset::validateFrames() - Animation asset '%s' specifies an out-of-bound frame of '%d' (key-index:'%d') against image asset Id '%s'.",
                getAssetName(),
                frame,
                frameIndex,
                mImageAsset.getAssetId() );

            if ( frame < 0 )
                frame = 0;
            else if ( frame >= imageAssetFrameCount )
                frame = imageAssetFrameCount-1;
        }

        // Use frame.
        mValidatedFrames.push_back( frame );
    }
}

//------------------------------------------------------------------------------

bool AnimationAsset::isAssetValid( void ) const
{
    return mImageAsset.notNull() && mImageAsset->isAssetValid() && mValidatedFrames.size() > 0;
}

//------------------------------------------------------------------------------

void AnimationAsset::initializeAsset( void )
{
    // Call parent.
    Parent::initializeAsset();

    // Currently there is no specific initialization required.
}

