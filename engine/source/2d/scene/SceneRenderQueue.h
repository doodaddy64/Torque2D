//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SCENE_RENDER_QUEUE_H_
#define _SCENE_RENDER_QUEUE_H_

#ifndef _SCENE_RENDER_REQUEST_H_
#include "2d/scene/SceneRenderRequest.h"
#endif

// Debug Profiling.
#include "debug/profiler.h"

//-----------------------------------------------------------------------------

class SceneRenderQueue : public IFactoryObjectReset
{
public:
    typedef Vector<SceneRenderRequest*> typeRenderRequestVector;

    // Scene Render Request Sort.
    enum RenderSort
    {
        RENDER_SORT_INVALID,
        ///---
        RENDER_SORT_OFF,
        RENDER_SORT_NEWEST,
        RENDER_SORT_OLDEST,
        RENDER_SORT_BATCH,
        RENDER_SORT_GROUP,
        RENDER_SORT_XAXIS,
        RENDER_SORT_YAXIS,
        RENDER_SORT_ZAXIS,
        RENDER_SORT_INVERSE_XAXIS,
        RENDER_SORT_INVERSE_YAXIS,
        RENDER_SORT_INVERSE_ZAXIS,
    };

private: 
    typeRenderRequestVector mRenderRequests;
    RenderSort              mSortMode;
    bool                    mStrictOrderMode;

private:
    static S32 QSORT_CALLBACK layeredNewFrontSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredOldFrontSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredDepthSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredInverseDepthSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layerBatchOrderSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layerGroupOrderSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredXSortPointSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredYSortPointSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredInverseXSortPointSort(const void* a, const void* b);
    static S32 QSORT_CALLBACK layeredInverseYSortPointSort(const void* a, const void* b);

public:
    SceneRenderQueue()
    {
        resetState();
    }
    virtual ~SceneRenderQueue()
    {
        resetState();
    }

    virtual void resetState( void )
    {
        // Debug Profiling.
        PROFILE_SCOPE(SceneRenderQueue_ResetState);

        // Cache request.
        for( typeRenderRequestVector::iterator itr = mRenderRequests.begin(); itr != mRenderRequests.end(); ++itr )
        {
            SceneRenderRequestFactory.cacheObject( *itr );
        }
        mRenderRequests.clear();

        // Reset sort mode.
        mSortMode = RENDER_SORT_NEWEST;

        // Set strict order mode.
        mStrictOrderMode = true;
    }

    inline SceneRenderRequest* createRenderRequest( void )
    {
        // Debug Profiling.
        PROFILE_SCOPE(SceneRenderQueue_CreateRenderRequest);

        // Create scene render request.
        SceneRenderRequest* pSceneRenderRequest = SceneRenderRequestFactory.createObject();

        // Queue render request.
        mRenderRequests.push_back( pSceneRenderRequest );

        return pSceneRenderRequest;
    }

    inline typeRenderRequestVector& getRenderRequests( void ) { return mRenderRequests; }

    inline void setSortMode( RenderSort sortMode ) { mSortMode = sortMode; }
    inline RenderSort getSortMode( void ) const { return mSortMode; }

    inline void setStrictOrderMode( const bool strictOrderMode ) { mStrictOrderMode = strictOrderMode; }
    inline bool getStrictOrderMode( void ) const { return mStrictOrderMode; }

    void sort( void )
    {
        // Sort layer appropriately.
        switch( mSortMode )
        {
            case RENDER_SORT_NEWEST:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortNewest);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredNewFrontSort );
                    return;
                }

            case RENDER_SORT_OLDEST:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortOldest);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredOldFrontSort );
                    return;
                }

            case RENDER_SORT_BATCH:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortBatch);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layerBatchOrderSort );

                    // Batching means we don't need strict order.
                    mStrictOrderMode = false;
                    return;
                }

            case RENDER_SORT_GROUP:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortGroup);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layerGroupOrderSort );
                    return;
                }

            case RENDER_SORT_XAXIS:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortXAxis);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredXSortPointSort);
                    return;
                }

            case RENDER_SORT_YAXIS:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortYAxis);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredYSortPointSort );
                    return;
                }

            case RENDER_SORT_ZAXIS:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortZAxis);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredDepthSort );
                    return;
                }

            case RENDER_SORT_INVERSE_XAXIS:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortInverseXAxis);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredInverseXSortPointSort );
                    return;
                }

            case RENDER_SORT_INVERSE_YAXIS:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortInverseYAxis);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredInverseYSortPointSort );
                    return;
                }

            case RENDER_SORT_INVERSE_ZAXIS:
                {
                    // Debug Profiling.
                    PROFILE_SCOPE(SceneRenderQueue_SortInverseZAxis);

                    dQsort( mRenderRequests.address(), mRenderRequests.size(), sizeof(SceneRenderRequest*), layeredInverseDepthSort );
                    return;
                }

            case RENDER_SORT_OFF:
                {
                    return;
                }

            default:
                break;
        };
    }

    static RenderSort getRenderSortEnum(const char* label);
    static const char* getRenderSortDescription( const RenderSort& sortMode );
    static EnumTable renderSortTable;
};

#endif // _SCENE_RENDER_QUEUE_H_
