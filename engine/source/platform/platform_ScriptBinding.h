//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2012
//-----------------------------------------------------------------------------

ConsoleFunction( setMouseLock, void, 2, 2, "( isLocked ) Use the setMouseLock function to un/lock the mouse.\n"
                                                                "@param isLocked A boolean value.\n"
                                                                "@return No return value")
{
    Platform::setMouseLock(dAtob(argv[1]));
}

//-----------------------------------------------------------------------------

ConsoleFunction( getRealTime, S32, 1, 1, "() Use the getRealTime function to the computer time in milliseconds.\n"
                                                                "@return Returns the current real time in milliseconds.\n"
                                                                "@sa getSimTime")
{
    return Platform::getRealMilliseconds();
}

//-----------------------------------------------------------------------------

ConsoleFunction( getLocalTime, const char*, 1, 1,   "() Get the local time\n"
                                                    "@return the local time formatted as: monthday/month/year hour/minute/second\n")
{
    char* buf = Con::getReturnBuffer(128);

    Platform::LocalTime lt;
    Platform::getLocalTime(lt);

    dSprintf(buf, 128, "%d/%d/%d %02d:%02d:%02d",
        lt.monthday,
        lt.month + 1,
        lt.year + 1900,
        lt.hour,
        lt.min,
        lt.sec);

    return buf;
}