//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _TORQUEGAME_H_
#define _TORQUEGAME_H_

#ifndef _GAMEINTERFACE_H_
#include "game/gameInterface.h"
#endif

#ifndef _SIM_EVENT_H_
#include "sim/simEvent.h"
#endif

#ifndef _TICKABLE_H_
#include "platform/Tickable.h"
#endif

//-----------------------------------------------------------------------------

class DefaultGame : public GameInterface, public virtual Tickable
{
public:
    virtual bool mainInitialize( int argc, const char **argv );
    virtual void mainLoop( void );
    virtual void mainShutdown( void );

    virtual void gameDeactivate( const bool noRender );
    virtual void gameReactivate( void );

    virtual void textureKill( void );
    virtual void textureResurrect( void );
    virtual void refreshWindow( void );

    virtual void processTick( void );
    virtual void interpolateTick( F32 delta ) {};
    virtual void advanceTime( F32 timeDelta );

    void processQuitEvent();
    void processTimeEvent(TimeEvent *event);
    void processInputEvent(InputEvent *event);
    void processMouseMoveEvent(MouseMoveEvent *event);
    void processScreenTouchEvent(ScreenTouchEvent *event);
    void processConsoleEvent(ConsoleEvent *event);
    void processPacketReceiveEvent(PacketReceiveEvent *event);
    void processConnectedAcceptEvent(ConnectedAcceptEvent *event);
    void processConnectedReceiveEvent(ConnectedReceiveEvent *event);
    void processConnectedNotifyEvent(ConnectedNotifyEvent *event);
};

#endif
