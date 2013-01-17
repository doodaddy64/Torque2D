//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/assets/ParticleAssetFieldCollection.h"

//-----------------------------------------------------------------------------

static StringTableEntry particleAssetFieldCollectionName;

//-----------------------------------------------------------------------------

ParticleAssetFieldCollection::ParticleAssetFieldCollection() :
                                    mpSelectedField( NULL )
{
    // Set collection name.
    particleAssetFieldCollectionName = StringTable->insert("Fields");
}

//-----------------------------------------------------------------------------

ParticleAssetFieldCollection::~ParticleAssetFieldCollection()
{
}

//------------------------------------------------------------------------------

void ParticleAssetFieldCollection::copyTo( ParticleAssetFieldCollection& fieldCollection )
{
    // Iterate the fields.
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

void ParticleAssetFieldCollection::addField( ParticleAssetField& particleAssetField, const char* pFieldName, F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue )
{
    // Sanity!
    AssertFatal( pFieldName != NULL, "ParticleAssetFieldCollection::addField() - Field name cannot be NULL or empty." );

    // Set the field name.
    particleAssetField.setFieldName( pFieldName );

    // Sanity!
    AssertFatal( !mFields.contains( particleAssetField.getFieldName() ), "ParticleAssetFieldCollection::addField() - The particle field name already exists." );

    // Add to fields.
    mFields.insert( particleAssetField.getFieldName(), &particleAssetField );

    // Initialize the field.
    particleAssetField.initialize( maxTime, minValue, maxValue, defaultValue );
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

    // Set the data-key value.
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

// Get the data-key value.
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

    // Get the data-key time.
    return mpSelectedField->getDataKeyTime( index );
}

//-----------------------------------------------------------------------------

const ParticleAssetField::DataKey& ParticleAssetFieldCollection::getDataKey( const U32 index ) const
{
    // Have we got a valid field selected?
    if ( !mpSelectedField )
    {
        // No, so warn.
        Con::warnf( "ParticleAssetFieldCollection::getDataKey() - No field selected." );
        return ParticleAssetField::BadDataKey;
    }

    // Get the data-key.
    return mpSelectedField->getDataKey( index );
}

//-----------------------------------------------------------------------------

U32 ParticleAssetFieldCollection::getDataKeyCount( void ) const
{
    // Have we got a valid field selected?
    if ( !mpSelectedField )
    {
        // No, so warn.
        Con::warnf( "ParticleAssetFieldCollection::getDataKeyCount() - No field selected." );
        return -1;
    }

    // Get the data-key count.
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

bool ParticleAssetFieldCollection::setRepeatTime( const F32 repeatTime )
{
    // Have we got a valid field selected?
    if ( !mpSelectedField )
    {
        // No, so warn.
        Con::warnf( "ParticleAssetFieldCollection::setRepeatTime() - No field selected." );
        return false;
    }

    // Set Time Repeat.
    return mpSelectedField->setRepeatTime( repeatTime );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetFieldCollection::getRepeatTime( void ) const
{
    // Have we got a valid field selected?
    if ( !mpSelectedField )
    {
        // No, so warn.
        Con::warnf( "ParticleAssetFieldCollection::getRepeatTime() - No field selected." );
        return 0.0f;
    }

    // Get Time Repeat.
    return mpSelectedField->getRepeatTime();
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

//------------------------------------------------------------------------------

void ParticleAssetFieldCollection::onTamlCustomWrite( TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAssetFieldCollection_OnTamlCustomWrite);

    // Finish if there no fields.
    if ( mFields.size() == 0 )
        return;

    // Add particle asset collection property.
    TamlCollectionProperty* pParticleAssetCollectionProperty = customCollection.addCollectionProperty( particleAssetFieldCollectionName );

    // Iterate the fields.
    for( typeFieldHash::iterator fieldItr = mFields.begin(); fieldItr != mFields.end(); ++fieldItr )
    {
        fieldItr->value->onTamlCustomWrite( pParticleAssetCollectionProperty );
    }
}

//-----------------------------------------------------------------------------

void ParticleAssetFieldCollection::onTamlCustomRead( const TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAssetFieldCollection_OnTamlCustomRead);

    // Find the particle asset collection property.
    const TamlCollectionProperty* pParticleAssetCollectionProperty = customCollection.findProperty( particleAssetFieldCollectionName );

    // Finish if we don't have a collection property.
    if ( pParticleAssetCollectionProperty == NULL )
        return;

    // Iterate the collection.
    for( TamlCollectionProperty::const_iterator propertyTypeAliasItr = pParticleAssetCollectionProperty->begin(); propertyTypeAliasItr != pParticleAssetCollectionProperty->end(); ++propertyTypeAliasItr )
    {
        // Fetch property type alias.
        TamlPropertyTypeAlias* pPropertyTypeAlias = *propertyTypeAliasItr;

        // Fetch alias name.
        StringTableEntry aliasName = pPropertyTypeAlias->mAliasName;

        // Find the field.
        ParticleAssetField* pParticleAssetField = findField( aliasName );

        // Did we find the field?
        if ( pParticleAssetField == NULL )
        {
            // No, so warn.
            Con::warnf( "ParticleAssetFieldCollection::onTamlCustomRead() - Cannot find data field '%s'.", aliasName );
            continue;
        }

        // Read the alias.
        pParticleAssetField->onTamlCustomRead( pPropertyTypeAlias );
    }
}


