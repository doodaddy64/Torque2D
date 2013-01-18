//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _TAML_COLLECTION_H_
#define _TAML_COLLECTION_H_

#ifndef _FACTORY_CACHE_H_
#include "memory/factoryCache.h"
#endif

#ifndef _STRINGTABLE_H_
#include "string/stringTable.h"
#endif

#ifndef _CONSOLE_H_
#include "console/console.h"
#endif

#ifndef B2_MATH_H
#include "box2d/Common/b2Math.h"
#endif

#ifndef _COLOR_H_
#include "graphics/color.h"
#endif

#ifndef _SIMBASE_H_
#include "sim/simBase.h"
#endif

#include "memory/safeDelete.h"

//-----------------------------------------------------------------------------

#define MAX_TAML_PROPERTY_FIELDVALUE_LENGTH 2048

//-----------------------------------------------------------------------------

class TamlWriteNode;

//-----------------------------------------------------------------------------

class TamlPropertyField : public IFactoryObjectReset
{
public:
    TamlPropertyField()
    {
        // Reset field object.
        // NOTE: This MUST be done before the state is reset otherwise we'll be touching uninitialized stuff.
        mpFieldWriteNode = NULL;
        mpFieldObject = NULL;

        resetState();
    }

    virtual ~TamlPropertyField()
    {
        // Everything should already be cleared in a state reset.
        // Touching any memory here is dangerous as this type is typically
        // held in a static factory cache until shutdown at which point
        // pretty much anything or everything could be invalid!
    }

    virtual void resetState( void );

    void set( const char* pFieldName, const char* pFieldValue );

    void set( const char* pFieldName, SimObject* pFieldObject );

    void setWriteNode( TamlWriteNode* pWriteNode );

    inline void getFieldValue( ColorF& fieldValue ) const
    {
        fieldValue.set( 1.0f, 1.0f, 1.0f, 1.0f );

        // Set color.
        const char* argv = (char*)mFieldValue;
        Con::setData( TypeColorF, &fieldValue, 0, 1, &argv );
    }

    inline void getFieldValue( ColorI& fieldValue ) const
    {
        fieldValue.set( 255, 255, 255, 255 );

        // Set color.
        const char* argv = (char*)mFieldValue;
        Con::setData( TypeColorI, &fieldValue, 0, 1, &argv );
    }

    inline void getFieldValue( Point2I& fieldValue ) const
    {
        if ( dSscanf( mFieldValue, "%d %d", &fieldValue.x, &fieldValue.y ) != 2 )
        {
            // Warn.
            Con::warnf( "TamlPropertyField - Reading point2I but it has an incorrect format: '%s'.", mFieldValue );
        }
    }

    inline void getFieldValue( Point2F& fieldValue ) const
    {
        if ( dSscanf( mFieldValue, "%g %g", &fieldValue.x, &fieldValue.y ) != 2 )
        {
            // Warn.
            Con::warnf( "TamlPropertyField - Reading point2F but it has an incorrect format: '%s'.", mFieldValue );
        }
    }

    inline void getFieldValue( b2Vec2& fieldValue ) const
    {
        if ( dSscanf( mFieldValue, "%g %g", &fieldValue.x, &fieldValue.y ) != 2 )
        {
            // Warn.
            Con::warnf( "TamlPropertyField - Reading vector but it has an incorrect format: '%s'.", mFieldValue );
        }
    }

    inline void getFieldValue( bool& fieldValue ) const
    {
        fieldValue = dAtob( mFieldValue );
    }

    inline void getFieldValue( S32& fieldValue ) const
    {
        fieldValue = dAtoi( mFieldValue );
    }

    inline void getFieldValue( U32& fieldValue ) const
    {
        fieldValue = (U32)dAtoi( mFieldValue );
    }

    inline void getFieldValue( F32& fieldValue ) const
    {
        fieldValue = dAtof( mFieldValue );
    }

    inline const char* getFieldValue( void ) const
    {
        return mFieldValue;
    }

    SimObject* getFieldObject( void ) const;

    inline const TamlWriteNode* getWriteNode( void ) const { return mpFieldWriteNode; }

