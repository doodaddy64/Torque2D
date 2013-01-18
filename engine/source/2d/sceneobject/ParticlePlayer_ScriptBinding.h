//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, setParticleAsset, void, 3, 3,     "(particleAssetId?) Sets the particle asset Id to play.\n"
                                                                "@param particleAssetId The particle asset Id to play.\n"
                                                                "@return No return value.")
{
    object->setParticle( argv[2] );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, getParticleAsset, const char*, 2, 2,  "() Gets the particle asset Id.\n"
                                                                    "@return The particle asset Id.")
{
    return object->getParticle();
}

//-----------------------------------------------------------------------------


ConsoleMethod(ParticlePlayer, setCameraIdleDistance, void, 3, 3,    "(idleDistance]) Sets the distance from any camera when the particle player will become idle i.e. stop integrating and rendering.\n"
                                                                    "@param pauseDistance The distance from any camera when the particle player will become idle i.e. stop integrating and rendering.\n"
                                                                    "@return No return value.")
{
    object->setCameraIdleDistance( dAtof(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, getCameraIdleDistance, F32, 2, 2,     "() Gets the distance from any camera when the particle player will become idle i.e. stop integrating and rendering.\n"
                                                                    "@return The distance from any camera when the particle player will become idle i.e. stop integrating and rendering.")
{
    return object->getCameraIdleDistance();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, play, bool, 2, 3, "([resetParticles]) Starts the particle player playing.\n"
                                                "@param resetParticles Whether to reset any existing particles before starting to play.  Default is true.\n"
                                                "@return Returns true on success and false otherwise.")
{
    // Fetch the reset-particle flag.
    const bool resetParticles = argc >= 3 ? dAtob(argv[2]) : true;

    return object->play( resetParticles );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, stop, void, 2, 4, "([waitForParticles?, killEffect?]) - Stops the Particle Effect.\n"
              "@param waitForParticles Whether or not the effect should wait until all of its particles have run their course, or just stop immediately and delete the particles (default true).\n"
              "@param killEffect Whether or not the effect should be deleted after it has stopped (default false).\n"
              "@return No return value.")
{
    // Fetch the wait-for-particles flag.
   const bool waitForParticles = argc >= 3 ? dAtob(argv[2]) : true;

   // Fetch the kill-effect flag.
   const bool killEffect = argc >= 4 ? dAtob(argv[3]) : false;

   // Stop playing.
   object->stop( waitForParticles, killEffect );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, getIsPlaying, bool, 2, 2, "() Gets whether the particle player is playing or not.\n"
                                                        "@return Whether the particle player is playing or not." )
{
   return object->getIsPlaying();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, setPaused, void, 3, 3,    "(paused?) Sets whether the particle player is paused or not.\n"
                                                        "@param paused Whether the particle player is paused or not.\n"
                                                        "@return No return value.")
{
    object->setPaused( dAtob(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, getPaused, bool, 2, 2,    "() Gets whether the particle player is paused or not.\n"
                                                        "@return Whether the particle player is paused or not.")
{
    return object->getPaused();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, setEmitterPaused, void, 4, 4, "(paused, emitterIndex) Sets whether the specified particle emitter is paused or not.\n"
                                                            "@param paused Whether the specified particle emitter is paused or not.\n"
                                                            "@param emitterIndex The index of the emitter to modify.\n"
                                                            "@return No return value.")
{
    object->setEmitterPaused( dAtob(argv[2]), dAtoi(argv[3]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, getEmitterPaused, bool, 3, 3, "(emitterIndex) Gets whether the specified particle emitter is paused or not.\n"
                                                            "@param emitterIndex The index of the emitter to modify.\n"
                                                            "@return Whether the specified particle emitter is paused or not.")
{
    return object->getEmitterPaused( dAtob(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, setEmitterVisible, void, 4, 4,    "(visible, emitterIndex) Sets whether the specified particle emitter is visible or not.\n"
                                                                "@param paused Whether the specified particle emitter is visible or not.\n"
                                                                "@param emitterIndex The index of the emitter to modify.\n"
                                                                "@return No return value.")
{
    object->setEmitterVisible( dAtob(argv[2]), dAtoi(argv[3]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticlePlayer, getEmitterVisible, bool, 3, 3,    "(emitterIndex) Gets whether the specified particle emitter is visible or not.\n"
                                                                "@param emitterIndex The index of the emitter to modify.\n"
                                                                "@return Whether the specified particle emitter is visible or not.")
{
    return object->getEmitterVisible( dAtob(argv[2]) );
}
