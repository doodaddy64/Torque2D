//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platformWin32/platformWin32.h"
#include "time.h"

void Platform::sleep(U32 ms)
{
   Sleep(ms);
}

//--------------------------------------
void Platform::getLocalTime(LocalTime &lt)
{
   struct tm *systime;
   time_t long_time;

   time( &long_time );                // Get time as long integer.
   systime = localtime( &long_time ); // Convert to local time.

   lt.sec      = systime->tm_sec;
   lt.min      = systime->tm_min;
   lt.hour     = systime->tm_hour;
   lt.month    = systime->tm_mon;
   lt.monthday = systime->tm_mday;
   lt.weekday  = systime->tm_wday;
   lt.year     = systime->tm_year;
   lt.yearday  = systime->tm_yday;
   lt.isdst    = systime->tm_isdst;
}

U32 Platform::getTime()
{
   time_t long_time;
   time( &long_time );
   return (U32)long_time;
}


U32 Platform::getRealMilliseconds()
{
   return GetTickCount();
}

U32 Platform::getVirtualMilliseconds()
{
   return winState.currentTime;
}

void Platform::advanceTime(U32 delta)
{
   winState.currentTime += delta;
}