    bool isObjectField( void ) const;

    inline StringTableEntry getFieldName( void ) const { return mFieldName; }

    bool fieldNameBeginsWith( const char* pComparison )
    {
        const U32 comparisonLength = dStrlen( pComparison );
        const U32 fieldNameLength = dStrlen( mFieldName );

        if ( comparisonLength == 0 || fieldNameLength == 0 || comparisonLength > fieldNameLength )
            return false;

        StringTableEntry comparison = StringTable->insert( pComparison );

        char fieldNameBuffer[1024];

        // Sanity!
        AssertFatal( fieldNameLength < sizeof(fieldNameBuffer), "TamlPropertyField: Field name is too long." );

        dStrcpy( fieldNameBuffer, mFieldName );
        fieldNameBuffer[fieldNameLength-1] = 0;
        StringTableEntry fieldName = StringTable->insert( fieldNameBuffer );

        return ( fieldName == comparison );
    }

private:
    StringTableEntry    mFieldName;
    char                mFieldValue[MAX_TAML_PROPERTY_FIELDVALUE_LENGTH];
    SimObject*          mpFieldObject;
    TamlWriteNode*      mpFieldWriteNode;
};

static FactoryCache<TamlPropertyField> TamlPropertyFieldFactory;

//-----------------------------------------------------------------------------

typedef Vector<TamlPropertyField*> TamlPropertyFieldVector;

