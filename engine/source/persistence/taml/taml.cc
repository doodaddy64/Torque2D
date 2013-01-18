//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "taml.h"

#ifndef _TAML_XMLWRITER_H_
#include "persistence/taml/tamlXmlWriter.h"
#endif

#ifndef _TAML_XMLREADER_H_
#include "persistence/taml/tamlXmlReader.h"
#endif

#ifndef _TAML_BINARYWRITER_H_
#include "persistence/taml/tamlBinaryWriter.h"
#endif

#ifndef _TAML_BINARYREADER_H_
#include "persistence/taml/tamlBinaryReader.h"
#endif

#ifndef _FRAMEALLOCATOR_H_
#include "memory/frameAllocator.h"
#endif

#ifndef _CONSOLETYPES_H_
#include "console/consoleTypes.h"
#endif

#ifndef _CONSOLEINTERNAL_H_
#include "console/consoleInternal.h"
#endif

#ifndef _ASSET_FIELD_TYPES_H
#include "assets/assetFieldTypes.h"
#endif

// Script bindings.
#include "taml_ScriptBinding.h"

// Debug Profiling.
#include "debug/profiler.h"

//-----------------------------------------------------------------------------

IMPLEMENT_CONOBJECT( Taml );

//-----------------------------------------------------------------------------

static EnumTable::Enums tamlFormatModeLookup[] =
                {
                { Taml::XmlFormat, "xml" },
                { Taml::BinaryFormat, "binary" },
                };

EnumTable tamlFormatModeTable(sizeof(tamlFormatModeLookup) / sizeof(EnumTable::Enums), &tamlFormatModeLookup[0]);

//-----------------------------------------------------------------------------

Taml::TamlFormatMode Taml::getFormatModeEnum(const char* label)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(tamlFormatModeLookup) / sizeof(EnumTable::Enums)); i++)
    {
        if( dStricmp(tamlFormatModeLookup[i].label, label) == 0)
            return (TamlFormatMode)tamlFormatModeLookup[i].index;
    }

    // Warn.
    Con::warnf( "Taml::getFormatModeEnum() - Invalid format of '%s'.", label );

    return Taml::InvalidFormat;
}

//-----------------------------------------------------------------------------

const char* Taml::getFormatModeDescription(const Taml::TamlFormatMode formatMode)
{
    // Search for Mnemonic.
    for (U32 i = 0; i < (sizeof(tamlFormatModeLookup) / sizeof(EnumTable::Enums)); i++)
    {
        if( tamlFormatModeLookup[i].index == (S32)formatMode )
            return tamlFormatModeLookup[i].label;
    }

    // Warn.
    Con::warnf( "Taml::getFormatModeDescription() - Invalid format mode." );

    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

Taml::Taml() :
    mFormatMode(XmlFormat),
    mBinaryCompression(true),
    mAutoFormat(true),
    mAutoFormatXmlExtension("taml"),    // These are set to string literals because Taml is used in a static scope and the string-table cannot be used like that.
    mAutoFormatBinaryExtension("baml")
{
}

//-----------------------------------------------------------------------------

void Taml::initPersistFields()
{
    // Call parent.
    Parent::initPersistFields();

    addField("Format", TypeEnum, Offset(mFormatMode, Taml), 1, &tamlFormatModeTable, "The read/write format that should be used.");
    addField("BinaryCompression", TypeBool, Offset(mBinaryCompression, Taml), "Whether ZIP compression is used on binary formatting or not.\n");
    addField("AutoFormat", TypeBool, Offset(mAutoFormat, Taml), "Whether the format type is automatically determined by the filename extension or not.\n");
    addField("AutoFormatXmlExtension", TypeString, Offset(mAutoFormatXmlExtension, Taml), "When using auto-format, this is the extension (end of filename) used to detect the XML format.\n");
    addField("AutoFormatBinaryExtension", TypeString, Offset(mAutoFormatBinaryExtension, Taml), "When using auto-format, this is the extension (end of filename) used to detect the BINARY format.\n");
}

//-----------------------------------------------------------------------------

bool Taml::write( SimObject* pSimObject, const char* pFilename )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_Write);

    // Sanity!
    AssertFatal( pSimObject != NULL, "Cannot write a NULL object." );
    AssertFatal( pFilename != NULL, "Cannot write to a NULL filename." );

    char filenameBuffer[1024];
    Con::expandPath( filenameBuffer, sizeof(filenameBuffer), pFilename );

    FileStream stream;

    // File opened?
    if ( !stream.open( filenameBuffer, FileStream::Write ) )
    {
        // No, so warn.
        Con::warnf("Taml::writeFile() - Could not open filename '%s' for write.", filenameBuffer );
        return false;
    }

    // Get the file auto-format mode.
    const TamlFormatMode formatMode = getFileAutoFormatMode( filenameBuffer );

    // Reset the compilation.
    resetCompilation();

    // Write object.
    const bool status = write( stream, pSimObject, formatMode );

    // Close file.
    stream.close();

    // Reset the compilation.
    resetCompilation();

    return status;
}

