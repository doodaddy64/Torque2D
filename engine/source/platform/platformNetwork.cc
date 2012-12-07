//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _CONSOLE_H_
#include "console/console.h"
#endif

#ifndef _PLATFORM_NETWORK_H_
#include "platformNetwork.h"
#endif

//-----------------------------------------------------------------------------

ConsoleFunction( setNetPort, bool, 2, 2, "(int port)"
                "Set the network port for the game to use.\n"
                "@param The requested port as an integer\n"
                "@return Returns true on success, false on fail")
{
    return Net::openPort(dAtoi(argv[1]));
}

//-----------------------------------------------------------------------------

ConsoleFunction( closeNetPort, void, 1, 1, "()"
   "@brief Closes the current network port\n\n"
   "@ingroup Networking")
{
   Net::closePort();
}
