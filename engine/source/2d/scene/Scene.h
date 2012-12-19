//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _SCENE_H_
#define _SCENE_H_

#ifndef _VECTOR_H_
#include "collection/vector.h"
#endif

#ifndef _MMATH_H_
#include "math/mMath.h"
#endif

#ifndef _VECTOR2_H_
#include "2d/core/Vector2.h"
#endif

#ifndef _FILESTREAM_H_
#include "io/fileStream.h"
#endif

#ifndef _NETOBJECT_H_
#include "network/netObject.h"
#endif

#ifndef _TICKABLE_H_
#include "platform/Tickable.h"
#endif

#ifndef _SCENE_OBJECT_GROUP_H_
#include "2d/sceneobject/SceneObjectGroup.h"
#endif

#ifndef _DEBUG_STATS_H_
#include "2d/scene/DebugStats.h"
#endif

#ifndef _PHYSICS_PROXY_H_
#include "2d/scene/PhysicsProxy.h"
#endif

#ifndef _WORLD_QUERY_H_
#include "2d/scene/WorldQuery.h"
#endif

#ifndef BOX2D_H
#include "box2d\Box2D.h"
#endif

#ifndef _DEBUG_DRAW_H_
#include "2d/scene/DebugDraw.h"
#endif

#ifndef _HASHTABLE_H
#include "collection/hashTable.h"
#endif

#ifndef _BATCH_RENDER_H_
#include "2d/core/BatchRender.h"
#endif

#ifndef _SCENE_RENDER_QUEUE_H_
#include "2d/scene/SceneRenderQueue.h"
#endif

#ifndef _SCENE_RENDER_OBJECT_H_
#include "2d/scene/SceneRenderObject.h"
#endif

//-----------------------------------------------------------------------------

extern EnumTable jointTypeTable;

///-----------------------------------------------------------------------------

class SceneObject;
class SceneWindow;
class SceneObjectGroup;

///-----------------------------------------------------------------------------

struct tDeleteRequest
{
    SimObjectId     mObjectId;
    SceneObject*    mpSceneObject;
    bool            mSafeDeleteReady;
};

struct TickContact
{
    TickContact()
    {
        initialize( NULL, NULL, NULL, NULL, NULL );
    }

    void initialize(
        b2Contact*      pContact,
        SceneObject*    pSceneObjectA,
        SceneObject*    pSceneObjectB,
        b2Fixture*      pFixtureA,
        b2Fixture*      pFixtureB )
    {
        mpContact      = pContact;
        mpSceneObjectA = pSceneObjectA;
        mpSceneObjectB = pSceneObjectB;
        mpFixtureA     = pFixtureA;
        mpFixtureB     = pFixtureB;

        // Get world manifold. 
        if ( mpContact != NULL )
        {
            mPointCount = pContact->GetManifold()->pointCount;
            mpContact->GetWorldManifold( &mWorldManifold );
        }
        else
        {
            mPointCount = 0;
        }

        // Reset impulses.
        for (U32 i = 0; i < b2_maxManifoldPoints; i++)
        {
            mNormalImpulses[i] = 0;
            mTangentImpulses[i] = 0;
        }
    }

    inline SceneObject* getCollideWith( SceneObject* pMe ) const
    {
        return pMe == mpSceneObjectA ? mpSceneObjectB : mpSceneObjectA;
    }

    inline b2Fixture* getCollideWithFixture( b2Fixture* pMe ) const
    {
        return pMe == mpFixtureA ? mpFixtureB : mpFixtureA;
    }

    b2Contact*      mpContact;
    SceneObject*    mpSceneObjectA;
    SceneObject*    mpSceneObjectB;
    b2Fixture*      mpFixtureA;
    b2Fixture*      mpFixtureB;
    U32             mPointCount;
    b2WorldManifold mWorldManifold;
    F32             mNormalImpulses[b2_maxManifoldPoints];
    F32             mTangentImpulses[b2_maxManifoldPoints];
};

///-----------------------------------------------------------------------------

typedef HashMap<U32, b2Joint*>              typeJointHash;
typedef HashMap<U32, U32>                   typeReverseJointHash;
typedef Vector<tDeleteRequest>              typeDeleteVector;
typedef Vector<TickContact>                 typeContactVector;
typedef HashMap<b2Contact*, TickContact>    typeContactHash;