//-----------------------------------------------------------------------------

SimObject* Taml::read( const char* pFilename )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_Read);

    // Sanity!
    AssertFatal( pFilename != NULL, "Cannot read from a NULL filename." );

    char filenameBuffer[1024];
    Con::expandPath( filenameBuffer, sizeof(filenameBuffer), pFilename );

    FileStream stream;

    // File opened?
    if ( !stream.open( filenameBuffer, FileStream::Read ) )
    {
        // No, so warn.
        Con::warnf("Taml::read() - Could not open filename '%s' for read.", filenameBuffer );
        return NULL;
    }

    // Get the file auto-format mode.
    const TamlFormatMode formatMode = getFileAutoFormatMode( filenameBuffer );

    // Reset the compilation.
    resetCompilation();

    // Write object.
    SimObject* pSimObject = read( stream, formatMode );

    // Close file.
    stream.close();

    // Reset the compilation.
    resetCompilation();

    // Did we generate an object?
    if ( pSimObject == NULL )
    {
        // No, so warn.
        Con::warnf( "Taml::read() - Failed to load an object from the file '%s'.", filenameBuffer );
    }

    return pSimObject;
}

//-----------------------------------------------------------------------------

bool Taml::write( FileStream& stream, SimObject* pSimObject, const TamlFormatMode formatMode )
{
    // Sanity!
    AssertFatal( pSimObject != NULL, "Cannot write a NULL object." );

    // Compile nodes.
    TamlWriteNode* pRootNode = compileObject( pSimObject );

    // Format appropriately.
    switch( formatMode )
    {
        /// Xml.
        case XmlFormat:
        {
            // Create writer.
            TamlXmlWriter writer( this );
            // Write.
            return writer.write( stream, pRootNode );
        }

        /// Binary.
        case BinaryFormat:
        {
            // Create writer.
            TamlBinaryWriter writer( this );
            // Write.
            return writer.write( stream, pRootNode, mBinaryCompression );
        }
        
        /// Invalid.
        case InvalidFormat:
        {
            // Warn.
            Con::warnf("Taml::write() - Cannot write, invalid format.");
            return false;
        }
    }

    // Warn.
    Con::warnf("Taml::write() - Unknown format.");
    return false;
}

//-----------------------------------------------------------------------------

SimObject* Taml::read( FileStream& stream, const TamlFormatMode formatMode )
{
    // Format appropriately.
    switch( formatMode )
    {
        /// Xml.
        case XmlFormat:
        {
            // Create reader.
            TamlXmlReader reader( this );

            // Read.
            return reader.read( stream );
        }

        /// Binary.
        case BinaryFormat:
        {
            // Create reader.
            TamlBinaryReader reader( this );

            // Read.
            return reader.read( stream );
        }
        
        /// Invalid.
        case InvalidFormat:
        {
            // Warn.
            Con::warnf("Taml::read() - Cannot read, invalid format.");
            return NULL;
        }
    }

    // Warn.
    Con::warnf("Taml::read() - Unknown format.");
    return NULL;
}

//-----------------------------------------------------------------------------

