//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

$worldWidth = 20 * 10;
$cameraWidth = 20;
$cameraHeight = 15;
$worldLeft = $worldWidth / -2;
$worldRight = $worldWidth / 2;
$floorLevel = -4.5;
$wheelSpeed = -400;
$DebugBannerOn = false;
$backdropDomain = 31;
$backgroundDomain = 25;
$truckDomain = 20;
$obstacleDomain = 15;
$foregroundDomain = 10;

// -----------------------------------------------------------------------------

function initializeTruckWorld()
{
   // ************************************************************************   
   // Scene Configuration
   // ************************************************************************   
      
   SandboxScene.setGravity( 0, -9.8 );
  

   // ************************************************************************   
   // Camera Configuration
   // ************************************************************************      
   SandboxWindow.setCurrentCameraArea( $cameraWidth/-2, $cameraHeight/2, $cameraWidth/2, $cameraHeight/-2 );
   SandboxWindow.setCurrentCameraPosition( $worldLeft + ($cameraWidth/2) - 10, 0 );
   SandboxWindow.setViewLimitOn( $worldLeft, $cameraHeight/-2, $worldRight, $cameraWidth/2 );
   
   // Background.
   createBackground();
   
   // Floor.
   createFloor();
   
   // Truck.
   createTruck( $worldLeft + ($cameraWidth/6), -2.6 );  
   
   // ************************************************************************   
   // Wrecked cars at start.
   // ************************************************************************   

   createWreckedCar( 1, -90, $floorLevel + 0.75, 0, true );
   createWreckedCar( 2, -85, $floorLevel + 0.75, 0, true );
   createWreckedCar( 3, -82, $floorLevel + 0.75, 0, true );
   createWreckedCar( 1, -87.123, -2.478, 2.537, true );
   createBrick( 3, -87.5, $floorLevel + 0.25, true );     
   createBrick( 4, -87.5, $floorLevel + 0.75, true );     
   createBrick( 2, -79, $floorLevel + 0.25, true );     


   // ************************************************************************   
   // Building with chains.   
   // ************************************************************************   
   
   createForegroundWall( 2, -99, -5 );   
   createForegroundWall( 1, -75.5, -6.5 );  
   createBrokenCementWall( -78, -1.5 );
   createWreckedBuilding( -71.5, -1 );
   createWoodPile( -65, -2.5 );
   createBrickPile( -67, $floorLevel + 0.45 );
   //createBrick( 1, -67, $floorLevel + 0.25, true );     
   //createBrick( 2, -68, $floorLevel + 0.25, true );     
   createForegroundBrickWall( 1, -61, -6 );


   // ************************************************************************   
   // Start of bridge.   
   // ************************************************************************   
   createPlank( 1, -53, $floorLevel + 0.5, 0, true );
   createPlank( 1, -50.1522, -2.3, 21.267, true );
   createWreckedCar( 2, -47, $floorLevel + 1.9, -100, true );
   createWreckedCar( 3, -45.5, $floorLevel + 1.9, 100, true );
   createPlank( 2, -44, $floorLevel + 2, -90, true );
   createPlank( 1, -43, $floorLevel + 2, -90, true );
   createPlank( 2, -42, $floorLevel + 2, -90, true );
   createPlank( 1, -41, $floorLevel + 2, -90, true );  
   createForegroundWall( 2, -42, -4.5 );  
   createBridge( -41, $floorLevel + 4, 40 );
   for ( %n = 0; %n < 10; %n++ )
   {
      %brick = createBrick( getRandom(1,5), -39 + getRandomF(0,16), $floorLevel + 5, false );     
      %brick.setAwake(true);
   }   
   createPlank( 1, -20.5, $floorLevel + 1.5, -90, true );
   createPlank( 3, -19, $floorLevel + 4, 0, true );
   createPlank( 1, -16.5, $floorLevel + 1.5, -90, true );
   createForegroundBrickWall( 2, -19, -6 );
   
   
   createWreckedCar( 1, -12, $floorLevel + 0.75, 0, true );
   createWreckedCar( 2, -7, $floorLevel + 0.75, 0, true );
   createWreckedCar( 3, -4, $floorLevel + 0.75, 0, true );
     
   // ************************************************************************   
   // Start of pyramid.
   // ************************************************************************   
   createPyramid( 2, $floorLevel + 0.25, 19, true );   
   createForegroundWall( 1, 9, -6 );
   createPyramid( 2+21, $floorLevel + 0.25, 13, true );
   createForegroundBrickWall( 1, 9, -7 );
   
   
   // ************************************************************************   
   // Start of brick stacks.
   // ************************************************************************      
   createBrickStack( 45, $floorLevel + 0.25, 10, false );
   createBrickStack( 47, $floorLevel + 0.25, 1, true );
   createBrickStack( 49, $floorLevel + 0.25, 10, false );
     
   createBrickStack( 72, $floorLevel + 0.25, 1, true );
   createBrickStack( 74, $floorLevel + 0.25, 10, false );
   createBrickStack( 76, $floorLevel + 0.25, 1, true );
   createBrickStack( 78, $floorLevel + 0.25, 10, false );
   
}

