//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_ASSET_FIELD_H_
#define _PARTICLE_ASSET_FIELD_H_

#ifndef _VECTOR_H_
#include "collection/vector.h"
#endif

#ifndef _TAML_COLLECTION_H_
#include "persistence/taml/tamlCollection.h"
#endif

///-----------------------------------------------------------------------------

class ParticleAssetField
{
public:
    /// Data Key Node.
    struct DataKey
    {
        F32     mTime;
        F32     mValue;
    };

private:
    StringTableEntry mFieldName;
    F32 mValueScale;
    F32 mTimeScale;
    F32 mMaxTime;
    F32 mMinValue;
    F32 mMaxValue;
    F32 mDefaultValue;

    Vector<DataKey> mDataKeys;

public:
    ParticleAssetField();
    virtual ~ParticleAssetField();

    void copyTo( ParticleAssetField& field );

    void setFieldName( const char* pFieldName );
    StringTableEntry getFieldName( void ) const { return mFieldName; }
    bool setValueScale( const F32 valueScale );
    bool setTimeRepeat( const F32 timeRepeat );
    void setValueBounds( F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue );
    void resetDataKeys( void );
    S32 addDataKey( const F32 time, const F32 value );
    bool removeDataKey( const U32 index );
    void clearDataKeys( void );
    const DataKey getDataKeyNode( const U32 index ) const;
    bool setDataKeyValue( const U32 index, const F32 value );
    F32 getDataKeyValue( const U32 index ) const;
    F32 getDataKeyTime( const U32 index ) const;
    U32 getDataKeyCount( void ) const;
    F32 getFieldValue( F32 time ) const;
    inline F32 getMinValue( void ) const { return mMinValue; };
    inline F32 getMaxValue( void ) const { return mMaxValue; };
    inline F32 getMinTime( void ) const { return 0.0f; }
    inline F32 getMaxTime( void ) const { return mMaxTime; };
    inline F32 getTimeRepeat( void ) const { return mTimeScale - 1.0f; };
    inline F32 getValueScale( void ) const { return mValueScale; };

    static F32 calculateFieldBV( const ParticleAssetField& base, const ParticleAssetField& variation, const F32 effectAge, const bool modulate = false, const F32 modulo = 0.0f );
    static F32 calculateFieldBVE( const ParticleAssetField& base, const ParticleAssetField& variation, const ParticleAssetField& effect, const F32 effectAge, const bool modulate = false, const F32 modulo = 0.0f );
    static F32 calculateFieldBVLE( const ParticleAssetField& base, const ParticleAssetField& variation, const ParticleAssetField& overlife, const ParticleAssetField& effect, const F32 effectTime, const F32 particleAge, const bool modulate = false, const F32 modulo = 0.0f );

protected:
    void onTamlCustomWrite( TamlCollection& customCollection );
    void onTamlCustomRead( const TamlCollection& customCollection );
};

//-----------------------------------------------------------------------------

/// Base field.
class ParticleAssetFieldBase
{
private:
    ParticleAssetField mBase;

public:
    inline ParticleAssetField& getBase( void ) { return mBase; }

    virtual void copyTo( ParticleAssetFieldBase& particleField )
    {
        mBase.copyTo( particleField.getBase() );
    };
};

//-----------------------------------------------------------------------------

/// Life field.
class ParticleAssetFieldLife
{
private:
    ParticleAssetField mLife;

public:
    inline ParticleAssetField& getLife( void ) { return mLife; }

    void copyTo( ParticleAssetFieldLife& particleField )
    {
        mLife.copyTo( particleField.getLife() );
    };
};

//-----------------------------------------------------------------------------

/// Variation field.
class ParticleAssetFieldVariation
{
private:
    ParticleAssetField mVariation;

public:
    inline ParticleAssetField& getVariation( void ) { return mVariation; }

    virtual void copyTo( ParticleAssetFieldVariation& particleField )
    {
        mVariation.copyTo( particleField.getVariation() );
    };
};

//-----------------------------------------------------------------------------

/// Base and variation fields.
class ParticleAssetFieldBaseVariation :
    public ParticleAssetFieldBase,
    public ParticleAssetFieldVariation
{
public:
    void copyTo( ParticleAssetFieldBaseVariation& particleField )
    {
        getBase().copyTo( particleField.getBase() );
        getVariation().copyTo( particleField.getVariation() );
    };
};

//-----------------------------------------------------------------------------

/// Base, variation and life fields.
class ParticleAssetFieldBaseVariationLife :
    public ParticleAssetFieldBase,
    public ParticleAssetFieldVariation,
    public ParticleAssetFieldLife
{
public:
    void copyTo( ParticleAssetFieldBaseVariationLife& particleField )
    {
        getBase().copyTo( particleField.getBase() );
        getVariation().copyTo( particleField.getVariation() );
        getLife().copyTo( particleField.getLife() );
    };
};

#endif // _PARTICLE_ASSET_FIELD_H_
