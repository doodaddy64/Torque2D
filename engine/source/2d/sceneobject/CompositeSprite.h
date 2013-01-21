//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _COMPOSITE_SPRITE_H_
#define _COMPOSITE_SPRITE_H_

#ifndef _SPRITE_BATCH_H_
#include "2d/core/SpriteBatch.h"
#endif

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneobject/SceneObject.h"
#endif

//------------------------------------------------------------------------------  

class CompositeSprite : public SceneObject, public SpriteBatch
{
protected:
    typedef SceneObject Parent;

public:
    // Batch layout type.
    enum BatchLayoutType
    {
        INVALID_LAYOUT,

        NO_LAYOUT,
        RECTILINEAR_LAYOUT,
        ISOMETRIC_LAYOUT,
        CUSTOM_LAYOUT
    };

private:
    BatchLayoutType mBatchLayoutType;

public:
    CompositeSprite();
    virtual ~CompositeSprite();

    static void initPersistFields();

    virtual void preIntegrate( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    virtual void integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    virtual void interpolateObject( const F32 timeDelta );

    virtual bool canPrepareRender( void ) const { return true; }
    
    virtual void scenePrepareRender( const SceneRenderState* pSceneRenderState, SceneRenderQueue* pSceneRenderQueue );
    
    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );

    virtual void copyTo( SimObject* object );

    void setBatchLayout( const BatchLayoutType& batchLayoutType );
    BatchLayoutType getBatchLayout( void ) const { return mBatchLayoutType; }

    static BatchLayoutType getBatchLayoutTypeEnum( const char* label );
    static const char* getBatchLayoutTypeDescription( const BatchLayoutType batchLayoutType );

    /// Declare Console Object.
    DECLARE_CONOBJECT( CompositeSprite );

protected:
    virtual SpriteBatchItem* createSprite( const SpriteBatchItem::LogicalPosition& logicalPosition );
    virtual SpriteBatchItem* createSpriteRectilinearLayout( const SpriteBatchItem::LogicalPosition& logicalPosition );
    virtual SpriteBatchItem* createSpriteIsometricLayout( const SpriteBatchItem::LogicalPosition& logicalPosition );
    virtual SpriteBatchItem* createCustomLayout( const SpriteBatchItem::LogicalPosition& logicalPosition );

    virtual void onTamlCustomWrite( TamlCollection& customCollection );
    virtual void onTamlCustomRead( const TamlCollection& customCollection );

protected:
    static bool         setDefaultSpriteAngle(void* obj, const char* data)                  { STATIC_VOID_CAST_TO(CompositeSprite, SpriteBatch, obj)->setDefaultSpriteAngle(mDegToRad(dAtof(data))); return false; }
    static const char*  getDefaultSpriteAngle(void* obj, const char* data)                  { return Con::getFloatArg( mRadToDeg(STATIC_VOID_CAST_TO(CompositeSprite, SpriteBatch, obj)->getDefaultSpriteAngle()) ); }
    static bool         writeDefaultSpriteAngle( void* obj, StringTableEntry pFieldName )   { return mNotZero( static_cast<SpriteBatch*>(obj)->getDefaultSpriteAngle() ); }
    static bool         writeBatchIsolated( void* obj, StringTableEntry pFieldName )        { return static_cast<CompositeSprite*>(obj)->getBatchIsolated(); }
    static bool         writeBatchSortMode( void* obj, StringTableEntry pFieldName )        { return static_cast<CompositeSprite*>(obj)->getBatchSortMode() != SceneRenderQueue::RENDER_SORT_OFF; }

    static bool         setBatchLayout(void* obj, const char* data)                         { static_cast<CompositeSprite*>(obj)->setBatchLayout( getBatchLayoutTypeEnum(data) ); return false; }
    static bool         writeBatchLayout( void* obj, StringTableEntry pFieldName )          { return static_cast<CompositeSprite*>(obj)->getBatchLayout() != CompositeSprite::RECTILINEAR_LAYOUT; }
    static bool         setBatchCulling(void* obj, const char* data)                        { STATIC_VOID_CAST_TO(CompositeSprite, SpriteBatch, obj)->setBatchCulling(dAtob(data)); return false; }
    static bool         writeBatchCulling( void* obj, StringTableEntry pFieldName )         { return !static_cast<CompositeSprite*>(obj)->getBatchCulling(); }
};

#endif // _COMPOSITE_SPRITE_H_