// -----------------------------------------------------------------------------

function createBackground()
{  
   // ************************************************************************   
   // Atmosphere
   // ************************************************************************   

   %obj = new Sprite();
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:background_day" );
   %obj.setSize( $worldWidth, 75 );
   %obj.setSceneLayer( $backdropDomain );
   %obj.setSceneGroup( $backdropDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );  
   
   // ************************************************************************   
   // Industrial Background
   // ************************************************************************
   
   %obj = new Scroller();
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:industrial_02" );
   %obj.setPosition( 0, -1 );
   %obj.setSize( $worldWidth, 8 );
   %obj.setRepeatX( $worldWidth / 8 );   
   %obj.setSceneLayer( $backgroundDomain );
   %obj.setSceneGroup( $backgroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );    
}

// -----------------------------------------------------------------------------

function createFloor()
{
   // ************************************************************************   
   // Ground
   // ************************************************************************   
   
   $GlobalFloor = new Scroller();
   %obj = $GlobalFloor;
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:woodGround" );
   %obj.setSize( $worldWidth, 3 );
   %obj.setPosition( 0, $floorLevel - (%obj.getSizeY()/2) );
   %obj.setRepeatX( $worldWidth / 12 );   
   %obj.setSceneLayer( $obstacleDomain );
   %obj.setSceneGroup( $obstacleDomain );
   %obj.setCollisionGroups( $obstacleDomain );
   %obj.setAwake( false );
   //%obj.createPolygonBoxCollisionShape( 25, 3 );
   %obj.createEdgeCollisionShape( $worldWidth/-2, 1.5, $worldWidth/2, 1.5 );
   SandboxScene.add( %obj );   
   SandboxScene.setDebugSceneObject( $GlobalFloor );
}

// -----------------------------------------------------------------------------

function createBrokenCementWall( %posX, %posY )
{
   %obj = new Sprite();   
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:brokenCementWall" );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 6, 6 );
   %obj.setSceneLayer( $backgroundDomain-2 );
   %obj.setSceneGroup( $backgroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );
   
   return %obj;   
}

// -----------------------------------------------------------------------------

function createWoodPile( %posX, %posY )
{
   %obj = new Sprite();   
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:woodPile" );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 8, 5 );
   %obj.setSceneLayer( $backgroundDomain-2 );
   %obj.setSceneGroup( $backgroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );
   
   return %obj;   
}

// -----------------------------------------------------------------------------

function createBrickStack( %posX, %posY, %brickCount, %static )
{
   for ( %n = 0; %n < %brickCount; %n++ )
   {
      createBrick( getRandom(1,5), %posX, %posY + (%n*0.5), %static );     
   }      
}

// -----------------------------------------------------------------------------

function createPyramid( %posX, %posY, %brickBaseCount, %static )
{
   if ( %brickBaseCount < 2 )
   {
      echo( "Invalid pyramid brick base count of" SPC %brickBaseCount );
      return;
   }
   
   for( %stack = 0; %stack < %brickBaseCount; %stack++ )
   {
      %stackIndexCount = %brickBaseCount - (%stack*2);
      %stackX = %posX + ( %stack * 1.0 ) + getRandomF(-0.3, 0.3 );
      %stackY = %posY + ( %stack * 0.5 );
      for ( %stackIndex = 0; %stackIndex < %stackIndexCount; %stackIndex++ )
      {
         createBrick( getRandom(1, 5), %stackX + %stackIndex, %stackY, %static );
      }
   }
}