///-----------------------------------------------------------------------------

void findObjectsCallback(SceneObject* pSceneObject, void* storage);
void findLayeredObjectsCallback(SceneObject* pSceneObject, void* storage);
const char* getJointTypeDescription( b2JointType jointType );

///-----------------------------------------------------------------------------

class Scene :
    public SimSet,
    public PhysicsProxy,
    public b2ContactListener,
    public b2DestructionListener,
    public virtual Tickable
{
public:
    /// Scene-Graph Debug Bits.
    enum
    {
        SCENE_DEBUG_STATISTICS         = BIT(0),
        SCENE_DEBUG_JOINTS             = BIT(1),
        ///---
        SCENE_DEBUG_AABB               = BIT(2),
        SCENE_DEBUG_OOBB               = BIT(3),
        SCENE_DEBUG_ASLEEP             = BIT(4),
        SCENE_DEBUG_COLLISION_SHAPES   = BIT(5),
        SCENE_DEBUG_POSITION_AND_COM   = BIT(6),
        SCENE_DEBUG_SORT_POINTS        = BIT(7),
    };

    /// Pick mode.
    enum PickMode
    {
        PICK_INVALID,
        ///---
        PICK_ANY,
        PICK_SIZE,
        PICK_COLLISION,
    };

    /// Debug drawing.
    DebugDraw                mDebugDraw;

private:
    typedef SimSet Parent;

    /// World.
    b2World*                    mpWorld;
    WorldQuery*                 mpWorldQuery;
    b2Vec2                      mWorldGravity;
    S32                         mVelocityIterations;
    S32                         mPositionIterations;
    b2BlockAllocator            mBlockAllocator;
    b2Body*                     mpGroundBody;

    /// Scene occupancy.
    typeSceneObjectVector       mSceneObjects;
    typeSceneObjectVector       mTickedSceneObjects;

    /// Joint access.
    typeJointHash               mJoints;
    typeReverseJointHash        mReverseJoints;
    U32                         mJointMasterId;

    /// Scene time.
    F32                         mSceneTime;
    bool                        mScenePause;

    /// Debug and metrics.
    DebugStats                  mDebugStats;
    U32                         mDebugMask;
    SceneObject*                mpDebugSceneObject;

    /// Layer sorting and draw order.
    SceneRenderQueue::RenderSort mLayerSortModes[MAX_LAYERS_SUPPORTED];

    /// Batch rendering.
    BatchRender                 mBatchRenderer;

    /// Window rendering.
    SceneWindow*                mpCurrentRenderWindow;

    /// Background color.
    ColorF                      mBackgroundColor;
    bool                        mUseBackgroundColor;

    /// Window attachments.
    SimSet                      mAttachedSceneWindows;

    /// Delete requests.
    typeDeleteVector            mDeleteRequests;
    typeDeleteVector            mDeleteRequestsTemp;
  
    /// Miscellaneous.
    S32                         mIsEditorScene;
    bool                        mUpdateCallback;
    typeContactHash             mBeginContacts;
    typeContactVector           mEndContacts;
    U32                         mSceneIndex;

private:   
    /// Contacts.
    void                        forwardContacts( void );
    void                        dispatchBeginContactCallbacks( void );
    void                        dispatchEndContactCallbacks( void );

    /// Joint definition.
    struct CommonJointDefinition
    {
        CommonJointDefinition()
        {
            jointType        = e_unknownJoint;
            collideConnected = false;
            pSceneObjectA    = NULL;
            pSceneObjectB    = NULL;
            localAnchorA     = b2Vec2_zero;
            localAnchorB     = b2Vec2_zero;
        }

        b2JointType         jointType;
        bool                collideConnected;
        SceneObject*        pSceneObjectA;
        SceneObject*        pSceneObjectB;
        b2Vec2              localAnchorA;
        b2Vec2              localAnchorB;
    };

protected:
    /// Taml callbacks.
    virtual void            onTamlPreRead( void );
    virtual void            onTamlPostRead( const TamlCollection& customCollection );
    virtual void            onTamlCustomWrite( TamlCollection& customCollection );
    virtual void            onTamlCustomRead( const TamlCollection& customCollection );

public:
    Scene();
    virtual ~Scene();

    /// Engine.
    virtual bool            onAdd();
    virtual void            onRemove();
    virtual void            onDeleteNotify( SimObject* object );
    static void             initPersistFields();

    /// Contact processing.
    virtual void            PreSolve( b2Contact* pContact, const b2Manifold* pOldManifold ) {}
    virtual void            PostSolve( b2Contact* pContact, const b2ContactImpulse* pImpulse );
    virtual void            BeginContact( b2Contact* pContact );
    virtual void            EndContact( b2Contact* pContact );
    const typeContactHash&  getBeginContacts( void ) const              { return mBeginContacts; }
    const typeContactVector& getEndContacts( void ) const               { return mEndContacts; }

    /// Integration.
    virtual void            processTick();
    virtual void            interpolateTick( F32 delta );
    virtual void            advanceTime( F32 timeDelta ) {};

    /// Render output.
    void                    sceneRender( const SceneRenderState* pSceneRenderState );

    /// World.
    inline b2World*         getWorld( void ) const                      { return mpWorld; }
    inline WorldQuery*      getWorldQuery( const bool clearQuery = false ) { if ( clearQuery ) mpWorldQuery->clearQuery(); return mpWorldQuery; }
    b2BlockAllocator*       getBlockAllocator( void )                   { return &mBlockAllocator; }
    inline b2Body*          getGroundBody( void ) const                 { return mpGroundBody; }
    virtual ePhysicsProxyType getPhysicsProxyType( void ) const         { return PhysicsProxy::PHYSIC_PROXY_GROUNDBODY; }
    void                    setGravity( const b2Vec2& gravity )         { mWorldGravity = gravity; if (mpWorld) mpWorld->SetGravity( gravity ); }
    inline b2Vec2           getGravity( void )                          { if (mpWorld) mWorldGravity = mpWorld->GetGravity(); return mWorldGravity; }
    inline void             setVelocityIterations( const S32 iterations ) { mVelocityIterations = iterations; }
    inline S32              getVelocityIterations( void ) const         { return mVelocityIterations; }
    inline void             setPositionIterations( const S32 iterations ) { mPositionIterations = iterations; }
    inline S32              getPositionIterations( void ) const         { return mPositionIterations; }

    /// Background color.
    inline void             setBackgroundColor( const ColorF& backgroundColor ) { mBackgroundColor = backgroundColor; }
    inline const ColorF&    getBackgroundColor( void ) const            { return mBackgroundColor; }
    inline void             setUseBackgroundColor( const bool useBackgroundColor ) { mUseBackgroundColor = useBackgroundColor; }
    inline bool             getUseBackgroundColor( void ) const         { return mUseBackgroundColor; }

    /// Scene occupancy.
    void                    clearScene( bool deleteObjects = true );
    void                    addToScene( SceneObject* pSceneObject );
    void                    removeFromScene( SceneObject* pSceneObject );

    void                    addToScene( SceneObjectGroup* pSceneObjectGroup );
    void                    removeFromScene( SceneObjectGroup* pSceneObjectGroup );
    void                    addToScene( SimObject* pObject );
    void                    removeFromScene( SimObject* pObject );
    void                    addObject( SimObject* pObject );
    bool                    findChildObject( SimObject* pObject );

    inline typeSceneObjectVectorConstRef getSceneObjects( void ) const  { return mSceneObjects; }
    inline U32              getSceneObjectCount( void ) const           { return mSceneObjects.size(); }
    SceneObject*            getSceneObject( const U32 objectIndex );
    U32                     getSceneObjects( typeSceneObjectVector& objects );
    U32                     getSceneObjects( typeSceneObjectVector& objects, const U32 sceneLayer );
    inline U32              getChildCount( void );

    /// Scene time.
    inline F32              getSceneTime( void ) const                  { return mSceneTime; };
    inline void             setScenePause( bool status )                { mScenePause = status; }
    inline bool             getScenePause( void ) const                 { return mScenePause; };

    /// Prefabs.
    void                    synchronizePrefabs( void );

    /// Joint access.
    inline U32              getJointCount( void ) const                 { return mJoints.size(); }
    b2Joint*                getJoint( const U32 jointId );
    b2JointType             getJointType( const U32 jointId );
    U32                     findJointId( b2Joint* pJoint );
    U32                     createJoint( b2JointDef* pJointDef );
    bool                    deleteJoint( const U32 jointId );
    bool                    hasJoints( SceneObject* pSceneObject );

    /// Distance joint.
    U32                     createDistanceJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA = b2Vec2_zero, const b2Vec2& localAnchorB = b2Vec2_zero,
                                const F32 length = -1.0f,
                                const F32 frequency = 0.0f,
                                const F32 dampingRatio = 0.0f,
                                const bool collideConnected = false );

    void                    setDistanceJointLength(
                                const U32 jointId,
                                const F32 length );

    F32                     getDistanceJointLength( const U32 jointId );

    void                    setDistanceJointFrequency(
                                const U32 jointId,
                                const F32 frequency );

    F32                     getDistanceJointFrequency( const U32 jointId );

    void                    setDistanceJointDampingRatio(
                                const U32 jointId,
                                const F32 dampingRatio );

    F32                     getDistanceJointDampingRatio( const U32 jointId );

    /// Rope joint.
    U32                     createRopeJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA = b2Vec2_zero, const b2Vec2& localAnchorB = b2Vec2_zero,
                                const F32 maxLength = -1.0f,
                                const bool collideConnected = false );

    void                    setRopeJointMaxLength(
                                const U32 jointId,
                                const F32 maxLength );

    F32                     getRopeJointMaxLength( const U32 jointId );

    /// Revolute joint.
    U32                     createRevoluteJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA = b2Vec2_zero, const b2Vec2& localAnchorB = b2Vec2_zero,
                                const bool collideConnected = false );

    void                    setRevoluteJointLimit(
                                const U32 jointId,
                                const bool enableLimit,
                                const F32 lowerAngle, const F32 upperAngle );

    bool                    getRevoluteJointLimit(
                                const U32 jointId,
                                bool& enableLimit,
                                F32& lowerAngle, F32& upperAngle );

    void                    setRevoluteJointMotor(
                                const U32 jointId,
                                const bool enableMotor,
                                const F32 motorSpeed = b2_pi,
                                const F32 maxMotorTorque = 0.0f );

    bool                    getRevoluteJointMotor(
                                const U32 jointId,
                                bool& enableMotor,
                                F32& motorSpeed,
                                F32& maxMotorTorque );
    /// Weld joint.
    U32                     createWeldJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA = b2Vec2_zero, const b2Vec2& localAnchorB = b2Vec2_zero,
                                const F32 frequency = 0.0f,
                                const F32 dampingRatio = 0.0f,
                                const bool collideConnected = false );

    void                    setWeldJointFrequency(
                                const U32 jointId,
                                const F32 frequency );

    F32                     getWeldJointFrequency( const U32 jointId );

    void                    setWeldJointDampingRatio(
                                const U32 jointId,
                                const F32 dampingRatio );

    F32                     getWeldJointDampingRatio( const U32 jointId );

    /// Wheel joint.
    U32                     createWheelJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA, const b2Vec2& localAnchorB,
                                const b2Vec2& worldAxis,
                                const bool collideConnected = false );

    void                    setWheelJointMotor(
                                const U32 jointId,
                                const bool enableMotor,
                                const F32 motorSpeed = b2_pi,
                                const F32 maxMotorTorque = 0.0f );

    bool                    getWheelJointMotor(
                                const U32 jointId,
                                bool& enableMotor,
                                F32& motorSpeed,
                                F32& maxMotorTorque );

    void                    setWheelJointFrequency(
                                const U32 jointId,
                                const F32 frequency );

    F32                     getWheelJointFrequency( const U32 jointId );

    void                    setWheelJointDampingRatio(
                                const U32 jointId,
                                const F32 dampingRatio );

    F32                     getWheelJointDampingRatio( const U32 jointId );

    /// Friction joint.
    U32                     createFrictionJoint(
                                const SceneObject* pSceneObjectA,const  SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA = b2Vec2_zero, const b2Vec2& localAnchorB = b2Vec2_zero,
                                const F32 maxForce = 0.0f,
                                const F32 maxTorque = 0.0f,
                                const bool collideConnected = false );

    void                    setFrictionJointMaxForce(
                                const U32 jointId,
                                const F32 maxForce );

    F32                     getFrictionJointMaxForce( const U32 jointId );

    void                    setFrictionJointMaxTorque(
                                const U32 jointId,
                                const F32 maxTorque );

    F32                     getFrictionJointMaxTorque( const U32 jointId );

    /// Prismatic joint.
    U32                     createPrismaticJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA, const b2Vec2& localAnchorB,
                                const b2Vec2& worldAxis,
                                const bool collideConnected = false );

    void                    setPrismaticJointLimit(
                                const U32 jointId,
                                const bool enableLimit,
                                const F32 lowerTranslation, const F32 upperTranslation );

    bool                    getPrismaticJointLimit(
                                const U32 jointId,
                                bool& enableLimit,
                                F32& lowerTranslation, F32& upperTranslation );

    void                    setPrismaticJointMotor(
                                const U32 jointId,
                                const bool enableMotor,
                                const F32 motorSpeed = b2_pi,
                                const F32 maxMotorForce = 0.0f );

    bool                    getPrismaticJointMotor(
                                const U32 jointId,
                                bool& enableMotor,
                                F32& motorSpeed,
                                F32& maxMotorTorque );

    /// Pulley joint.
    U32                     createPulleyJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2& localAnchorA, const b2Vec2& localAnchorB,
                                const b2Vec2& worldGroundAnchorA, const b2Vec2& worldGroundAnchorB,
                                const F32 ratio,
                                const F32 lengthA = -1.0f, const F32 lengthB = -1.0f,
                                const bool collideConnected = false );

    /// Mouse joint.
    U32                     createTargetJoint(
                                const SceneObject* pSceneObject,
                                const b2Vec2& worldTarget,
                                const F32 maxForce,
                                const F32 frequency = 5.0f,
                                const F32 dampingRatio = 0.7f,
                                const bool collideConnected = false );

    void                    setTargetJointTarget(
                                const U32 jointId,
                                const b2Vec2& worldTarget );

    b2Vec2                  getTargetJointTarget( const U32 jointId );

    void                    setTargetJointMaxForce(
                                const U32 jointId,
                                const F32 maxForce );

    F32                     getTargetJointMaxForce( const U32 jointId );

    void                    setTargetJointFrequency(
                                const U32 jointId,
                                const F32 frequency );

    F32                     getTargetJointFrequency( const U32 jointId );

    void                    setTargetJointDampingRatio(
                                const U32 jointId,
                                const F32 dampingRatio );

    F32                     getTargetJointDampingRatio( const U32 jointId );

    /// Motor Joint.
    U32                     createMotorJoint(
                                const SceneObject* pSceneObjectA, const SceneObject* pSceneObjectB,
                                const b2Vec2 linearOffset = b2Vec2_zero,
                                const F32 angularOffset = 0.0f,
                                const F32 maxForce = 1.0f,
                                const F32 maxTorque = 1.0f,
                                const F32 correctionFactor = 0.3f,
                                const bool collideConnected = false );

    void                    setMotorJointLinearOffset(
                                const U32 jointId,
                                const b2Vec2& linearOffset );

    b2Vec2                  getMotorJointLinearOffset( const U32 jointId );

    void                    setMotorJointAngularOffset(
                                const U32 jointId,
                                const F32 angularOffset );

    F32                     getMotorJointAngularOffset( const U32 jointId );

    void                    setMotorJointMaxForce(
                                const U32 jointId,
                                const F32 maxForce );

    F32                     getMotorJointMaxForce( const U32 jointId );

    void                    setMotorJointMaxTorque(
                                const U32 jointId,
                                const F32 maxTorque );

    F32                     getMotorJointMaxTorque( const U32 jointId );

    /// Debug and metrics.
    inline void             setDebugOn( const U32 debugMask )           { mDebugMask |= debugMask; }
    inline void             setDebugOff( const U32 debugMask )          { mDebugMask &= ~debugMask; }
    inline U32              getDebugMask( void ) const                  { return mDebugMask; }
    DebugStats&             getDebugStats( void )                       { return mDebugStats; }
    inline void             resetDebugStats( void )                     { mDebugStats.reset(); }
    void                    setDebugSceneObject( SceneObject* pSceneObject );
    inline SceneObject*     getDebugSceneObject( void ) const           { return mpDebugSceneObject; }

    /// Layer sorting.
    inline SceneRenderQueue::RenderSort getLayerSortMode( const U32 layer ) { AssertFatal( layer < MAX_LAYERS_SUPPORTED, "Invalid layer" ); return mLayerSortModes[layer]; }

    /// Window attachments.
    void                    attachSceneWindow( SceneWindow* pSceneWindow2D );
    void                    detachSceneWindow( SceneWindow* pSceneWindow2D );
    void                    detachAllSceneWindows( void );
    bool                    isSceneWindowAttached( SceneWindow* pSceneWindow2D );
    inline SimSet&          getAttachedSceneWindows( void ) { return mAttachedSceneWindows; }

    /// Delete requests.
    void                    addDeleteRequest( SceneObject* pSceneObject );
    void                    processDeleteRequests( const bool forceImmediate );

    /// Destruction listeners.
    virtual                 void SayGoodbye( b2Joint* pJoint );
    virtual                 void SayGoodbye( b2Fixture* pFixture );

    /// Miscellaneous.
    inline void             setBatchingEnabled( const bool enabled )    { mBatchRenderer.setBatchEnabled( enabled ); }
    inline bool             getBatchingEnabled( void ) const            { return mBatchRenderer.getBatchEnabled(); }
    inline bool             getIsEditorScene( void ) const              { return ((mIsEditorScene > 0) ? true : false); }
    inline void             setIsEditorScene( bool status )             { mIsEditorScene += (status ? 1 : -1); }
    static U32              getGlobalSceneCount( void );
    inline U32              getSceneIndex( void ) const                 { return mSceneIndex; }
    inline bool             getUpdateCallback( void ) const             { return mUpdateCallback; }

    /// Declare Console Object.
    DECLARE_CONOBJECT(Scene);

