//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _PARTICLE_EFFECT_H_
#define _PARTICLE_EFFECT_H_

#ifndef _SCENE_OBJECT_H_
#include "2d/sceneobject/SceneObject.h"
#endif

#ifndef _PARTICLE_ASSET_FIELD_H_
#include "ParticleAssetField.h"
#endif

#ifndef _PARTICLE_EMITTER_H_
#include "ParticleEmitter.h"
#endif

//-----------------------------------------------------------------------------

struct tEmitterHandle
{
   SimObjectId     mObjectId;
   SimObject*      mpSceneObject;
};

//-----------------------------------------------------------------------------

typedef Vector<tEmitterHandle> typeEmitterVector;

//-----------------------------------------------------------------------------

class ParticleEffect : public SceneObject
{
    friend class ParticleEmitter;

    typedef SceneObject      Parent;

private:
    StringTableEntry                mEffectFile;

    typeEmitterVector               mParticleEmitterList;                                   ///< Emitter List.

    bool                            mEffectPlaying;
    F32                             mEffectAge;
    bool                            mEffectPaused;

    F32                             mEffectLifetime;
    
    F32                             mCameraIdleDistance;
    bool                            mCameraIdle;

    bool                            mInitialised;
    bool                            mWaitingForParticles;
    bool                            mWaitingForDelete;
    bool                            mUseEffectCollisions;

    U32                             mEmitterSerial;

    bool							mDisableParticleInterpolation;

    /// Graph Selection.
    struct tGraphSelection
    {
        StringTableEntry            mGraphName;
        ParticleAssetField*          mpGraphObject;
    };

    ParticleAssetField*              mpCurrentGraph;
    StringTableEntry            mCurrentGraphName;
    Vector<tGraphSelection*>    mGraphSelectionList;

    /// Graph Selection.
    void clearGraphSelections( void );
    void addGraphSelection( const char* graphName, ParticleAssetField* pGraphObject );
    ParticleAssetField* findGraphSelection( const char* graphName ) const;


public:
    /// Graph Properties.
    ParticleAssetFieldBase     mParticleLife;
    ParticleAssetFieldBase     mQuantity;
    ParticleAssetFieldBase     mSizeX;
    ParticleAssetFieldBase     mSizeY;
    ParticleAssetFieldBase     mSpeed;
    ParticleAssetFieldBase     mSpin;
    ParticleAssetFieldBase     mFixedForce;
    ParticleAssetFieldBase     mRandomMotion;
    ParticleAssetFieldBaseVariation    mEmissionForce;
    ParticleAssetFieldBaseVariation    mEmissionAngle;
    ParticleAssetFieldBaseVariation    mEmissionArc;
    ParticleAssetFieldBase     mVisibility;

    /// Effect Life Mode.
    enum eEffectLifeMode
    {
        INFINITE,
        CYCLE,
        KILL,
        STOP

    } mEffectLifeMode;

    ParticleEffect();
    virtual ~ParticleEffect();

    static void initPersistFields();

    virtual void            OnRegisterScene( Scene* pScene );
    virtual void            OnUnregisterScene( Scene* pScene );

    // Initialise.
    void initialise( void );

    // Emitter Indices.
    bool isEmitterValid( SimObjectId emitterID ) const;
    S32 findEmitterIndex( SimObjectId emitterID ) const;

    /// Graph Editing.
    void selectGraph( const char* graphName );
    S32 addDataKey( F32 time, F32 value );
    bool removeDataKey( S32 index );
    bool clearDataKeys( void );
    bool setDataKey( S32 index, F32 value );
    F32 getDataKeyValue( S32 index ) const;
    F32 getDataKeyTime( S32 index ) const;
    U32 getDataKeyCount( void ) const;
    F32 getMinValue( void ) const;
    F32 getMaxValue( void ) const;
    F32 getMinTime( void ) const;
    F32 getMaxTime( void ) const;
    F32 getGraphValue( F32 time ) const;
    bool setTimeRepeat( const F32 timeRepeat );
    F32 getTimeRepeat( void ) const;
    bool setValueScale( const F32 valueScale );
    F32 getValueScale( void ) const;