// -----------------------------------------------------------------------------

function createBridge( %posX, %posY, %linkCount )
{
   
   %linkWidth = 0.5;
   %linkHeight = %linkWidth * 0.5;
   %halfLinkWidth = %linkWidth * 0.5;
   
   %rootObj = new Sprite();
   %rootObj.setBodyType( "static" );
   %rootObj.setImage( "TruckToy:cable" );
   %rootObj.setPosition( %posX, %posY );
   %rootObj.setSize( %linkWidth, %linkHeight );
   %rootObj.setSceneLayer( $backgroundDomain-3 );
   %rootObj.setSceneGroup( $obstacleDomain );
   %rootObj.setCollisionSuppress();
   SandboxScene.add( %rootObj );

   %lastLinkObj = %rootObj;
   
   for ( %n = 1; %n <= %linkCount; %n++ )
   {
      %obj = new Sprite();
         
      %obj.setImage( "TruckToy:cable" );
      %obj.setPosition( %posX + (%n*%linkWidth), %posY );
      %obj.setSize( %linkWidth, %linkHeight );
      %obj.setSceneLayer( $backgroundDomain-3 );
      %obj.setSceneGroup( $obstacleDomain );
      
      if ( %n == %linkCount )
      {
         %obj.setBodyType( "static" );
         %obj.setCollisionSuppress();
      }
      else
      {      
         %obj.setCollisionGroups( $obstacleDomain );   
         %obj.setDefaultDensity( 1 );
         %obj.setDefaultFriction( 1.0 );
         %obj.createPolygonBoxCollisionShape( %linkWidth, %linkHeight );
         %obj.setAngularDamping( 1.0 );
         %obj.setLinearDamping( 1.0 );
      }
      //%obj.setDebugOn( 5 );
      SandboxScene.add( %obj );   
      
      SandboxScene.createRevoluteJoint( %lastLinkObj, %obj, %halfLinkWidth, 0, -%halfLinkWidth, 0 );
      %joint = SandboxScene.createMotorJoint( %lastLinkObj, %obj );
      SandboxScene.setMotorJointMaxForce( %joint, 500 );
      %obj.setAwake( false );
      %lastLinkObj.setAwake( false );
      //%obj.setDebugOn( 5 );
      
      %lastLinkObj = %obj;
   }
   
   return %lastLinkObj;        
}

// -----------------------------------------------------------------------------

function createChain( %posX, %posY, %linkCount )
{
   %linkWidth = 0.25;
   %linkHeight = %linkWidth * 2;
   %halfLinkHeight = %linkHeight * 0.5;
   
   %rootObj = new Sprite();
   %rootObj.setBodyType( "static" );
   %rootObj.setImage( "TruckToy:chain" );
   %rootObj.setPosition( %posX, %posY );
   %rootObj.setSize( %linkWidth, %linkHeight );
   %rootObj.setSceneLayer( $backgroundDomain-3 );
   %rootObj.setSceneGroup( $obstacleDomain );
   %rootObj.setCollisionSuppress();
   SandboxScene.add( %rootObj );

   %lastLinkObj = %rootObj;
   
   for ( %n = 1; %n <= %linkCount; %n++ )
   {
      %obj = new Sprite();
      %obj.setImage( "TruckToy:chain" );
      %obj.setPosition( %posX, %posY - (%n*%linkHeight) );
      %obj.setSize( %linkWidth, %linkHeight );
      %obj.setSceneLayer( $backgroundDomain-3 );
      %obj.setSceneGroup( $obstacleDomain );
      %obj.setCollisionGroups( $obstacleDomain );
      %obj.setDefaultDensity( 1 );
      %obj.setDefaultFriction( 0.2 );
      %obj.createPolygonBoxCollisionShape( %linkWidth, %linkHeight );
      %obj.setAngularDamping( 1.0 );
      %obj.setLinearDamping( 0.5 );
      //%obj.setDebugOn( 5 );
      SandboxScene.add( %obj );   
      
      SandboxScene.createRevoluteJoint( %lastLinkObj, %obj, 0, -%halfLinkHeight, 0, %halfLinkHeight, false );
      //%lastLinkObj.setAwake( false );
      //%obj.setAwake( false );
      
      %lastLinkObj = %obj;
   }

   %lastLinkObj.setAwake(false);
   
   return %lastLinkObj;        
}


