//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "2d/assets/ParticleAssetField.h"

#ifndef _CORE_MATH_H_
#include "2d/core/coreMath.h"
#endif

#ifndef _MMATH_H_
#include "math/mMath.h"
#endif

#ifndef _SIMBASE_H_
#include "sim/simBase.h"
#endif

//-----------------------------------------------------------------------------

static bool particleAssetFieldPropertiesInitialized = false;

static StringTableEntry particleAssetFieldCollectionName;
static StringTableEntry particleAssetFieldTimeScaleName;
static StringTableEntry particleAssetFieldValueScaleName;
static StringTableEntry particleAssetFieldMaxTimeName;
static StringTableEntry particleAssetFieldMinValueName;
static StringTableEntry particleAssetFieldMaxValueName;
static StringTableEntry particleAssetFieldDefaultValueName;

static StringTableEntry particleAssetFieldDataKeyName;
static StringTableEntry particleAssetFieldDataKeyTimeName;
static StringTableEntry particleAssetFieldDataKeyValueName;

//-----------------------------------------------------------------------------

ParticleAssetField::ParticleAssetField() :
                            mTimeScale(1.0f),
                            mValueScale(1.0f)
{
    // Set Vector Associations.
    VECTOR_SET_ASSOCIATION( mDataKeys );

    // Initialize names.
    if ( !particleAssetFieldPropertiesInitialized )
    {
        particleAssetFieldCollectionName      = StringTable->insert( "Cells" );

        // Flag as initialized.
        particleAssetFieldPropertiesInitialized = true;
    }
}

//-----------------------------------------------------------------------------

ParticleAssetField::~ParticleAssetField()
{
    // Clear Data Keys.
    mDataKeys.clear();
}

//-----------------------------------------------------------------------------

void ParticleAssetField::copyTo(ParticleAssetField& graph)
{
   graph.mTimeScale = mTimeScale;
   graph.mValueScale = mValueScale;
   graph.mMaxTime = mMaxTime;
   graph.mMinValue = mMinValue;
   graph.mMaxValue = mMaxValue;
   graph.mDefaultValue = mDefaultValue;

   for (S32 i = 0; i < mDataKeys.size(); i++)
   {
      DataKey key = mDataKeys[i];
      graph.addDataKey(key.mTime, key.mValue);
   }
}

//-----------------------------------------------------------------------------

void ParticleAssetField::resetDataKeys(void)
{
    // Clear Data Keys.
    mDataKeys.clear();

    // Add default value Data-Key.
    addDataKey( 0.0f, mDefaultValue );
}

//-----------------------------------------------------------------------------

void ParticleAssetField::setValueBounds( F32 maxTime, F32 minValue, F32 maxValue, F32 defaultValue )
{
    // Check Max Time.
    if ( maxTime <= 0.0f )
    {
        // Warn.
        Con::warnf("ParticleAssetField::setBounds() - Max Time is not valid! (maxTime:%f", maxTime );

        // Set Default Max Time.
        maxTime = 1.0f;
    }

    // Set Max Time.
    mMaxTime = maxTime;

    // Check Value Range Normalisation.
    if ( minValue > maxValue )
    {
        // Warn.
        Con::warnf("ParticleAssetField::setBounds() - Value Range is not normalised! (minValue:%f / maxValue:%f)", minValue, maxValue );

        // Normalise Y-Axis.
        F32 temp = minValue;
        minValue = maxValue;
        maxValue = temp;
    }
    // Check Value Range Scale.
    else if ( minValue == maxValue )
    {
        // Warn.
        Con::warnf("ParticleAssetField::setBounds() - Value Range has no scale! (minValue:%f / maxValue:%f)", minValue, maxValue );

        // Insert some Y-Axis Scale.
        maxValue = minValue + 0.001f;
    }

    // Set Bounds.
    mMinValue = minValue;
    mMaxValue = maxValue;

    // Check Default Value.
    if ( defaultValue < minValue || defaultValue > maxValue )
    {
        // Warn.
        Con::warnf("ParticleAssetField::setBounds() - Default Value is out of range! (minValue:%f / maxValue:%f / defaultValue:%f)", minValue, maxValue, defaultValue );

        // Clamp at lower value.
        defaultValue = minValue;
    }

    // Set Default Value.
    mDefaultValue = defaultValue;

    // Reset Data-Keys.
    resetDataKeys();
}

