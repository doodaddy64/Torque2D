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
    Vector<DataKey> mDataKeys;

    F32 mValueScale;
    F32 mTimeScale;
    F32 mMaxTime;
    F32 mMinValue;
    F32 mMaxValue;
    F32 mDefaultValue;

public:
    ParticleAssetField();
    virtual ~ParticleAssetField();

    virtual void copyTo(ParticleAssetField& graph);

    const DataKey getDataKeyNode( const U32 index ) const;
    void resetDataKeys( void );
    bool setTimeRepeat( const F32 timeRepeat );
    bool setValueScale( const F32 valueScale );
    void setValueBounds( F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue );
    S32 addDataKey( const F32 time, const F32 value );
    bool removeDataKey( const U32 index );
    void clearDataKeys( void );
    bool setDataKeyValue( const U32 index, const F32 value );
    F32 getDataKeyValue( const U32 index ) const;
    F32 getDataKeyTime( const U32 index ) const;
    U32 getDataKeyCount( void ) const;
    F32 getGraphValue( F32 time ) const;
    F32 getMinValue( void ) const { return mMinValue; };
    F32 getMaxValue( void ) const { return mMaxValue; };
    F32 getMinTime( void ) const { return 0.0f; }
    F32 getMaxTime( void ) const { return mMaxTime; };
    F32 getTimeRepeat( void ) const { return mTimeScale - 1.0f; };
    F32 getValueScale( void ) const { return mValueScale; };

    // Calculate Graph Variants.
    static F32 calcGraphBV( const ParticleAssetField& base, const ParticleAssetField& variation, const F32 effectAge, const bool modulate = false, const F32 modulo = 0.0f );
    static F32 calcGraphBVE( const ParticleAssetField& base, const ParticleAssetField& variation, const ParticleAssetField& effect, const F32 effectAge, const bool modulate = false, const F32 modulo = 0.0f );
    static F32 calcGraphBVLE( const ParticleAssetField& base, const ParticleAssetField& variation, const ParticleAssetField& overlife, const ParticleAssetField& effect, const F32 effectTime, const F32 particleAge, const bool modulate = false, const F32 modulo = 0.0f );

protected:
    void onTamlCustomWrite( TamlCollection& customCollection );
    void onTamlCustomRead( const TamlCollection& customCollection );
};


///-----------------------------------------------------------------------------
/// Base-Only Graph.
///-----------------------------------------------------------------------------
struct ParticleAssetField_B
{
   ParticleAssetField       GraphField_Base;

   virtual void copyTo(ParticleAssetField_B& graph)
   {
      GraphField_Base.copyTo(graph.GraphField_Base);
   };
};

///-----------------------------------------------------------------------------
/// Life-Only Graph.
///-----------------------------------------------------------------------------
struct ParticleAssetField_L
{
   ParticleAssetField       GraphField_OverLife;

   void copyTo(ParticleAssetField_L& graph)
   {
      GraphField_OverLife.copyTo(graph.GraphField_OverLife);
   };
};

///-----------------------------------------------------------------------------
/// Base & Variation Graphs.
///-----------------------------------------------------------------------------
struct ParticleAssetField_BV
{
    ParticleAssetField       GraphField_Base;
    ParticleAssetField       GraphField_Variation;

    void copyTo(ParticleAssetField_BV& graph)
    {
       GraphField_Base.copyTo(graph.GraphField_Base);
       GraphField_Variation.copyTo(graph.GraphField_Variation);
    };
};

///-----------------------------------------------------------------------------
/// Base, Variation and Over-Time Graphs.
///-----------------------------------------------------------------------------
struct ParticleAssetField_BVL
{
    ParticleAssetField       GraphField_Base;
    ParticleAssetField       GraphField_Variation;
    ParticleAssetField       GraphField_OverLife;

    void copyTo(ParticleAssetField_BVL& graph)
    {
       GraphField_Base.copyTo(graph.GraphField_Base);
       GraphField_Variation.copyTo(graph.GraphField_Variation);
       GraphField_OverLife.copyTo(graph.GraphField_OverLife);
    };
};

#endif // _PARTICLE_ASSET_FIELD_H_