// -----------------------------------------------------------------------------

function createWreckedBuilding( %posX, %posY )
{
   %obj = new Sprite();   
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:wreckedBuilding" );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 9, 8 );
   %obj.setSceneLayer( $backgroundDomain-1 );
   %obj.setSceneGroup( $backgroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );
   
   createChain( %posX - 3, %posY + 3.4, 10 );   
   createChain( %posX - 1, %posY + 3.2, 10 );   
   createChain( %posX + 1, %posY + 3.0, 10 );   
   createChain( %posX + 3, %posY + 2.8, 10 );   
}

// -----------------------------------------------------------------------------

function createForegroundBrickWall( %wallNumber, %posX, %posY )
{
   if ( %wallNumber < 1 || %wallNumber > 2 )
   {
      echo( "Invalid foreground wall no of" SPC %wallNumber );
      return;
   }

   %image = "TruckToy:brickWall_0" @ %wallNumber;
      
   %obj = new Sprite();   
   %obj.setBodyType( "static" );
      
   %obj.setImage( %image );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 10, 5 );
   %obj.setSceneLayer( $foregroundDomain-1 );
   %obj.setSceneGroup( $foregroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );
      
   
   return %obj;   
}

// -----------------------------------------------------------------------------

function createForegroundWall( %wallNumber, %posX, %posY )
{
   if ( %wallNumber < 1 || %wallNumber > 2 )
   {
      echo( "Invalid foreground wall no of" SPC %wallNumber );
      return;
   }

   %image = "TruckToy:foregroundWall_0" @ %wallNumber;
      
   %obj = new Sprite();   
   %obj.setBodyType( "static" );
      
   %obj.setImage( %image );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 6, 6 );
   %obj.setSceneLayer( $foregroundDomain-1 );
   %obj.setSceneGroup( $foregroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );
   
   return %obj;   
}

// -----------------------------------------------------------------------------

function createBrick( %brickNumber, %posX, %posY, %static )
{
   if ( %brickNumber < 1 || %brickNumber > 5 )
   {
      echo( "Invalid brick no of" SPC %brickNumber );
      return;
   }
   
   %image = "TruckToy:brick_0" @ %brickNumber;
   
   %obj = new Sprite();   
   if ( %static ) %obj.setBodyType( "static" );
   %obj.setImage( %image );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 1, 0.5 );
   %obj.setSceneLayer( $obstacleDomain );
   %obj.setSceneGroup( $obstacleDomain );
   %obj.setCollisionGroups( $obstacleDomain );
   %obj.setDefaultFriction( 1.0 );
   %obj.createPolygonBoxCollisionShape( 1, 0.5 );
   %obj.setAwake( false );
   SandboxScene.add( %obj );
   
   return %obj;
}

// -----------------------------------------------------------------------------

function createBrickPile( %posX, %posY )
{
   %obj = new Sprite();   
   %obj.setBodyType( "static" );
   %obj.setImage( "TruckToy:brickPile" );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 4, 1 );
   %obj.setSceneLayer( $backgroundDomain-3 );
   %obj.setSceneGroup( $backgroundDomain );
   %obj.setCollisionSuppress();
   %obj.setAwake( false );
   %obj.setActive( false );
   SandboxScene.add( %obj );   
}

// -----------------------------------------------------------------------------

