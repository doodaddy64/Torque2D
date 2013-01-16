//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/assets/ParticleAssetFieldCollection.h"

#ifndef _PARTICLE_ASSET_FIELD_H_
#include "2d/assets/particleAssetField.h"
#endif

//-----------------------------------------------------------------------------

ParticleAssetFieldCollection::ParticleAssetFieldCollection() :
                                    mpSelectedField( NULL )
{
}

//-----------------------------------------------------------------------------

ParticleAssetFieldCollection::~ParticleAssetFieldCollection()
{
}

//------------------------------------------------------------------------------

void ParticleAssetFieldCollection::copyTo( ParticleAssetFieldCollection& fieldCollection )
{

    // Iterate field.
    for( typeFieldHash::iterator fieldItr = mFields.begin(); fieldItr != mFields.end(); ++fieldItr )
    {
        // Fetch field.
        ParticleAssetField* pParticleAssetField = fieldItr->value;

        // Find target field.
        ParticleAssetField* pTargetParticleAssetField = fieldCollection.findField( fieldItr->key );

        // Sanity!
        AssertFatal( pTargetParticleAssetField != NULL, "ParticleAssetFieldCollection::copyTo() - Could not find target particle asset field." );

        // Copy field.
        pParticleAssetField->copyTo( *pTargetParticleAssetField );
    }

    // Select the same field name.
    if ( getSelectedField() != NULL )
        fieldCollection.setSelectedField( getSelectedField()->getFieldName() );
}

//------------------------------------------------------------------------------

void ParticleAssetFieldCollection::addField( ParticleAssetField& particleAssetField )
{
    // Sanity!
    AssertFatal( !mFields.contains( particleAssetField.getFieldName() ), "ParticleAssetFieldCollection::addField() - The particle field name already exists." );

    // Add to fields.
    mFields.insert( particleAssetField.getFieldName(), &particleAssetField );
}

//------------------------------------------------------------------------------

void ParticleAssetFieldCollection::addField( ParticleAssetField& particleAssetField, const char* pFieldName, F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue )
{
    // Sanity!
    AssertFatal( pFieldName != NULL && pFieldName != StringTable->EmptyString, "ParticleAssetFieldCollection::addField() - Field name cannot be NULL or empty." );

    // Set the field name.
    particleAssetField.setFieldName( pFieldName );

    // Sanity!
    AssertFatal( !mFields.contains( particleAssetField.getFieldName() ), "ParticleAssetFieldCollection::addField() - The particle field name already exists." );

    // Add to fields.
    mFields.insert( particleAssetField.getFieldName(), &particleAssetField );

    // Set value bounds.
    particleAssetField.setValueBounds( maxTime, minValue, maxValue, defaultValue );
}

//-----------------------------------------------------------------------------

ParticleAssetField* ParticleAssetFieldCollection::findField( const char* pFieldName )
{
    // Sanity!
    AssertFatal( pFieldName != NULL, "ParticleAssetFieldCollection::findField() - Cannot find NULL field." );

    // Find the field.
    typeFieldHash::iterator fieldItr = mFields.find( StringTable->insert( pFieldName ) );

    // Return the field if it was found.
    return fieldItr == mFields.end() ? NULL : fieldItr->value;
}

//-----------------------------------------------------------------------------

S32 ParticleAssetFieldCollection::addDataKey( F32 time, F32 value )
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::addDataKey() - No field selected." );
      return -1;
   }

   // Add Data Key.
   return mpSelectedField->addDataKey( time, value );
}

//-----------------------------------------------------------------------------

bool ParticleAssetFieldCollection::removeDataKey( S32 index )
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::removeDataKey() - No field selected." );
      return false;
   }

   // Remove Data Key.
   return mpSelectedField->removeDataKey( index );
}

//-----------------------------------------------------------------------------

bool ParticleAssetFieldCollection::clearDataKeys( void )
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::clearDataKeys() - No field selected." );
      return false;
   }

   // Clear Data Keys
   mpSelectedField->clearDataKeys();

   // Return Okay.
   return true;
}

//-----------------------------------------------------------------------------

bool ParticleAssetFieldCollection::setDataKey( S32 index, F32 value )
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::setDataKey() - No field selected." );
      return false;
   }

   // Set Data Key.
   return mpSelectedField->setDataKeyValue( index, value );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getDataKeyValue( S32 index ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getDataKeyValue() - No field selected." );
      return 0.0f;
   }

   // Get Data Key Value.
   return mpSelectedField->getDataKeyValue( index );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getDataKeyTime( S32 index ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getDataKeyTime() - No field selected." );
      return 0.0f;
   }

   // Get Data Key Time.
   return mpSelectedField->getDataKeyTime( index );
}

//-----------------------------------------------------------------------------

U32 ParticleAssetFieldCollection::getDataKeyCount( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getDataKeyCount() - No field selected." );
      return 0;
   }

   // Get Data Key Count.
   return mpSelectedField->getDataKeyCount();
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getMinValue( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getMinValue() - No field selected." );
      return 0.0f;
   }

   // Get Min Value.
   return mpSelectedField->getMinValue();
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getMaxValue( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getMaxValue() - No field selected." );
      return 0.0f;
   }

   // Get Max Value.
   return mpSelectedField->getMaxValue();
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getMinTime( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getMinTime() - No field selected." );
      return 0.0f;
   }

   // Get Min Time.
   return mpSelectedField->getMinTime();
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getMaxTime( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getMaxTime() - No field selected." );
      return 0.0f;
   }

   // Get Max Time.
   return mpSelectedField->getMaxTime();
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getFieldValue( F32 time ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getFieldValue() - No field selected." );
      return 0.0f;
   }

   // Get Graph Value.
   return mpSelectedField->getFieldValue( time );
}

//-----------------------------------------------------------------------------

bool ParticleAssetFieldCollection::setTimeRepeat( const F32 timeRepeat )
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::setTimeRepeat() - No field selected." );
      return false;
   }

   // Set Time Repeat.
   return mpSelectedField->setTimeRepeat( timeRepeat );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getTimeRepeat( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getTimeRepeat() - No field selected." );
      return 0.0f;
   }

   // Get Time Repeat.
   return mpSelectedField->getTimeRepeat();
}

//-----------------------------------------------------------------------------

bool ParticleAssetFieldCollection::setValueScale( const F32 valueScale )
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::setValueScale() - No field selected." );
      return false;
   }

   // Set Value Scale.
   return mpSelectedField->setValueScale( valueScale );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getValueScale( void ) const
{
   // Have we got a valid field selected?
   if ( !mpSelectedField )
   {
      // No, so warn.
      Con::warnf( "ParticleAssetFieldCollection::getValueScale() - No field selected." );
      return 0.0f;
   }

   // Get Value Scale.
   return mpSelectedField->getValueScale();
}