protected:
    /// Physics.
    static bool setGravity( void* obj, const char* data )                           { static_cast<Scene*>(obj)->setGravity( Utility::mGetStringElementVector( data, 0 ) ); return false; }
    static const char* getGravity(void* obj, const char* data)                      { return Vector2(static_cast<Scene*>(obj)->getGravity()).scriptThis(); }
    static bool writeGravity( void* obj, StringTableEntry pFieldName )              { return Vector2(static_cast<Scene*>(obj)->getGravity()).notEqual( Vector2::getZero() ); }
    static bool writeVelocityIterations( void* obj, StringTableEntry pFieldName )   { return static_cast<Scene*>(obj)->getVelocityIterations() != 8; }
    static bool writePositionIterations( void* obj, StringTableEntry pFieldName )   { return static_cast<Scene*>(obj)->getPositionIterations() != 3; }
    static bool writeBackgroundColor( void* obj, StringTableEntry pFieldName )      { return static_cast<Scene*>(obj)->mUseBackgroundColor; }
    static bool writeUseBackgroundColor( void* obj, StringTableEntry pFieldName )   { return static_cast<Scene*>(obj)->mUseBackgroundColor; }

    static bool writeLayerSortMode( void* obj, StringTableEntry pFieldName )
    {
        // Fetch layer.
        // The magic "13" here is the offset from the field prefix of "layerSortMode##".
        const U32 layer = dAtoi(pFieldName+13);

        // Just allow the write if an bad parse.
        if ( layer > MAX_LAYERS_SUPPORTED )
            return true;

        return static_cast<Scene*>(obj)->getLayerSortMode( layer ) != SceneRenderQueue::RENDER_SORT_NEWEST;
    }

    // Callbacks.
    static bool writeUpdateCallback( void* obj, StringTableEntry pFieldName )       { return static_cast<Scene*>(obj)->getUpdateCallback(); }

public:
    static SimObjectPtr<Scene> LoadingScene;
};

//-----------------------------------------------------------------------------

extern const char* getJointTypeDescription( b2JointType jointType );
extern b2JointType getJointTypeEnum(const char* label);
extern Scene::PickMode getPickMode(const char* label);
extern const char* getPickModeDescription( Scene::PickMode pickMode );

#endif // _SCENE_H_
