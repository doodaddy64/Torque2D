//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifdef TORQUE_ALLOW_JOURNALING
ConsoleFunction( saveJournal, void, 2, 2, "( namedFile ) Use the saveJournal function to save a new journal of the current game.\n"
                                                                "@param namedFile A full path specifying the file to save this journal to. Usually, journal names end with the extension .jrn.\n"
                                                                "@return No return value.\n"
                                                                "@sa playJournal")
{
   Game->saveJournal(argv[1]);
}

ConsoleFunction( playJournal, void, 2, 3, "( namedFile , doBreak ) Use the playJournal function to play back a journal from namedFile and to optionally break (into an active debugger) after loading the Journal. This allow us to debug engine bugs by reproducing them consistently repeatedly.\n"
                                                                "The journaling system is a vital tool for debugging complex or hard to reproduce engine and script bugs.\n"
                                                                "@param namedFile A full path to a valid journal file. Usually, journal names end with the extension .jrn.\n"
                                                                "@param doBreak A boolean value. If true, the engine will load the journal and then assert a break (to break into an active debugger). If not true, the engine will play back the journal with no break.\n"
                                                                "@return No return value.\n"
                                                                "@sa saveJournal")
{
   bool jBreak = (argc > 2)? dAtob(argv[2]): false;
   Game->playJournal(argv[1],jBreak);
}
#endif //TORQUE_ALLOW_JOURNALING