function createPlank( %plankNumber, %posX, %posY, %angle, %static )
{
   if ( %plankNumber < 1 || %plankNumber > 3 )
   {
      echo( "Invalid plank no of" SPC %plankNumber );
      return;
   }
   
   %image = "TruckToy:plank_0" @ %plankNumber;
   
   %obj = new Sprite();   
   if ( %static ) %obj.setBodyType( "static" );
   %obj.setImage( %image );
   %obj.setAngle( %angle );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 5, 1 );   
   %obj.setSceneLayer( $obstacleDomain );
   %obj.setSceneGroup( $obstacleDomain );
   %obj.setCollisionGroups( $obstacleDomain );
   %obj.setAwake( false );
   %obj.setDefaultFriction( 1.0 );

   switch$( %plankNumber )
   {  
      case 1: 
         %obj.createPolygonCollisionShape( "-2.5 -0.5 2.5 -0.5 2.5 -0.2 2.0 0.5 -2.5 -0.2" );
      case 2:
         %obj.createPolygonCollisionShape( "-2.5 -0.4 2.4 -0.5 2.4 0.5 0 0.5 -2.1 0.1 -2.5 -0.2" );
      case 3:
         %obj.createPolygonCollisionShape( "-2.5 -0.4 2.5 -0.5 1.9 0.5 -1.8 0.5 -2.5 0" );
   }
   
   //%obj.setDebugOn( 5 );
   SandboxScene.add( %obj );
   
   return %obj;   
}

// -----------------------------------------------------------------------------

function createWreckedCar( %carNumber, %posX, %posY, %angle, %static )
{
   if ( %carNumber < 1 || %carNumber > 3 )
   {
      echo( "Invalid brick no of" SPC %brickNumber );
      return;
   }
   
   %image = "TruckToy:wreckedCar_0" @ %carNumber;
   
   %obj = new Sprite();   
   if ( %static ) %obj.setBodyType( "static" );
   %obj.setImage( %image );
   %obj.setAngle( %angle );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 4, 1.5 );   
   %obj.setSceneLayer( $obstacleDomain );
   %obj.setSceneGroup( $obstacleDomain );
   %obj.setCollisionGroups( $obstacleDomain );
   %obj.setAwake( false );
   %obj.setDefaultFriction( 1.0 );

   switch$( %carNumber )
   {  
      case 1: 
         %obj.createPolygonCollisionShape( "-2 -0.65 0.5 -0.75 2 -0.45 1.9 0.2 0.5 0.65 -0.5 0.6 -2 -0.3" );
      case 2:
         %obj.createPolygonCollisionShape( "-2 -0.75 2 -0.75 2 -0.2 0.4 0.65 -0.9 0.7 -2 0.0" );
      case 3:
         %obj.createPolygonCollisionShape( "-2 -0.65 0 -0.75 2 -0.55 1.8 0.3 0.5 0.75 -0.5 0.75 -2 0.1" );
   }
   
   //%obj.setDebugOn( 5 );
   SandboxScene.add( %obj );
   
   return %obj;
}

// -----------------------------------------------------------------------------