//-----------------------------------------------------------------------------

bool ParticleAssetField::setTimeRepeat( const F32 timeRepeat )
{
    // Check Time Repeat.
    if ( timeRepeat < 0.0f )
    {
        // Warn.
        Con::warnf("ParticleAssetField::setTimeRepeat() - Invalid Time Repeat! (%f)", timeRepeat );

        // Return Error.
        return false;
    }

    // Set Time Scale.
    // NOTE:-   Incoming Time-Repeat is zero upwards and we actually
    //          want to use it as a multiplier so we increase it
    //          by one.
    mTimeScale = timeRepeat + 1.0f;

    // Return Okay.
    return true;
}

//-----------------------------------------------------------------------------

bool ParticleAssetField::setValueScale( const F32 valueScale )
{
    // Check Value Scale.
    if ( valueScale < 0.0f )
    {
        // Warn.
        Con::warnf("ParticleAssetField::setValueScale() - Invalid Value Scale! (%f)", valueScale );

        // Return Error.
        return false;
    }

    // Set Value Scale/
    mValueScale = valueScale;

    // Return Okay.
    return true;
}


//-----------------------------------------------------------------------------

S32 ParticleAssetField::addDataKey( const F32 time, const F32 value )
{
    // Check Max Time.
    if ( time > mMaxTime )
    {
        // Warn.
        Con::warnf("ParticleAssetField::addDataKey() - Time is out of bounds! (time:%f)", time );

        // Return Error.
        return -1;
    }

    // If data key exists already then set it and return the key index.
    U32 index = 0;
    for ( index = 0; index < getDataKeyCount(); index++ )
    {
        // Found Time?
        if ( mDataKeys[index].mTime == time )
        {
            // Yes, so set time.
            mDataKeys[index].mValue = value;

            // Return Index.
            return index;
        }
        // Past Time?
        else if ( mDataKeys[index].mTime > time )
            // Finish search.
            break;
    }

    // Insert Data-Key.
    mDataKeys.insert( index );

    // Set Data-Key.
    mDataKeys[index].mTime = time;
    mDataKeys[index].mValue = value;

    // Return Index.
    return index;
}

//-----------------------------------------------------------------------------

bool ParticleAssetField::removeDataKey( const U32 index )
{
    // Cannot Remove First Node!
    if ( index == 0 )
    {
        // Warn.
        Con::warnf("removeDataKey() - Cannot remove first Data-Key!");
        return false;
    }

    // Check Index.
    if ( index >= getDataKeyCount() )
    {
        // Warn.
        Con::warnf("removeDataKey() - Index out of range! (%d of %d)", index, getDataKeyCount()-1);
        return false;
    }

    // Remove Index.
    mDataKeys.erase(index);

    // Return Okay.
    return true;
}

//-----------------------------------------------------------------------------

void ParticleAssetField::clearDataKeys( void )
{
    // Reset Data Keys.
    resetDataKeys();
}

//-----------------------------------------------------------------------------

const ParticleAssetField::DataKey ParticleAssetField::getDataKeyNode( const U32 index ) const
{
    // Check Index.
    if ( index >= getDataKeyCount() )
    {
        // Warn.
        Con::warnf("getDataKeyNode() - Index out of range! (%d of %d)", index, getDataKeyCount()-1);
        return DataKey();
    }

    // Return Data-Key.
    return mDataKeys[index];
}

//-----------------------------------------------------------------------------

bool ParticleAssetField::setDataKeyValue( const U32 index, const F32 value )
{
    // Check Index.
    if ( index >= getDataKeyCount() )
    {
        // Warn.
        Con::warnf("setDataKeyValue() - Index out of range! (%d of %d)", index, getDataKeyCount()-1);
        return false;
    }

    // Set Data Key Value.
    mDataKeys[index].mValue = value;

    // Return Okay.
    return true;
}

//-----------------------------------------------------------------------------

F32 ParticleAssetField::getDataKeyValue( const U32 index ) const
{
    // Check Index.
    if ( index >= getDataKeyCount() )
    {
        // Warn.
        Con::warnf("getDataKeyValue() - Index out of range! (%d of %d)", index, getDataKeyCount()-1);
        return 0.0f;
    }

    // Return Data Key Value.
    return mDataKeys[index].mValue;
}

