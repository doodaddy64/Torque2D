//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platformiOS/platformiOS.h"
#include "platform/event.h"
#include "game/gameInterface.h"
#include "platformiOS/iOSEvents.h"
#include <pthread.h>


void Platform::postQuitMessage(const U32 in_quitVal)
{
   platState.quit = true;
   Event quitEvent;
   quitEvent.type = QuitEventType;

   Game->postEvent(quitEvent);
}

void Platform::debugBreak()
{
   Platform::outputDebugString((const char *)"\pDEBUG_BREAK!");
}

void Platform::forceShutdown(S32 returnValue)
{
   exit(returnValue);
}   

void Platform::restartInstance()
{
   // execl() leaves open file descriptors open, that's the main reason it's not
   // used here. We want to start fresh.
   
   if( Game->isRunning() )
   {
      Con::errorf("The game is still running, we cant relaunch now!");
      return;
   }
   // get the path to the torque executable
   CFBundleRef mainBundle =  CFBundleGetMainBundle();
   CFURLRef execURL = CFBundleCopyExecutableURL(mainBundle);
   CFStringRef execString = CFURLCopyFileSystemPath(execURL, kCFURLPOSIXPathStyle);

   // append ampersand so that we can launch without blocking.
   // encase in quotes so that spaces in the path are accepted.
   CFMutableStringRef mut = CFStringCreateMutableCopy(NULL, 0, execString);
   CFStringInsert(mut, 0, CFSTR("\""));
   CFStringAppend(mut, CFSTR("\" & "));
   
   U32 len = CFStringGetMaximumSizeForEncoding(CFStringGetLength(mut), kCFStringEncodingUTF8);
   char *execCString = new char[len+1];
   CFStringGetCString(mut, execCString, len, kCFStringEncodingUTF8);
   execCString[len - 1] = '\0';
   
   Con::printf("---- %s -----",execCString);
   system(execCString);
   delete[] execCString;
}
