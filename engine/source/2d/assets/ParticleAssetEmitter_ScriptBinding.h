//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
/// Particle fields.
//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, setSelectedField, bool, 3, 3,   "(fieldName) Select the specified field by its name.\n"
                                                                    "@param fieldName The field name to use for the selection.  Use an empty name to deselect to stop accidental changes.\n"
                                                                    "@return Whether the field was successfully selected or not.")
{
    return object->getParticleFields().setSelectedField( argv[2] ) != NULL;
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getSelectedField, bool, 2, 2,   "() Gets the selected field name or nothing if no field is selected.\n"
                                                                    "@return The selected field name or nothing if no fields is selected.")
{
    // Get the selected field.
    const ParticleAssetField* pParticleAssetField = object->getParticleFields().getSelectedField();

    return pParticleAssetField == NULL ? StringTable->EmptyString : pParticleAssetField->getFieldName();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, addDataKey, S32, 4, 4,  "(time, value) Add Data-Key to Graph.\n"
                                                            "@param time The key time.\n"
                                                            "@param value The value at specified time\n"
                                                            "@return Returns the index of the new data-key or -1 on failure.")
{
    return object->getParticleFields().addDataKey( dAtof(argv[2]), dAtof(argv[3]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, removeDataKey, bool, 3, 3,  "(keyIndex) Remove the data-key from the field.\n"
                                                                "@param keyIndex The index of the data-key you want to remove.\n"
                                                                "@return Whether the operation was successful or not.")
{
   return object->getParticleFields().removeDataKey( dAtoi(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, clearDataKeys, bool, 2, 2,  "() Clears all data-key(s) from the field.\n"
                                                                "@return Whether the operation was successful or not.")
{
   return object->getParticleFields().clearDataKeys();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, setDataKeyValue, bool, 4, 4,    "(keyIndex, value) Set data-key value for the field.\n"
                                                                    "@param keyIndex The index of the key to be modified.\n"
                                                                    "@param value The value to change the key to.\n"
                                                                    "@return Whether the operation was successful or not.")
{
   // Set Data Key.
   return object->getParticleFields().setDataKey( dAtoi(argv[2]), dAtof(argv[3]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getDataKeyCount, S32, 2, 2,     "() Gets the data-key count.\n"
                                                                    "@return The number of data-keys in the currently selected field or -1 if no field is selected.")
{
   // Get Data Key Count.
   return object->getParticleFields().getDataKeyCount();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getDataKey, const char*, 3, 3,  "(keyIndex) Gets the data-key at the specified index from the field.\n"
                                                                    "@param keyIndex The index of the data-key to be retrieved.\n"
                                                                    "@return The data-key comprising both the time and value or nothing if the key is invalid.")
{
   // Fetch the key index.
   const S32 keyIndex = dAtoi(argv[2]);

   // Fetch the data-key.
   const ParticleAssetField::DataKey dataKey = object->getParticleFields().getDataKey( keyIndex );

   // Finish if the data-key is bad.
   if ( dataKey == ParticleAssetField::BadDataKey )
       return StringTable->EmptyString;

   // Create Returnable Buffer.
   char* pBuffer = Con::getReturnBuffer(32);

   // Format Buffer.
   dSprintf(pBuffer, 32, "%f %f", dataKey.mTime, dataKey.mValue );

   // Return buffer.
   return pBuffer;
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getMinValue, F32, 2, 2,     "() Get the minimum value for the field.\n"
                                                                "@return The minimum value for the field or always 0.0 if no field is selected." )
{
   return object->getParticleFields().getMinValue();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getMaxValue, F32, 2, 2,     "() Get the maximum value for the field.\n"
                                                                "@return The maximum value for the field or always 0.0 if no field is selected." )
{
   return object->getParticleFields().getMaxValue();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getMinTime, F32, 2, 2,      "() Get the minimum time for the field.\n"
                                                                "@return The minimum time for the field or always 0.0 if no field is selected." )
{
   return object->getParticleFields().getMinTime();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getMaxTime, F32, 2, 2,      "() Get the maximum time for the field.\n"
                                                                "@return The maximum time for the field or always 0.0 if no field is selected." )
{
   return object->getParticleFields().getMaxTime();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getFieldValue, F32, 3, 3,   "(time) Get the fields' value at the specified time.\n"
                                                                "@param time The time to sample the field value at.\n"
                                                                "@return The fields' value at the specified time or always 0.0 if no field is selected.")
{
   return object->getParticleFields().getFieldValue( dAtof(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, setRepeatTime, bool, 3, 3,  "(repeatTime) Sets the time period to repeat (cycle) the fields' values at.\n"
                                                                "@return Whether the operation was successful or not.")
{
   return object->getParticleFields().setRepeatTime( dAtof(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getRepeatTime, F32, 2, 2,   "() Gets the time period that the fields' value repeat (cycle) at.\n"
                                                                "@return The time period that the fields' value repeat (cycle) at.\n" )
{
   return object->getParticleFields().getRepeatTime();
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, setValueScale, bool, 3, 3,  "(valueScale) Set the scaling of field values retrieved from the field.  This does not alter the actual data-key values.\n"
                                                                "@param valueScale The scale for field values retrieved from the field.\n"
                                                                "@return Whether the operation was successful or not.")
{
   return object->getParticleFields().setValueScale( dAtof(argv[2]) );
}

//-----------------------------------------------------------------------------

ConsoleMethod(ParticleAssetEmitter, getValueScale, F32, 2, 2,   "() Gets the scaling of field values' retrieved from the field.\n"
                                                                "@return The scaling of field values' retrieved from the field." )
{
   return object->getParticleFields().getValueScale();
}
