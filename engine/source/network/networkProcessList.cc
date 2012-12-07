//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _NETWORK_PROCESS_LIST_H_
#include "network/networkProcessList.h"
#endif

//--------------------------------------------------------------------------

NetworkProcessList::NetworkProcessList( const bool isServer )
{
   mDirty = false;
   mCurrentTag = 0;
   mLastTick = 0;
   mLastTime = 0;
   mLastDelta = 0;
   mIsServer = isServer;
}

//--------------------------------------------------------------------------

bool NetworkProcessList::advanceTime( const SimTime& timeDelta )
{
    const U32 TickShift = 5;
    const U32 TickMs = 1 << TickShift;
    //const F32 TickSec = (F32)(TickMs) / 1000.0f;
    const U32 TickMask =   (TickMs - 1);
  

    SimTime targetTime = mLastTime + timeDelta;
    SimTime targetTick = targetTime & ~TickMask;

    bool ret = mLastTick != targetTick;

    mLastTime = targetTime;
    return ret;
}