//-----------------------------------------------------------------------------

F32 ParticleAssetField::getDataKeyTime( const U32 index ) const
{
    // Check Index.
    if ( index >= getDataKeyCount() )
    {
        // Warn.
        Con::warnf("getDataKeyTime() - Index out of range! (%d of %d)", index, getDataKeyCount()-1);
        return 0.0f;
    }

    // Return Data Key Time.
    return mDataKeys[index].mTime;
}

//-----------------------------------------------------------------------------

U32 ParticleAssetField::getDataKeyCount( void ) const
{
    // Return Data Key Count.
    return mDataKeys.size();
}

//-----------------------------------------------------------------------------

F32 ParticleAssetField::getGraphValue( F32 time ) const
{
    // Return First Entry if it's the only one or we're using zero time.
    if ( mIsZero(time) || getDataKeyCount() < 2)
        return mDataKeys[0].mValue * mValueScale;

    // Clamp Key-Time.
    time = getMin(getMax( 0.0f, time ), mMaxTime);

    // Repeat Time.
    time = mFmod( time * mTimeScale, mMaxTime + FLT_EPSILON );

    // Fetch Max Key Index.
    const U32 maxKeyIndex = getDataKeyCount()-1;

    // Return Last Value if we're on/past the last time.
    if ( time >= mDataKeys[maxKeyIndex].mTime )
        return mDataKeys[maxKeyIndex].mValue * mValueScale;

    // Find Data-Key Indexes.
    U32 index1;
    U32 index2;
    for ( index1 = 0; index1 < getDataKeyCount(); index1++ )
        if ( mDataKeys[index1].mTime >= time )
            break;

    // If we're exactly on a Data-Key then return that key.
    if ( mIsEqual( mDataKeys[index1].mTime, time) )
        return mDataKeys[index1].mValue * mValueScale;

    // Set Adjacent Indexes.
    index2 = index1--;

    // Fetch Index Times.
    const F32 time1 = mDataKeys[index1].mTime;
    const F32 time2 = mDataKeys[index2].mTime;
    // Calculate Time Differential.
    const F32 dTime = (time-time1)/(time2-time1);

    // Return Lerped Value.
    return ((mDataKeys[index1].mValue * (1.0f-dTime)) + (mDataKeys[index2].mValue * dTime)) * mValueScale;
}

//-----------------------------------------------------------------------------
//
//  Some explanation of the acronyms used here is called for...
//
//  B - Base Value used in the emitter
//  V - Variation Value used in the emitter
//  L - Over-Life Value used in the particle.
//  E - Effect Value used in the effect.
//
//  The particle generation system has fields which are initially set when the
//  particle is created and then scaled during the lifetime of the particle.
//  When the particle is created, each field is calculated based upon a value
//  from its base/variation graphs which are against the effects age.  Into this
//  a scaling from the effect itself is added.
//
//  When the particle is active, these values are scaled using the over-life
//  graphs for the appropriate fields.
//
//  BV -    This is the calculation where the field does not use either the
//          over-life field 'L' of the effect field 'E'.
//
//  BVE -   This is the calculation where the field does not use the over-life
//          field 'L'.
//
//  BVLE -  This is the full calculation including the over-life field.
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

