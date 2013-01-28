//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _ASSET_QUERY_H_
#include "assetQuery.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

#ifndef _TAML_CUSTOM_H_
#include "persistence/taml/tamlCustom.h"
#endif

// Script bindings.
#include "assetQuery_ScriptBinding.h"

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT( AssetQuery );

//-----------------------------------------------------------------------------

void AssetQuery::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();

    addProtectedField( "Count", TypeS32, 0, &defaultProtectedNotSetFn, &getCount, &writeCount, "Gets the number of results in the asset query." );
}

//-----------------------------------------------------------------------------

void AssetQuery::onTamlCustomWrite( TamlCustomProperties& customProperties )
{
    // Call parent.
    Parent::onTamlCustomWrite( customProperties );

    // Add property.
    TamlCustomProperty* pProperty = customProperties.addProperty( ASSETQUERY_CUSTO_PROPERTY_NAME );

    // Finish if no assets.
    if ( size() == 0 )
        return;

    // Iterate asset.
    for( Vector<StringTableEntry>::iterator assetItr = begin(); assetItr != end(); ++assetItr )
    {
        // Add alias.
        TamlPropertyAlias* pAlias = pProperty->addAlias( ASSETQUERY_TYPE_NAME );

        // Add fields.
        pAlias->addField( ASSETQUERY_ASSETID_FIELD_NAME, *assetItr );
    }
}

//-----------------------------------------------------------------------------

void AssetQuery::onTamlCustomRead( const TamlCustomProperties& customProperties )
{
    // Call parent.
    Parent::onTamlCustomRead( customProperties );

    // Find custom property name.
    const TamlCustomProperty* pProperty = customProperties.findProperty( ASSETQUERY_CUSTO_PROPERTY_NAME );

    // Finish if we don't have a property name.
    if ( pProperty == NULL )
        return;

    // Fetch alias name.
    StringTableEntry typeAliasName = StringTable->insert( ASSETQUERY_TYPE_NAME );

    // Iterate property alias.
    for( TamlCustomProperty::const_iterator propertyAliasItr = pProperty->begin(); propertyAliasItr != pProperty->end(); ++propertyAliasItr )
    {
        // Fetch property alias.
        const TamlPropertyAlias* pAlias = *propertyAliasItr;

        // Skip if an unknown alias name.
        if ( pAlias->mAliasName != typeAliasName )
            continue;

        // Fetch field.
        const TamlPropertyField* pField = pAlias->findField( ASSETQUERY_ASSETID_FIELD_NAME );

        // Do we find the field?
        if ( pField == NULL )
        {
            // No, so warn.
            Con::warnf( "AssetQuery::onTamlCustomRead() - Could not find '%s' field.", ASSETQUERY_ASSETID_FIELD_NAME );
            continue;
        }

        // Store asset.
        push_back( pField->getFieldValue() );
    }
}