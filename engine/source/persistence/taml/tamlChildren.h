//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _TAML_CHILDREN_H_
#define _TAML_CHILDREN_H_

//-----------------------------------------------------------------------------

class SimObject;

//-----------------------------------------------------------------------------

class TamlChildren
{
public:
    /// Called when Taml attempts to compile a list of children.
    virtual U32 getTamlChildCount( void ) const = 0;

    /// Called when Taml attempts to compile a list of children.
    virtual SimObject* getTamlChild( const U32 childIndex ) const = 0;

    /// Called when Taml attempts to populate an objects children during a read.
    virtual void addTamlChild( SimObject* pSimObject ) = 0;
};

#endif // _TAML_CHILDREN_H_