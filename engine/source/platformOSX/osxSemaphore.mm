//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import <pthread.h>
#import <stdlib.h>
#import <errno.h>
#include "platform/platform.h"
#include "platform/threads/semaphore.h"

//-----------------------------------------------------------------------------

struct PlatformSemaphore
{
    pthread_mutex_t mDarkroom;
    pthread_cond_t  mCond;
    S32 count;
};

//-----------------------------------------------------------------------------

Semaphore::Semaphore(S32 initialCount)
{
    bool ok;

    // Create the semaphore data.
    mData = new PlatformSemaphore;
    
    // Initialize the semaphore.
    ok = pthread_mutex_init(&mData->mDarkroom,NULL);
    
    // Sanity!
    AssertFatal(ok == 0,"Create semaphore failed at creating mutex mDarkroom.");

    // Initialize the condition variables.
    ok = pthread_cond_init(&mData->mCond,NULL);
    
    // Sanity!
    AssertFatal(ok == 0,"Create semaphore failed at creating condition mCond.");
    
    // Set the initial semaphore count.
    mData->count = initialCount;
}

//-----------------------------------------------------------------------------

Semaphore::~Semaphore()
{
    // Destroy the system semaphore.
    pthread_mutex_destroy(&mData->mDarkroom);
    pthread_cond_destroy(&mData->mCond);
    
    // Destroy the semaphore data.
    delete mData;
}

//-----------------------------------------------------------------------------

bool Semaphore::acquire( bool block, S32 timeoutMS )
{
    // Sanity!
    AssertFatal(mData, "Semaphore::acquireSemaphore: invalid semaphore");
    
    // Create the system mutex.
    bool ok;
    ok = pthread_mutex_lock(&mData->mDarkroom);
    
    // Sanity!
    AssertFatal(ok == 0,"Mutex Lock failed on mDarkroom in acquireSemaphore().");
    
    // Should we unlock the mutex?
    if( mData->count <= 0 && !block )
    {
        // Yes, so unlock it.
        ok = pthread_mutex_unlock(&mData->mDarkroom);
        
        // Sanity!
        AssertFatal(ok == 0,"Mutex Unlock failed on mDarkroom when not blocking in acquireSemaphore().");
        return false;
    }
    
    while( mData->count <= 0 )
    {
        ok = pthread_cond_wait(&mData->mCond, &mData->mDarkroom); // releases mDarkroom while blocked.
        AssertFatal(ok == 0,"Waiting on mCond failed in acquireSemaphore().");
    }

    // Reduce the semaphore count.
    mData->count--;
    
    // Unlock the mutex.
    ok = pthread_mutex_unlock(&mData->mDarkroom);
    
    // Sanity!
    AssertFatal(ok == 0,"Mutex Unlock failed on mDarkroom when leaving acquireSemaphore().");
    
    return true;
}

//-----------------------------------------------------------------------------

void Semaphore::release()
{
    // Sanity!
    AssertFatal(mData, "Semaphore::releaseSemaphore: invalid semaphore");
    
    // Lock the mutex.
    bool ok;
    ok = pthread_mutex_lock(&mData->mDarkroom);
    
    // Sanity!
    AssertFatal(ok == 0,"Mutex Lock failed on mDarkroom in releaseSemaphore().");
    
    // Increase the semaphore count.
    mData->count++;
    if(mData->count > 0)
    {
        ok = pthread_cond_signal(&mData->mCond);
        
        // Sanity!
        AssertFatal(ok == 0,"Signaling mCond failed in releaseSemaphore().");
    }
    
    // Unlock the mutex.
    ok = pthread_mutex_unlock(&mData->mDarkroom);
    
    // Sanity!
    AssertFatal(ok == 0,"Mutex Unlock failed on mDarkroom when leaving releaseSemaphore().");
}
