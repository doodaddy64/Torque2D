//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _NETWORK_PROCESS_LIST_H_
#define _NETWORK_PROCESS_LIST_H_

#ifndef _SIM_EVENT_H_
#include "sim/simEvent.h"
#endif

//-----------------------------------------------------------------------------

class NetworkProcessList
{
public:
    NetworkProcessList( const bool isServer );
    inline SimTime getLastTime( void ) const { return mLastTime; }
    inline void markDirty( void ) { mDirty = true; }
    inline bool isDirty( void ) const { return mDirty; }
    bool advanceTime( const SimTime& timeDelta );

private:
    U32 mCurrentTag;
    SimTime mLastTick;
    SimTime mLastTime;
    SimTime mLastDelta;
    bool mIsServer;
    bool mDirty;
};

#endif // _NETWORK_PROCESS_LIST_H_