F32 ParticleAssetField::calcGraphBV( const ParticleAssetField& base, const ParticleAssetField& variation, const F32 effectAge, const bool modulate, const F32 modulo )
{
    // Fetch Graph Components.
    const F32 baseValue   = base.getGraphValue( effectAge );
    const F32 varValue    = variation.getGraphValue( effectAge ) * 0.5f;

    // Modulate?
    if ( modulate )
        // Return Modulo Calculation.
        return mFmod( baseValue + CoreMath::mGetRandomF(-varValue, varValue), modulo );
    else
        // Return Clamped Calculation.
        return mClampF( baseValue + CoreMath::mGetRandomF(-varValue, varValue), base.getMinValue(), base.getMaxValue() );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetField::calcGraphBVE( const ParticleAssetField& base, const ParticleAssetField& variation, const ParticleAssetField& effect, const F32 effectAge, const bool modulate, const F32 modulo )
{
    // Fetch Graph Components.
    const F32 baseValue   = base.getGraphValue( effectAge );
    const F32 varValue    = variation.getGraphValue( effectAge ) * 0.5f;
    const F32 effectValue = effect.getGraphValue( effectAge );

    // Modulate?
    if ( modulate )
        // Return Modulo Calculation.
        return mFmod( (baseValue + CoreMath::mGetRandomF(-varValue, varValue)) * effectValue, modulo );
    else
        // Return Clamped Calculation.
        return mClampF( (baseValue + CoreMath::mGetRandomF(-varValue, varValue)) * effectValue, base.getMinValue(), base.getMaxValue() );
}

//-----------------------------------------------------------------------------

F32 ParticleAssetField::calcGraphBVLE( const ParticleAssetField& base, const ParticleAssetField& variation, const ParticleAssetField& overlife, const ParticleAssetField& effect, const F32 effectAge, const F32 particleAge, const bool modulate, const F32 modulo )
{
    // Fetch Graph Components.
    const F32 baseValue   = base.getGraphValue( effectAge );
    const F32 varValue    = variation.getGraphValue( effectAge ) * 0.5f;
    const F32 effectValue = effect.getGraphValue( effectAge );
    const F32 lifeValue   = overlife.getGraphValue( particleAge );

    // Modulate?
    if ( modulate )
        // Return Modulo Calculation.
        return mFmod( (baseValue + CoreMath::mGetRandomF(-varValue, varValue)) * effectValue * lifeValue, modulo );
    else
        // Return Clamped Calculation.
        return mClampF( (baseValue + CoreMath::mGetRandomF(-varValue, varValue)) * effectValue * lifeValue, base.getMinValue(), base.getMaxValue() );
}

//------------------------------------------------------------------------------

void ParticleAssetField::onTamlCustomWrite( TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAssetField_OnTamlCustomWrite);


}

//-----------------------------------------------------------------------------

void ParticleAssetField::onTamlCustomRead( const TamlCollection& customCollection )
{
    // Debug Profiling.
    PROFILE_SCOPE(ParticleAssetField_OnTamlCustomRead);



}

/*
IMPLEMENT_2D_LOAD_METHOD( ParticleAssetField, 2 )
{
    F32 valueScale;
    F32 timeScale;
    F32 maxTime;
    F32 minValue;
    F32 maxValue;
    F32 defaultValue;

    // Object Info.
    if  (   !stream.read( &timeScale ) ||
            !stream.read( &valueScale ) ||
            !stream.read( &maxTime ) ||
            !stream.read( &minValue ) ||
            !stream.read( &maxValue ) ||
            !stream.read( &defaultValue ) )
        return false;

    // Set Value Bounds.
    object->setValueBounds( maxTime, minValue, maxValue, defaultValue );

    // Set Time Scale Directly.
    object->mTimeScale = timeScale;

    // Read Data-Key Count.
    S32 keyCount;
    if ( !stream.read( &keyCount ) )
        return false;

    // Read Data-Keys.
    F32 time;
    F32 value;
    for ( U32 n = 0; n < (U32)keyCount; n++ )
    {
        // Read Time/Value.
        if (    !stream.read( &time ) ||
                !stream.read( &value ) )
                return false;

        // Add Data-Key.
        // NOTE:-   We'll simply overwrite the default key at t=0.0!
        object->addDataKey( time, value );
    }

    // Return Okay.
    return true;
}

IMPLEMENT_2D_SAVE_METHOD( ParticleAssetField, 2 )
{
    // Object Info.
    if  (   !stream.write( object->mTimeScale ) ||
            !stream.write( object->mValueScale ) ||
            !stream.write( object->mMaxTime ) ||
            !stream.write( object->mMinValue ) ||
            !stream.write( object->mMaxValue ) ||
            !stream.write( object->mDefaultValue ) )
        return false;

    // Write Data-Key Count.
    if ( !stream.write( object->mDataKeys.size() ) )
        return false;

    // Write Data-Keys.
    for ( U32 n = 0; n < (U32)object->mDataKeys.size(); n++ )
        if (    !stream.write( object->mDataKeys[n].mTime ) ||
                !stream.write( object->mDataKeys[n].mValue ) )
            return false;

    // Return Okay.
    return true;
}
*/