void Taml::resetCompilation( void )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_ResetCompilation);

    // Clear compiled nodes.
    for( typeNodeVector::iterator itr = mCompiledNodes.begin(); itr != mCompiledNodes.end(); ++itr )
    {
        // Fetch node.
        TamlWriteNode* pNode = (*itr);

        // Reset node.
        pNode->resetNode();

        // Delete node.
        delete pNode;
    }
    mCompiledNodes.clear();

    // Clear compiled objects.
    mCompiledObjects.clear();

    // Reset master node Id.
    mMasterNodeId = 0;
}

//-----------------------------------------------------------------------------

Taml::TamlFormatMode Taml::getFileAutoFormatMode( const char* pFilename )
{
    // Sanity!
    AssertFatal( pFilename != NULL, "Cannot auto-format using a NULL filename." );

    // Is auto-format active?
    if ( mAutoFormat )
    {
        // Yes, so fetch the extension lengths.
        const U32 xmlExtensionLength = dStrlen( mAutoFormatXmlExtension );
        const U32 binaryExtensionLength = dStrlen( mAutoFormatBinaryExtension );

        // Fetch filename length.
        const U32 filenameLength = dStrlen( pFilename );

        // Fetch end of filename,
        const char* pEndOfFilename = pFilename + filenameLength;

        // Check for the XML format.
        if ( xmlExtensionLength <= filenameLength && dStricmp( pEndOfFilename - xmlExtensionLength, mAutoFormatXmlExtension ) == 0 )
            return Taml::XmlFormat;

        // Check for the Binary format.
        if ( binaryExtensionLength <= filenameLength && dStricmp( pEndOfFilename - xmlExtensionLength, mAutoFormatBinaryExtension ) == 0 )
            return Taml::BinaryFormat;  
    }

    // Use the explicitly specified format mode.
    return mFormatMode;
}

//-----------------------------------------------------------------------------

TamlWriteNode* Taml::compileObject( SimObject* pSimObject )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CompileObject);

    // Sanity!
    AssertFatal( pSimObject != NULL, "Cannot compile a NULL object." );

    // Fetch object Id.
    const SimObjectId objectId = pSimObject->getId();

    // Find a previously compiled node.
    typeCompiledHash::iterator compiledItr = mCompiledObjects.find( objectId );

    // Have we already compiled this?
    if ( compiledItr != mCompiledObjects.end() )
    {
        // Yes, so sanity!
        AssertFatal( mCompiledNodes.size() != 0, "Found a compiled node at the root." );

        // Yes, so fetch node.
        TamlWriteNode* compiledNode = compiledItr->value;

        // Is a reference Id already present?
        if ( compiledNode->mRefId == 0 )
        {
            // No, so allocate one.
            compiledNode->mRefId = ++mMasterNodeId;
        }

        // Create write node.
        TamlWriteNode* pNewNode = new TamlWriteNode();
        pNewNode->set( pSimObject );

        // Set reference node.
        pNewNode->mRefToNode = compiledNode;

        // Push new node.
        mCompiledNodes.push_back( pNewNode );

        return pNewNode;
    }

    // No, so create write node.
    TamlWriteNode* pNewNode = new TamlWriteNode();
    pNewNode->set( pSimObject );

    // Push new node.
    mCompiledNodes.push_back( pNewNode );

    // Insert compiled object.
    mCompiledObjects.insert( objectId, pNewNode );

    // Are there any Taml callbacks?
    if ( pNewNode->mpTamlCallbacks != NULL )
    {
        // Yes, so call it.
        tamlPreWrite( pNewNode->mpTamlCallbacks );
    }

    // Compile static and dynamic fields.
    compileStaticFields( pNewNode );
    compileDynamicFields( pNewNode );

    // Compile children.
    compileChildren( pNewNode );

    // Compile Collections.
    compileCollection( pNewNode );

    // Are there any Taml callbacks?
    if ( pNewNode->mpTamlCallbacks != NULL )
    {
        // Yes, so call it.
        tamlPostWrite( pNewNode->mpTamlCallbacks );
    }

    return pNewNode;
}

//-----------------------------------------------------------------------------

