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
    friend class Taml;

private:
    /// Called when Taml attempts to compile a list of children.
    virtual U32 getTamlChildCount( void ) const = 0;

    /// Called when Taml attempts to compile a list of children.
    virtual SimObject* getTamlChild( const U32 childIndex ) const = 0;
};

#endif // _TAML_CHILDREN_H_