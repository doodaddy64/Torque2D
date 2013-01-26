//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _REFERENCED_ASSETS_H_
#define _REFERENCED_ASSETS_H_

#ifndef _SIMBASE_H_
#include "sim/simBase.h"
#endif

//-----------------------------------------------------------------------------

class ReferencedAssets : public SimObject
{
    friend class AssetManager;

private:
    typedef SimObject Parent;

    StringTableEntry    mPath;
    StringTableEntry    mExtension;
    bool                mRecurse;

public:
    ReferencedAssets() :
        mPath( StringTable->EmptyString ),
        mExtension( StringTable->EmptyString ),
        mRecurse( true )
        {}
    virtual ~ReferencedAssets() {}

    static void initPersistFields();

    inline void setPath( const char* pPath )            { mPath = StringTable->insert( pPath ); }
    inline StringTableEntry getPath( void ) const       { return mPath; }
    inline void setExtension( const char* pPath )       { mExtension = StringTable->insert( pPath ); }
    inline StringTableEntry getExtension( void ) const  { return mExtension; }
    inline void setRecurse( const bool recurse )        { mRecurse = recurse; }
    inline bool getRecurse( void ) const                { return mRecurse; }

    /// Declare Console Object.
    DECLARE_CONOBJECT( ReferencedAssets );
};

#endif // _REFERENCED_ASSETS_H_