void Taml::compileStaticFields( TamlWriteNode* pTamlWriteNode )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CompileStaticFields);

    // Sanity!
    AssertFatal( pTamlWriteNode != NULL, "Cannot compile static fields on a NULL node." );
    AssertFatal( pTamlWriteNode->mpSimObject != NULL, "Cannot compile static fields on a node with no object." );

    // Fetch object.
    SimObject* pSimObject = pTamlWriteNode->mpSimObject;

    // Fetch field list.
    const AbstractClassRep::FieldList& fieldList = pSimObject->getFieldList();

    // Fetch field count.
    const U32 fieldCount = fieldList.size();

    // Iterate fields.
    for( U32 index = 0; index < fieldCount; ++index )
    {
        // Fetch field.
        const AbstractClassRep::Field* pField = &fieldList[index];

        // Ignore if field not appropriate.
        if( pField->type == AbstractClassRep::DepricatedFieldType ||
            pField->type == AbstractClassRep::StartGroupFieldType ||
            pField->type == AbstractClassRep::EndGroupFieldType)
            continue;

        // Fetch fieldname.
        StringTableEntry fieldName = StringTable->insert( pField->pFieldname );

        // Fetch element count.
        const U32 elementCount = pField->elementCount;

        // Skip if the field should not be written.
        // For now, we only deal with non-array fields.
        if ( elementCount == 1 &&
            pField->writeDataFn != NULL &&
            pField->writeDataFn( pSimObject, fieldName ) == false )
            continue;

        // Iterate elements.
        for( U32 elementIndex = 0; elementIndex < elementCount; ++elementIndex )
        {
            char indexBuffer[8];
            dSprintf( indexBuffer, 8, "%d", elementIndex );

            // Fetch object field value.
            const char* pFieldValue = pSimObject->getPrefixedDataField( fieldName, indexBuffer );

            U32 nBufferSize = dStrlen( pFieldValue ) + 1;
            FrameTemp<char> valueCopy( nBufferSize );
            dStrcpy( (char *)valueCopy, pFieldValue );

            // Skip if field should not be written.
            if (!pSimObject->writeField(fieldName, valueCopy))
                continue;

            // Reassign field value.
            pFieldValue = valueCopy;

            // Detect and collapse relative path information
            char fnBuf[1024];
            if ((S32)pField->type == TypeFilename)
            {
                Con::collapsePath( fnBuf, 1024, pFieldValue );
                pFieldValue = fnBuf;
            }

            // Save field/value.
            TamlWriteNode::FieldValuePair* pFieldValuePair = new TamlWriteNode::FieldValuePair( fieldName, pFieldValue );
            pTamlWriteNode->mFields.push_back( pFieldValuePair );
        }
    }    
}

//-----------------------------------------------------------------------------

static S32 QSORT_CALLBACK compareFieldEntries(const void* a,const void* b)
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CompareFieldEntries);

    SimFieldDictionary::Entry *fa = *((SimFieldDictionary::Entry **)a);
    SimFieldDictionary::Entry *fb = *((SimFieldDictionary::Entry **)b);
    return dStricmp(fa->slotName, fb->slotName);
}

//-----------------------------------------------------------------------------

