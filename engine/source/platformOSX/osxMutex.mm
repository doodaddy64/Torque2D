//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

#import <pthread.h>
#import <stdlib.h>
#import <errno.h>
#import "memory/safeDelete.h"
#import "platform/threads/mutex.h"
#import "platform/threads/thread.h"
#import "platform/platform.h"
#import "platformOSX.h"

//-----------------------------------------------------------------------------

struct PlatformMutexData
{
    pthread_mutex_t   mMutex;
    bool              locked;
    U32         lockedByThread;
};

//-----------------------------------------------------------------------------

Mutex::Mutex()
{
    bool ok;
    
    // Create the mutex data.
    mData = new PlatformMutexData;

    // Initialize the system mutex.
    pthread_mutexattr_t attr;
    ok = pthread_mutexattr_init(&attr);
    ok = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
    ok = pthread_mutex_init(&(mData->mMutex),&attr);
    
    // Sanity!
    AssertFatal(ok == 0, "Mutex() failed: pthread_mutex_init() failed.");
    
    // Set the initial mutex state.
    mData->locked = false;
    mData->lockedByThread = 0;
}

//-----------------------------------------------------------------------------

Mutex::~Mutex()
{
    // Destroy the system mutex.
    const bool ok = pthread_mutex_destroy( &(mData->mMutex) );
    
    // Sanity!
    AssertFatal(ok == 0, "~Mutex() failed: pthread_mutex_destroy() failed.");
    
    // Delete the mutex data.
    SAFE_DELETE( mData );
}

//-----------------------------------------------------------------------------

bool Mutex::lock( bool block )
{
    // Is this a blocking lock?
    if( block )
    {
        // Yes, so block until mutex can be locked.
        const bool ok = pthread_mutex_lock( &(mData->mMutex) );
        
        // Sanity!
        AssertFatal( ok != EINVAL, "Mutex::lockMutex() failed: invalid mutex.");
        AssertFatal( ok != EDEADLK, "Mutex::lockMutex() failed: system detected a deadlock!");
        AssertFatal( ok == 0, "Mutex::lockMutex() failed: pthread_mutex_lock() failed -- unknown reason.");
    }
    else
    {
        // No, so attempt to lock the thread without blocking.
        const bool ok = pthread_mutex_trylock( &(mData->mMutex) );
        // returns EBUSY if mutex was locked by another thread,
        // returns EINVAL if mutex was not a valid mutex pointer,
        // returns 0 if lock succeeded.
        
        // Sanity!
        AssertFatal( ok != EINVAL, "Mutex::lockMutex(non blocking) failed: invalid mutex.");
        
        // Finish if we couldn't lock the mutex.
        if( ok != 0 )
            return false;
        
        AssertFatal( ok == 0, "Mutex::lockMutex(non blocking) failed: pthread_mutex_trylock() failed -- unknown reason.");
    }
    
    // Flag as locked by the current thread.
    mData->locked = true;
    mData->lockedByThread = ThreadManager::getCurrentThreadId();
    return true;
}

//-----------------------------------------------------------------------------

void Mutex::unlock()
{
    // Unlock the thread.
    const bool ok = pthread_mutex_unlock( &(mData->mMutex) );
    
    // Sanity!
    AssertFatal( ok == 0, "Mutex::unlockMutex() failed: pthread_mutex_unlock() failed.");
    
    // Flag as unlocked.
    mData->locked = false;
    mData->lockedByThread = 0;
}