class TamlPropertyTypeAlias :
    public TamlPropertyFieldVector,
    public IFactoryObjectReset
{
public:
    TamlPropertyTypeAlias()
    {
        resetState();
    }

    virtual ~TamlPropertyTypeAlias()
    {
        // Everything should already be cleared in a state reset.
        // Touching any memory here is dangerous as this type is typically
        // held in a static factory cache until shutdown at which point
        // pretty much anything or everything could be invalid!
    }

    virtual void resetState( void )
    {
        while( size() > 0 )
        {
            TamlPropertyFieldFactory.cacheObject( back() );
            pop_back();
        }

        mAliasName = StringTable->EmptyString;
        mIgnoreEmpty = false;
    }

    void set( const char* pAliasName )
    {
        // Sanity!
        AssertFatal( pAliasName != NULL, "Type alias cannot be NULL." );

        mAliasName = StringTable->insert( pAliasName );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const ColorI& fieldValue )
    {
        // Fetch the field value.
        const char* pFieldValue = Con::getData( TypeColorI, &const_cast<ColorI&>(fieldValue), 0 );

        // Did we get a field value?
        if ( pFieldValue == NULL )
        {
            // No, so warn.
            Con::warnf( "Taml: Failed to add property field name '%s' with ColorI value.", pFieldName );
            pFieldValue = StringTable->EmptyString;
        }

        return addPropertyField( pFieldName, pFieldValue );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const ColorF& fieldValue )
    {
        // Fetch the field value.
        const char* pFieldValue = Con::getData( TypeColorF, &const_cast<ColorF&>(fieldValue), 0 );

        // Did we get a field value?
        if ( pFieldValue == NULL )
        {
            // No, so warn.
            Con::warnf( "Taml: Failed to add property field name '%s' with ColorF value.", pFieldName );
            pFieldValue = StringTable->EmptyString;
        }

        return addPropertyField( pFieldName, pFieldValue );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const Point2I& fieldValue )
    {
        char fieldValueBuffer[32];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%d %d", fieldValue.x, fieldValue.y );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const Point2F& fieldValue )
    {
        char fieldValueBuffer[32];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%.5g %0.5g", fieldValue.x, fieldValue.y );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const b2Vec2& fieldValue )
    {
        char fieldValueBuffer[32];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%.5g %.5g", fieldValue.x, fieldValue.y );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const U32 fieldValue )
    {
        char fieldValueBuffer[16];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%d", fieldValue );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const bool fieldValue )
    {
        char fieldValueBuffer[16];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%d", fieldValue );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const S32 fieldValue )
    {
        char fieldValueBuffer[16];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%d", fieldValue );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const float fieldValue )
    {
        char fieldValueBuffer[16];
        dSprintf( fieldValueBuffer, sizeof(fieldValueBuffer), "%.5g", fieldValue );
        return addPropertyField( pFieldName, fieldValueBuffer );
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, const char* pFieldValue )
    {
        // Create a property field.
        TamlPropertyField* pPropertyField = TamlPropertyFieldFactory.createObject();

        // Set property field.
        pPropertyField->set( pFieldName, pFieldValue );

#if TORQUE_DEBUG
        // Ensure a field name conflict does not exist.
        for( Vector<TamlPropertyField*>::iterator propertyFieldItr = begin(); propertyFieldItr != end(); ++propertyFieldItr )
        {
            // Skip if field name is not the same.
            if ( pPropertyField->getFieldName() != (*propertyFieldItr)->getFieldName() )
                continue;

            // Warn!
            Con::warnf("Conflicting Taml property field name of '%s' in property type alias of '%s'.", pFieldName, mAliasName );

            // Cache property field.
            TamlPropertyFieldFactory.cacheObject( pPropertyField );
            return NULL;
        }

        // Ensure the field value is not too long.
        if ( dStrlen( pFieldValue ) >= MAX_TAML_PROPERTY_FIELDVALUE_LENGTH )
        {
            // Warn.
            Con::warnf("Taml field name '%s' has a field value that is too long (Max:%d): '%s'.",
                pFieldName,
                MAX_TAML_PROPERTY_FIELDVALUE_LENGTH,
                pFieldValue );

            // Cache property field.
            TamlPropertyFieldFactory.cacheObject( pPropertyField );
            return NULL;
        }
#endif
        // Store property field.
        push_back( pPropertyField );

        return pPropertyField;
    }

    TamlPropertyField* addPropertyField( const char* pFieldName, SimObject* pFieldObject )
    {
        // Create a property field.
        TamlPropertyField* pPropertyField = TamlPropertyFieldFactory.createObject();

        // Set property field.
        pPropertyField->set( pFieldName, pFieldObject );

#if TORQUE_DEBUG
        // Ensure a field name conflict does not exist.
        for( TamlPropertyFieldVector::iterator propertyFieldItr = begin(); propertyFieldItr != end(); ++propertyFieldItr )
        {
            // Skip if field name is not the same.
            if ( pPropertyField->getFieldName() != (*propertyFieldItr)->getFieldName() )
                continue;

            // Warn!
            Con::warnf("Conflicting Taml property field name of '%s' in property type alias of '%s'.", pFieldName, mAliasName );

            // Cache property field.
            TamlPropertyFieldFactory.cacheObject( pPropertyField );
            return NULL;
        }
#endif
        // Store property field.
        push_back( pPropertyField );

        return pPropertyField;
    }

    const TamlPropertyField* findField( const char* pFieldName ) const
    {
        // Sanity!
        AssertFatal( pFieldName != NULL, "Cannot find Taml field name that is NULL." );

        // Fetch field name.
        StringTableEntry fieldName = StringTable->insert( pFieldName );

        // Find property field.
        for( TamlPropertyFieldVector::const_iterator fieldItr = begin(); fieldItr != end(); ++fieldItr )
        {
            if ( (*fieldItr)->getFieldName() == fieldName )
                return (*fieldItr);
        }

        return NULL;
    }

    StringTableEntry    mAliasName;
    bool                mIgnoreEmpty;
};

static FactoryCache<TamlPropertyTypeAlias> TamlPropertyTypeAliasFactory;

//-----------------------------------------------------------------------------

typedef Vector<TamlPropertyTypeAlias*> TamlPropertyTypeAliasVector;

class TamlCollectionProperty :
    public TamlPropertyTypeAliasVector,
    public IFactoryObjectReset
{
public:
    TamlCollectionProperty()
    {
    }

    virtual ~TamlCollectionProperty()
    {
        // Everything should already be cleared in a state reset.
        // Touching any memory here is dangerous as this type is typically
        // held in a static factory cache until shutdown at which point
        // pretty much anything or everything could be invalid!
    }

    virtual void resetState( void )
    {
        while( size() > 0 )
        {
            TamlPropertyTypeAliasFactory.cacheObject( back() );
            pop_back();
        }
        mIgnoreEmpty = true;
    }

    void set( const char* pPropertyName )
    {
        // Sanity!
        AssertFatal( pPropertyName != NULL, "TamlCollectionProperty::set() - Property name cannot be NULL." );

        mPropertyName = StringTable->insert( pPropertyName );
    }

    TamlPropertyTypeAlias* addTypeAlias( const char* pAliasName, const bool ignoreEmpty = false )
    {
        // Create a type alias.
        TamlPropertyTypeAlias* pTypeAlias = TamlPropertyTypeAliasFactory.createObject();

        // Set alias name.
        pTypeAlias->set( pAliasName );

        // Set ignore-empty flag.
        pTypeAlias->mIgnoreEmpty = ignoreEmpty;

        // Store type alias.
        push_back( pTypeAlias );

        return pTypeAlias;
    }

    void removeTypeAlias( const U32 index )
    {
        // Sanity!
        AssertFatal( index < (U32)size(), "TamlCollectionProperty::removeTypeAlias() - Index is out of bounds." );

        // Cache the type alias.
        TamlPropertyTypeAliasFactory.cacheObject( at(index) );

        // Remove it from the collection.
        erase( index );
    }

    StringTableEntry mPropertyName;
    bool mIgnoreEmpty;
};

static FactoryCache<TamlCollectionProperty> TamlCollectionPropertyFactory;

//-----------------------------------------------------------------------------

typedef Vector<TamlCollectionProperty*> TamlCollectionPropertyVector;

class TamlCollection :
    public TamlCollectionPropertyVector,
    public IFactoryObjectReset
{
public:
    TamlCollection()
    {
    }

    virtual ~TamlCollection()
    {
        resetState();
    }

    virtual void resetState( void )
    {
        while( size() > 0 )
        {
            TamlCollectionPropertyFactory.cacheObject( back() );
            pop_back();
        }
    }

    TamlCollectionProperty* addCollectionProperty( const char* pPropertyName, const bool ignoreEmpty = true )
    {
        // Create a collection property.
        TamlCollectionProperty* pCollectionProperty = TamlCollectionPropertyFactory.createObject();

        // Set property name.
        pCollectionProperty->set( pPropertyName );

        // Set ignore-empty flag.
        pCollectionProperty->mIgnoreEmpty = ignoreEmpty;

#if TORQUE_DEBUG
        // Ensure an property name conflict does not exist.
        for( TamlCollectionPropertyVector::iterator propertyItr = begin(); propertyItr != end(); ++propertyItr )
        {
            // Skip if property name is not the same.
            if ( pCollectionProperty->mPropertyName != (*propertyItr)->mPropertyName )
                continue;

            // Warn!
            Con::warnf("Conflicting Taml property name of '%s' in collection.", pPropertyName );

            // Cache property.
            TamlCollectionPropertyFactory.cacheObject( pCollectionProperty );
            return NULL;
        }
#endif
        // Store property.
        push_back( pCollectionProperty );

        return pCollectionProperty;
    }

    void removeCollectionProperty( const U32 index )
    {
        // Sanity!
        AssertFatal( index < (U32)size(), "TamlCollectionProperty::removeCollectionProperty() - Index is out of bounds." );

        // Cache the collection property.
        TamlCollectionPropertyFactory.cacheObject( at(index) );

        // Remove it from the collection.
        erase( index );
    }

    const TamlCollectionProperty* findProperty( const char* pPropertyName ) const
    {
        // Sanity!
        AssertFatal( pPropertyName != NULL, "Cannot find Taml property name that is NULL." );

        // Fetch property name.
        StringTableEntry propertyName = StringTable->insert( pPropertyName );

        // Find property.
        for( Vector<TamlCollectionProperty*>::const_iterator propertyItr = begin(); propertyItr != end(); ++propertyItr )
        {
            if ( (*propertyItr)->mPropertyName == propertyName )
                return (*propertyItr);
        }

        return NULL;
    }
};

#endif // _TAML_COLLECTION_H_