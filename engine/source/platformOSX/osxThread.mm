//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#import <pthread.h>
#import <stdlib.h>
#import <errno.h>
#import "memory/safeDelete.h"
#import "platform/threads/thread.h"
#import "platform/platformSemaphore.h"
#import "platform/threads/mutex.h"

//-----------------------------------------------------------------------------

#pragma mark ---- Thread Class Methods ----

struct PlatformThreadData
{
    ThreadRunFunction       mRunFunc;
    void*                   mRunArg;
    Thread*                 mThread;
    Semaphore               mGateway; // default count is 1
    U32                     mThreadID;
};

//-----------------------------------------------------------------------------

static void *ThreadRunHandler( void* arg )
{
    // Fetch the platform thread data.
    PlatformThreadData* mData = reinterpret_cast<PlatformThreadData*>(arg);
    
    // Fetch the thread.
    Thread* thread = mData->mThread;
    mData->mThreadID = ThreadManager::getCurrentThreadId();
    
    // Add the thread.
    ThreadManager::addThread(thread);
    
    // Run the thread against a pool.
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    thread->run(mData->mRunArg);
    [pool drain];
    
    // Release the thread.
    mData->mGateway.release();
    
    // Does the thread want to be auto-deleted?
    if(thread->autoDelete)
    {
        // Yes, so remove the thread.
        ThreadManager::removeThread(thread);
        
        // Delete the thread.
        delete thread;
    }
    
    // This is for pthread.
    return NULL;
}

//-----------------------------------------------------------------------------

Thread::Thread(ThreadRunFunction func, void* arg, bool start_thread, bool autodelete)
{
    mData = new PlatformThreadData;
    mData->mRunFunc = func;
    mData->mRunArg = arg;
    mData->mThread = this;
    mData->mThreadID = 0;
    autoDelete = autodelete;
    
    if(start_thread)
        start();
}

//-----------------------------------------------------------------------------

Thread::~Thread()
{
    stop();
    join();
    
    SAFE_DELETE(mData);
}

//-----------------------------------------------------------------------------

void Thread::start()
{
    if(isAlive())
        return;
    
    // cause start to block out other pthreads from using this Thread,
    // at least until ThreadRunHandler exits.
    mData->mGateway.acquire();
    
    // reset the shouldStop flag, so we'll know when someone asks us to stop.
    shouldStop = false;
    
    pthread_create((pthread_t*)(&mData->mThreadID), NULL, ThreadRunHandler, mData);
}

//-----------------------------------------------------------------------------

bool Thread::join()
{
    if(!isAlive())
        return true;
    
    // not using pthread_join here because pthread_join cannot deal
    // with multiple simultaneous calls.
    mData->mGateway.acquire();
    mData->mGateway.release();
    return true;
}

//-----------------------------------------------------------------------------

void Thread::run(void* arg)
{
    if(mData->mRunFunc)
        mData->mRunFunc(arg);
}

//-----------------------------------------------------------------------------

bool Thread::isAlive()
{
    if(mData->mThreadID == 0)
        return false;
    
    if( mData->mGateway.acquire(false) )
    {
        mData->mGateway.release();
        return false; // we got the lock, it aint alive.
    }
    else
        return true; // we could not get the lock, it must be alive.
}

//-----------------------------------------------------------------------------

U32 Thread::getId()
{
   return mData->mThreadID;
}

#pragma mark ---- ThreadManager Class Methods ----

//-----------------------------------------------------------------------------

U32 ThreadManager::getCurrentThreadId()
{
   return (U32)pthread_self();
}

//-----------------------------------------------------------------------------

bool ThreadManager::compare( U32 threadId_1, U32 threadId_2 )
{
   return (bool)pthread_equal((pthread_t)threadId_1, (pthread_t)threadId_2);
}
