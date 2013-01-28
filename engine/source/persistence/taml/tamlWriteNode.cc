//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "persistence/taml/tamlWriteNode.h"

// Debug Profiling.
#include "debug/profiler.h"

//-----------------------------------------------------------------------------

void TamlWriteNode::resetNode( void )
{
    // Debug Profiling.
    PROFILE_SCOPE(TamlWriteNode_ResetNode);

    // Clear fields.
    for( Vector<TamlWriteNode::FieldValuePair*>::iterator itr = mFields.begin(); itr != mFields.end(); ++itr )
    {
        delete (*itr)->mpValue;
    }
    mFields.clear();

    // Clear children.
    if ( mChildren != NULL )
    {
        for( Vector<TamlWriteNode*>::iterator itr = mChildren->begin(); itr != mChildren->end(); ++itr )
        {
            (*itr)->resetNode();
        }

        mChildren->clear();
        delete mChildren;
        mChildren = NULL;
    }

    mRefId = 0;
    mRefToNode = NULL;
    mChildren = NULL;
    mpObjectName = NULL;
    mpSimObject = NULL;

    // Reset callbacks.
    mpTamlCallbacks = NULL;

    // Reset custom properties.
    mCustomProperties.resetState();
}