    /// Emitter Objects.
    ParticleEmitter* addEmitter( ParticleEmitter* pEmitter = NULL );
    void removeEmitter( SimObjectId emitterID, bool deleteEmitter = true );
    void clearEmitters( void );
    S32 getEmitterCount( void ) const;
    SimObjectId findEmitterObject( const char* emitterName ) const;
    SimObjectId getEmitterObject( S32 index ) const;
    void moveEmitter( S32 fromIndex, S32 toIndex );

    /// Effect Control.
    bool playEffect( bool resetParticles );
    void stopEffect( bool waitForParticles, bool killEffect );
    bool getIsEffectPlaying() { return mEffectPlaying; };
    bool moveEffectTo( const F32 moveTime, const F32 timeStep, U32& peakCount, F32& peakTime );
    bool findParticlePeak( const F32 searchTime, const F32 timeStep, const U32 peakLimit, U32& peakCount, F32& peakTime );
    inline void setEffectPaused( bool effectPaused ) { mEffectPaused = effectPaused; }
    inline bool getEffectPaused( void ) const { return mEffectPaused; }
    inline void setCameraIdleDistance( const F32 idleDistance ) { mCameraIdleDistance = idleDistance; mCameraIdle = false; }
    inline F32 getCameraIdleDistance( void ) const { return mCameraIdleDistance; }

    /// Effect file.
    inline StringTableEntry getEffectFile( void ) const { return mEffectFile; }

    /// Effect Life Mode.
    void setEffectLifeMode( eEffectLifeMode lifeMode, F32 time );
    eEffectLifeMode getEffectLifeMode( void ) const { return mEffectLifeMode; };
    F32 getEffectLifeTime( void ) const { return mEffectLifetime; };
    void setEffectLifeTime( F32 time ) { mEffectLifetime = getMax( time, 0.f ); };

    /// Load/Save Effect.
    bool loadEffect( const char* effectFile );
    bool saveEffect( const char* effectFile );

    /// Collisions.
    inline void setUseEffectCollision( const bool status ) { mUseEffectCollisions = status; }
    bool getUseEffectCollision( void ) const { return mUseEffectCollisions; }

    /// Integration.
    virtual void preIntegrate( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    virtual void integrateObject( const F32 totalTime, const F32 elapsedTime, DebugStats* pDebugStats );
    virtual void interpolateObject( const F32 timeDelta );

    virtual void safeDelete();
    virtual bool onAdd();
    virtual void onRemove();
    virtual void sceneRender( const SceneRenderState* pSceneRenderState, const SceneRenderRequest* pSceneRenderRequest, BatchRender* pBatchRenderer );
    virtual void sceneRenderOverlay( const SceneRenderState* sceneRenderState );
    virtual bool canRender( void ) const { return mEffectPlaying; }

    virtual void copyTo(SimObject* object);

    /// Declare Console Object.
    DECLARE_CONOBJECT(ParticleEffect);

protected:
    virtual void onTamlAddParent( SimObject* pParentObject );

protected:
    static bool setEffectFile(void* obj, const char* data);
    static bool writeEffectFile( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(ParticleEffect); return pCastObject->getEffectFile() == StringTable->EmptyString; }
    static bool setUseEffectCollisions(void* obj, const char* data) { static_cast<ParticleEffect*>(obj)->setUseEffectCollision(dAtob(data)); return false; };
    static bool writeUseEffectCollisions( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(ParticleEffect); return pCastObject->getUseEffectCollision() == true; }
    static bool setEffectMode(void* obj, const char* data);
    static bool writeEffectMode( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(ParticleEffect); return pCastObject->getEffectLifeMode() != INFINITE; }
    static bool setEffectTime(void* obj, const char* data)  { static_cast<ParticleEffect*>(obj)->setEffectLifeTime(dAtof(data)); return false; };
    static bool writeEffectTime( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(ParticleEffect); return pCastObject->getEffectLifeTime() > 0.0f; }
    static bool setCameraIdleDistance(void* obj, const char* data)  { static_cast<ParticleEffect*>(obj)->setCameraIdleDistance(dAtof(data)); return false; };
    static bool writeCameraIdleDistance( void* obj, StringTableEntry pFieldName ) { PREFAB_WRITE_CHECK(ParticleEffect); return pCastObject->getCameraIdleDistance() > 0.0f; }
};

//-----------------------------------------------------------------------------

extern ParticleEffect::eEffectLifeMode getEffectMode(const char* label);

#endif // _PARTICLE_EFFECT_H_
