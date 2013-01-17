//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_FIELD_COLLECTION_H_
#define _PARTICLE_ASSET_FIELD_COLLECTION_H_

#ifndef _PARTICLE_ASSET_FIELD_H_
#include "2d/assets/ParticleAssetField.h"
#endif

#ifndef _HASHTABLE_H
#include "collection/hashTable.h"
#endif

#ifndef _TAML_COLLECTION_H_
#include "persistence/taml/tamlCollection.h"
#endif

///-----------------------------------------------------------------------------

class ParticleAssetFieldCollection
{
public:
    typedef HashMap<StringTableEntry, ParticleAssetField*> typeFieldHash;

private:
    typeFieldHash       mFields;
    ParticleAssetField* mpSelectedField;

public:
    ParticleAssetFieldCollection();
    virtual ~ParticleAssetFieldCollection();

    void copyTo( ParticleAssetFieldCollection& fieldCollection );

    void addField( ParticleAssetField& particleAssetField, const char* pFieldName, F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue );

    inline ParticleAssetField* selectField( const char* pFieldName ) { mpSelectedField = findField( pFieldName ); return mpSelectedField; }
    inline void deselectField( void ) { mpSelectedField = NULL; }
    inline const ParticleAssetField* getSelectedField( void ) const { return mpSelectedField; }

    inline const typeFieldHash& getFields( void ) const { return mFields; }
    ParticleAssetField* findField( const char* pFieldName );

    S32 addDataKey( F32 time, F32 value );
    bool removeDataKey( S32 index );
    bool clearDataKeys( void );
    bool setDataKey( S32 index, F32 value );

    F32 getDataKeyValue( S32 index ) const;
    F32 getDataKeyTime( S32 index ) const;
    const ParticleAssetField::DataKey& getDataKey( const U32 index ) const;
    U32 getDataKeyCount( void ) const;
    F32 getMinValue( void ) const;
    F32 getMaxValue( void ) const;
    F32 getMinTime( void ) const;
    F32 getMaxTime( void ) const;
    F32 getFieldValue( F32 time ) const;
    bool setRepeatTime( const F32 repeatTime );
    F32 getRepeatTime( void ) const;
    bool setValueScale( const F32 valueScale );
    F32 getValueScale( void ) const;    

    void onTamlCustomWrite( TamlCollection& customCollection );
    void onTamlCustomRead( const TamlCollection& customCollection );
};

#endif // ParticleAssetFieldCollection
