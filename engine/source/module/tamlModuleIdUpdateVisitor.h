//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _TAML_MODULE_ID_UPDATE_VISITOR_H_
#define _TAML_MODULE_ID_UPDATE_VISITOR_H_

#ifndef _TAML_XMLPARSER_H_
#include "persistence//taml/tamlXmlParser.h"
#endif

#ifndef _STRINGTABLE_H_
#include "string/stringTable.h"
#endif

#ifndef _ASSET_FIELD_TYPES_H
#include "assets/assetFieldTypes.h"
#endif

//-----------------------------------------------------------------------------

class TamlModuleIdUpdateVisitor : public TamlXmlVisitor
{
protected:
    virtual bool visit( TiXmlElement* pXmlElement, TamlXmlParser& xmlParser )
    {
        // Iterate attributes.
        for ( TiXmlAttribute* pAttribute = pXmlElement->FirstAttribute(); pAttribute; pAttribute = pAttribute->Next() )
        {
            // Fetch attribute value.
            const char* pAttributeValue = pAttribute->Value();

            // Fetch value length.
            const U32 valueLenth = dStrlen(pAttributeValue);

            char newAttributeValueBuffer[1024];

            // Is this an expando?
            if ( *pAttributeValue == '^' )
            {
                // Yes, so skip if it's not the correct length.
                if ( valueLenth < mModuleIdLengthFrom+1 )
                    continue;

                // Is this the module Id?
                if ( dStrnicmp( pAttributeValue+1, mModuleIdFrom, mModuleIdLengthFrom ) == 0 )
                {
                    // Yes, so format a new value.
                    dSprintf( newAttributeValueBuffer, sizeof(newAttributeValueBuffer), "^%s%s",
                        mModuleIdTo, pAttributeValue+1+mModuleIdLengthFrom );

                    // Assign new value.
                    pAttribute->SetValue( newAttributeValueBuffer );
                }

                // Skip to next attribute.
                continue;
            }

            // Does the field start with the module Id?
            if ( dStrnicmp( pAttributeValue, mModuleIdFrom, mModuleIdLengthFrom ) == 0 )
            {
                // Yes, so format a new value.
                dSprintf( newAttributeValueBuffer, sizeof(newAttributeValueBuffer), "%s%s",
                    mModuleIdTo, pAttributeValue+mModuleIdLengthFrom );

                // Assign new value.
                pAttribute->SetValue( newAttributeValueBuffer );
            }
        }

        return true;
    }

    virtual bool visit( TiXmlAttribute* pAttribute, TamlXmlParser& xmlParser ) { return true; }

public:
    TamlModuleIdUpdateVisitor() :
        mModuleIdFrom( StringTable->EmptyString ),
        mModuleIdTo( StringTable->EmptyString ),
        mModuleIdLengthFrom( 0 ),
        mModuleIdLengthTo( 0 )      
        {}
    virtual ~TamlModuleIdUpdateVisitor() {}

    bool parse( const char* pFilename )
    {
        TamlXmlParser parser;
        return parser.parse( pFilename, *this, true );
    }

    void setModuleIdFrom( const char* pModuleIdFrom )
    {
        // Sanity!
        AssertFatal( pModuleIdFrom != NULL, "Asset Id from cannot be NULL." );

        // Set module Id.
        mModuleIdFrom = StringTable->insert( pModuleIdFrom );
        mModuleIdLengthFrom = dStrlen(mModuleIdFrom);
    }
    StringTableEntry getModuleIdFrom( void ) const { return mModuleIdFrom; }

    void setModuleIdTo( const char* pModuleIdTo )
    {
        // Sanity!
        AssertFatal( pModuleIdTo != NULL, "Asset Id to cannot be NULL." );

        // Set module Id.
        mModuleIdTo = StringTable->insert( pModuleIdTo );
        mModuleIdLengthTo = dStrlen(mModuleIdTo);
    }
    const char* getModuleIdTo( void ) const { return mModuleIdTo; }

private:    
    StringTableEntry mModuleIdFrom;
    StringTableEntry mModuleIdTo;
    U32 mModuleIdLengthFrom;
    U32 mModuleIdLengthTo;
};

#endif // _TAML_MODULE_ID_UPDATE_VISITOR_H_