function createTruck( %posX, %posY )
{
   // ************************************************************************   
   // Truck Body.
   // ************************************************************************      
   
   $truckBody = new Sprite();
   $truckBody.setPosition( %posX, %posY );
   $truckBody.setImage( "TruckToy:truckBody" );
   $truckBody.setSize( 5, 2.5 );
   $truckBody.setSceneLayer( $truckDomain );
   $truckBody.setSceneGroup( $obstacleDomain );
   $truckBody.setCollisionGroups( $obstacleDomain );
   $truckBody.createPolygonCollisionShape( "-2 0.2 -2 -0.5 0 -.95 2 -0.5 2 0.0 0 0.7 -1.5 0.7" ); 
   //$truckBody.setDebugOn( 5 );
   SandboxScene.add( $truckBody );
   
   // Mount camera to truck body.
   SandboxWindow.mount( $truckBody, "0 0", 0, true, true );


   // ************************************************************************   
   // Tires.
   // ************************************************************************   

   // Suspension = -1.0 : -1.5   

   // Rear tire.   
   %tireRear = new Sprite();
   %tireRear.setPosition( %posX-1.4, %posY-1.0 );
   %tireRear.setImage( "TruckToy:tires" );
   %tireRear.setSize( 1.7, 1.7 );
   %tireRear.setSceneLayer( $truckDomain-1 );
   %tireRear.setSceneGroup( $obstacleDomain );
   %tireRear.setCollisionGroups( $obstacleDomain );
   %tireRear.setDefaultFriction( 1.0 );
   %tireRear.setDefaultDensity( 3.0 );
   %tireRear.createCircleCollisionShape( 0.8 ); 
   SandboxScene.add( %tireRear );

   // Front tire.
   %tireFront = new Sprite();
   %tireFront.setPosition( %posX+1.7, %posY-1.0 );
   %tireFront.setImage( "TruckToy:tires" );
   %tireFront.setSize( 1.7, 1.7 );
   %tireFront.setSceneLayer( $truckDomain-1 );
   %tireFront.setSceneGroup( $obstacleDomain );
   %tireFront.setCollisionGroups( $obstacleDomain );
   %tireFront.setDefaultFriction( 1.0 );
   %tireFront.setDefaultDensity( 6.0 );
   %tireFront.createCircleCollisionShape( 0.8 ); 
   SandboxScene.add( %tireFront );   
   
   // ************************************************************************   
   // Suspension joints.
   // ************************************************************************   
   //%rearWheelSpring = SandboxScene.createPrismaticJoint( $truckBody, %tireRear, "-1.1 -1.25", "0 0", "0 1" );
   //%frontWheelSpring = SandboxScene.createPrismaticJoint( $truckBody, %tireFront, "1.4 -1.25", "0 0", "0 1" );   
   //SandboxScene.setPrismaticJointLimit( %rearWheelSpring, true, -5, 0 );
   //SandboxScene.setPrismaticJointLimit( %frontWheelSpring, true, -5, 0 );
   
   $rearMotorJoint = SandboxScene.createWheelJoint( $truckBody, %tireRear, "-1.4 -1.25", "0 0", "0 1" );
   $frontMotorJoint = SandboxScene.createWheelJoint( $truckBody, %tireFront, "1.7 -1.25", "0 0", "0 1" );     
   //SandboxScene.setWheelJointMotor( $rearMotorJoint, true, $wheelSpeed, 10000 );
   //SandboxScene.setWheelJointMotor( $frontMotorJoint, true, $wheelSpeed, 10000 );
   
   //$rearMotorJoint = SandboxScene.createRevoluteJoint( $truckBody, %tireRear, "-1.1 -1.25", "0 0" );
   //$frontMotorJoint = SandboxScene.createRevoluteJoint( $truckBody, %tireFront, "1.35, -1.25", "0 0" );
   //SandboxScene.setRevoluteJointMotor( $rearMotorJoint, true, -36, 10000 );
   //SandboxScene.setRevoluteJointMotor( $frontMotorJoint, true, -36, 10000 );
}

// -----------------------------------------------------------------------------

function truckReverse(%val)
{
    if(%val)
    {
        if ( !$truckMoving )
        {
            SandboxScene.setWheelJointMotor( $rearMotorJoint, true, $wheelSpeed*-1, 10000 );
            SandboxScene.setWheelJointMotor( $frontMotorJoint, true, $wheelSpeed*-1, 10000 );
        }
              
        $truckMoving = true;
    }
    else
    {
        if ( $truckMoving )
        {
            SandboxScene.setWheelJointMotor( $rearMotorJoint, true, 0, 10000 );
            SandboxScene.setWheelJointMotor( $frontMotorJoint, true, 0, 10000 );
        }
              
        $truckMoving = false;
    }
}

function truckForward(%val)
{
    if(%val)
    { 
        if ( !$truckMoving )
        {
            SandboxScene.setWheelJointMotor( $rearMotorJoint, true, $wheelSpeed, 10000 );
            SandboxScene.setWheelJointMotor( $frontMotorJoint, true, $wheelSpeed, 10000 );
        }
              
        $truckMoving = true;
    }
    else
    {
        if ( $truckMoving )
        {
            SandboxScene.setWheelJointMotor( $rearMotorJoint, true, 0, 10000 );
            SandboxScene.setWheelJointMotor( $frontMotorJoint, true, 0, 10000 );
        }
              
        $truckMoving = false;
    }
}