void Taml::compileDynamicFields( TamlWriteNode* pTamlWriteNode )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CompileDynamicFields);

     // Sanity!
    AssertFatal( pTamlWriteNode != NULL, "Cannot compile dynamic fields on a NULL node." );
    AssertFatal( pTamlWriteNode->mpSimObject != NULL, "Cannot compile dynamic fields on a node with no object." );

    // Fetch object.
    SimObject* pSimObject = pTamlWriteNode->mpSimObject;

    // Fetch field dictionary.
    SimFieldDictionary* pFieldDictionary = pSimObject->getFieldDictionary();

    // Ignore if not writing dynamic fields.
    if ( !pFieldDictionary || !pSimObject->getCanSaveDynamicFields() )
        return;

    // Fetch field list.
    const AbstractClassRep::FieldList& fieldList = pSimObject->getFieldList();

    // Fetch field count.
    const U32 fieldCount = fieldList.size();

    Vector<SimFieldDictionary::Entry*> dynamicFieldList(__FILE__, __LINE__);

    // Ensure the dynamic field doesn't conflict with static field.
    for( U32 hashIndex = 0; hashIndex < SimFieldDictionary::HashTableSize; ++hashIndex )
    {
        for( SimFieldDictionary::Entry* pEntry = pFieldDictionary->mHashTable[hashIndex]; pEntry; pEntry = pEntry->next )
        {
            // Iterate static fields.
            U32 fieldIndex;
            for( fieldIndex = 0; fieldIndex < fieldCount; ++fieldIndex )
            {
                if( fieldList[fieldIndex].pFieldname == pEntry->slotName)
                    break;
            }

            // Skip if found.
            if( fieldIndex != (U32)fieldList.size() )
                continue;

            // Skip if not writing field.
            if ( !pSimObject->writeField( pEntry->slotName, pEntry->value) )
                continue;

            dynamicFieldList.push_back( pEntry );
        }
    }

    // Sort Entries to prevent version control conflicts
    if ( dynamicFieldList.size() > 1 )
        dQsort(dynamicFieldList.address(), dynamicFieldList.size(), sizeof(SimFieldDictionary::Entry*), compareFieldEntries);

    // Save the fields.
    for( Vector<SimFieldDictionary::Entry*>::iterator entryItr = dynamicFieldList.begin(); entryItr != dynamicFieldList.end(); ++entryItr )
    {
        // Fetch entry.
        SimFieldDictionary::Entry* pEntry = *entryItr;

        // Save field/value.
        TamlWriteNode::FieldValuePair*  pFieldValuePair = new TamlWriteNode::FieldValuePair( pEntry->slotName, pEntry->value );
        pTamlWriteNode->mFields.push_back( pFieldValuePair );
    }
}

//-----------------------------------------------------------------------------

void Taml::compileChildren( TamlWriteNode* pTamlWriteNode )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CompileChildren);

    // Sanity!
    AssertFatal( pTamlWriteNode != NULL, "Cannot compile children on a NULL node." );
    AssertFatal( pTamlWriteNode->mpSimObject != NULL, "Cannot compile children on a node with no object." );

    // Fetch object.
    SimObject* pSimObject = pTamlWriteNode->mpSimObject;

    // Fetch the Taml children.
    TamlChildren* pChildren = dynamic_cast<TamlChildren*>( pSimObject );

    // Finish if object does not contain Taml children.
    if ( pChildren == NULL || pChildren->getTamlChildCount() == 0 )
        return;

    // Create children vector.
    pTamlWriteNode->mChildren = new typeNodeVector();

    // Fetch the child count.
    const U32 childCount = pChildren->getTamlChildCount();

    // Iterate children.
    for ( U32 childIndex = 0; childIndex < childCount; childIndex++ )
    {
        // Compile object.
        TamlWriteNode* pChildTamlWriteNode = compileObject( pChildren->getTamlChild(childIndex) );

        // Save node.
        pTamlWriteNode->mChildren->push_back( pChildTamlWriteNode );
    }
}

//-----------------------------------------------------------------------------

void Taml::compileCollection( TamlWriteNode* pTamlWriteNode )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CompileCollection);

    // Sanity!
    AssertFatal( pTamlWriteNode != NULL, "Cannot compile collections on a NULL node." );
    AssertFatal( pTamlWriteNode->mpSimObject != NULL, "Cannot compile collections on a node with no object." );

    // Fetch the collection on the write node.
    TamlCollection& customCollection = pTamlWriteNode->mCustomCollection;

    // Are there any Taml callbacks?
    if ( pTamlWriteNode->mpTamlCallbacks != NULL )
    {
        // Yes, so call it.
        tamlCustomWrite( pTamlWriteNode->mpTamlCallbacks, customCollection );
    }

    // Finish if no custom collection to process.
    if ( customCollection.size() == 0 )
        return;

    // Iterate collections.
    for( TamlCollectionPropertyVector::iterator propertyItr = customCollection.begin(); propertyItr != customCollection.end(); ++propertyItr )
    {
        TamlCollectionProperty* pCollectionProperty = *propertyItr;

        // Iterate type alias.
        for( TamlPropertyTypeAliasVector::iterator typeAliasItr = pCollectionProperty->begin(); typeAliasItr != pCollectionProperty->end(); ++typeAliasItr )
        {
            TamlPropertyTypeAlias* pTypeAlias = *typeAliasItr;

            // Iterate the fields.
            for( TamlPropertyFieldVector::iterator fieldItr = pTypeAlias->begin(); fieldItr != pTypeAlias->end(); ++fieldItr )
            {
                TamlPropertyField* pPropertyField = *fieldItr;

                // Skip if not an object field.
                if ( !pPropertyField->isObjectField() )
                    continue;

                // Compile the object.
                TamlWriteNode* pCompiledWriteNode = compileObject( pPropertyField->getFieldObject() );

                // Set reference field.
                pCompiledWriteNode->mRefField = pPropertyField->getFieldName();

                // Set the write node for the property field.
                pPropertyField->setWriteNode( pCompiledWriteNode );
            }
        }
    }

