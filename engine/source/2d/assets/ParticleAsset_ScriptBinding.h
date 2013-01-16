//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, createEmitter, S32, 2, 2,  "() - Creates and add a new emitter.\n"
                                                        "@return The new emitter that was added or 0 if failed.")
{
    // Find the emitter.
    ParticleAssetEmitter* pEmitter = Sim::findObject<ParticleAssetEmitter>( argv[2] );

    // Did we find the emitter?
    if ( pEmitter == NULL )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::addEmitter() - Could not find the emitter '%s'.", argv[2] );
        return false;
    }

    return object->addEmitter(pEmitter);
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, addEmitter, bool, 3, 3,    "(emitterId) - Adds an existing emitter.\n"
                                                        "@param emitterId The emitter to add.\n"
                                                        "@return On success it returns the ID of the emitter, or 0 if failed.")
{
    // Find the emitter.
    ParticleAssetEmitter* pEmitter = Sim::findObject<ParticleAssetEmitter>( argv[2] );

    // Did we find the emitter?
    if ( pEmitter == NULL )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::addEmitter() - Could not find the emitter '%s'.", argv[2] );
        return false;
    }

    return object->addEmitter(pEmitter);
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, removeEmitter, bool, 3, 4, "(emitterId, [bool deleteEmitter]) - Removes an emitter.\n"
                                                        "@param emitterId The emitter to remove.\n"
                                                        "@return No return value.")
{
    // Find the emitter.
    ParticleAssetEmitter* pEmitter = Sim::findObject<ParticleAssetEmitter>( argv[2] );

    // Did we find the emitter?
    if ( pEmitter == NULL )
    {
        // No, so warn.
        Con::warnf( "ParticleAsset::removeEmitter() - Could not find the emitter '%s'.", argv[2] );
        return false;
    }

    bool deleteEmitter = true;
    if (argc > 3)
        deleteEmitter = dAtob(argv[3]);

    // Remove the emitter.
    object->removeEmitter( pEmitter, deleteEmitter );

    return true;
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, clearEmitters, void, 2, 2, "() Clear all the emitters.\n"
                                                        "@return No return Value.")
{
   // Clear Emitters.
   object->clearEmitters();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, getEmitterCount, S32, 2, 2,    "() Gets the emitter count.\n"
                                                            "@return Returns the number of emitters as an integer.")
{
   // Get Emitter Count.
   return object->getEmitterCount();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, getEmitter, S32, 3, 3,     "(emitterIndex) Gets the emitter at the specified index.\n"
                                                        "@param emitterIndex The index for the desired emitter\n"
                                                        "@return The emitter or 0 if not found.")
{
   // Get the emitter.
   ParticleAssetEmitter* pEmitter = object->getEmitter( dAtoi(argv[2]) );

   return pEmitter == NULL ? 0 : pEmitter->getId();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, findEmitterObject, S32, 3, 3, "(emitterName) Finds the emitter by its name.\n"
                                                            "@param emitterName The name of the desired emitter.\n"
                                                            "@return The emitter or 0 if not found.")
{
   // Find Emitter Object.
   ParticleAssetEmitter* pEmitter = object->findEmitter( argv[2] );

   return pEmitter == NULL ? 0 : pEmitter->getId();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAsset, moveEmitter, void, 4, 4, "(fromEmitterIndex, toEmitterIndex) Moves the emitter order.\n"
              "@param fromEmitterIndex The source index of the emitter to move.\n"
              "@param toEmitterIndex The destination index to move the emitter to.\n"
              "@return No return value.")
{
   // Move Emitter Object.
   object->moveEmitter( dAtoi(argv[2]), dAtoi(argv[3]) );
}