#if 0
    // Iterate the collection removing ignored items.
    for( S32 collectionPropertyIndex = 0; collectionPropertyIndex < customCollection.size(); ++collectionPropertyIndex )
    {
        // Fetch the collection property.
        TamlCollectionProperty* pCollectionProperty = customCollection.at( collectionPropertyIndex );

        // Skip if we are not ignoring the collection if empty.
        if ( !pCollectionProperty->mIgnoreEmpty )
            continue;

        // Iterate the type alias.
        for ( S32 typeAliasIndex = 0; typeAliasIndex < pCollectionProperty->size(); ++typeAliasIndex )
        {
            // Fetch the type alias.
            TamlPropertyTypeAlias* pTypeAlias = pCollectionProperty->at( typeAliasIndex );

            // Skip If we're not ignoring the type alias or the collection is not empty.
            if ( !pTypeAlias->mIgnoreEmpty && pTypeAlias->size() != 0 )
                continue;

            // Remove the type alias.
            pCollectionProperty->removeTypeAlias( typeAliasIndex-- );
        }

        // Skip if the collection is not empty.
        if ( pCollectionProperty->size() != 0 )
            continue;

        // Remove the collection property.
        customCollection.removeCollectionProperty( collectionPropertyIndex-- );
    }
#endif
}

//-----------------------------------------------------------------------------

SimObject* Taml::createType( StringTableEntry typeName )
{
    // Debug Profiling.
    PROFILE_SCOPE(Taml_CreateType);

    typedef HashMap<StringTableEntry, AbstractClassRep*> typeClassHash;
    static typeClassHash mClassMap;

    // Sanity!
    AssertFatal( typeName != NULL, "Taml: Type cannot be NULL" );

    // Find type.
    typeClassHash::iterator typeItr = mClassMap.find( typeName );

    // Found type?
    if ( typeItr == mClassMap.end() )
    {
        // No, so find type.
        AbstractClassRep* pClassRep = AbstractClassRep::getClassList();
        while( pClassRep )
        {
            // Is this the type?
            if( dStricmp( pClassRep->getClassName(), typeName ) == 0 )
            {
                // Yes, so insert it.
                typeItr = mClassMap.insert( typeName, pClassRep );
                break;
            }

            // Next type.
            pClassRep = pClassRep->getNextClass();
        }

        // Did we find the type?
        if ( typeItr == mClassMap.end() )
        {
            // No, so warn and fail.
            Con::warnf( "Taml: Failed to create type '%s' as such a registered type could not be found.", typeName );
            return NULL;
        }
    }

    // Create the object.
    ConsoleObject* pConsoleObject = typeItr->value->create();

    // NOTE: It is important that we don't register the object here as many objects rely on the fact that
    // fields are set prior to the object being registered.  Registering here will invalid those assumptions.

    // Fetch the SimObject.
    SimObject* pSimObject = dynamic_cast<SimObject*>( pConsoleObject );

    // Was it a SimObject?
    if ( pSimObject == NULL )
    {
        // No, so warn.
        Con::warnf( "Taml: Failed to create type '%s' as it is not a SimObject.", typeName );

        // Destroy object and fail.
        delete pConsoleObject;
        return NULL;
    }

    return pSimObject;
}
