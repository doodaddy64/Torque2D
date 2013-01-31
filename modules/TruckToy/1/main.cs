//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

function TruckToy::create( %this )
{
    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", false );
    setSandboxDragModeAvailable( "pull", false );
    
    // Set the drag mode as "off".
    setSandboxDragMode( "off" );
        
    // Activate the package.
    activatePackage( TruckToyPackage );

    TruckToy.CameraWidth = 20;
    TruckToy.CameraHeight = 15;
    TruckToy.WorldWidth = TruckToy.CameraWidth * 10;
    TruckToy.WorldLeft = TruckToy.WorldWidth * -0.5;
    TruckToy.WorldRight = TruckToy.WorldWidth * 0.5;
    TruckToy.FloorLevel = -4.5;
    TruckToy.WheelSpeed = -400;
    TruckToy.BackdropDomain = 31;
    TruckToy.BackgroundDomain = 25;
    TruckToy.TruckDomain = 20;
    TruckToy.ObstacleDomain = 15;
    TruckToy.ForegroundDomain = 10;    
    
    // Reset the toy.
    %this.reset();
}

//-----------------------------------------------------------------------------

function TruckToy::destroy( %this )
{
    // Deactivate the package.
    deactivatePackage( TruckToyPackage );
}

//-----------------------------------------------------------------------------

function TruckToy::reset( %this )
{   
    // Clear the scene.
    SandboxScene.clear();
    
    // Set a typical Earth gravity.
    SandboxScene.setGravity( 0, -9.8 );  

    // Camera Configuration
    SandboxWindow.setCurrentCameraArea( TruckToy.CameraWidth/-2, TruckToy.CameraHeight/2, TruckToy.CameraWidth/2, TruckToy.CameraHeight/-2 );
    SandboxWindow.setCurrentCameraPosition( TruckToy.WorldLeft + (TruckToy.CameraWidth/2) - 10, 0 );
    SandboxWindow.setViewLimitOn( TruckToy.WorldLeft, TruckToy.CameraHeight/-2, TruckToy.WorldRight, TruckToy.CameraWidth/2 );

    // Create the scene contents in a roughly left to right order.      

    // Background.
    createBackground();

    // Floor.
    createFloor();
     
    // Wrecked cars at start.
    createWreckedCar( 1, -90, TruckToy.FloorLevel + 0.75, 0, true );
    createWreckedCar( 2, -85, TruckToy.FloorLevel + 0.75, 0, true );
    createWreckedCar( 3, -82, TruckToy.FloorLevel + 0.75, 0, true );
    createWreckedCar( 1, -87.123, -2.478, 2.537, true );
    createBrick( 3, -87.5, TruckToy.FloorLevel + 0.25, true );     
    createBrick( 4, -87.5, TruckToy.FloorLevel + 0.75, true );     
    createBrick( 2, -79, TruckToy.FloorLevel + 0.25, true );     


    // Building with chains.   
    createForegroundWall( 2, -99, -5 );   
    createForegroundWall( 1, -75.5, -6.5 );  
    createBrokenCementWall( -78, -1.5 );
    createWreckedBuilding( -71.5, -1 );
    createWoodPile( -65, -2.5 );
    createBrickPile( -67, TruckToy.FloorLevel + 0.45 );
    createForegroundBrickWall( 1, -61, -6 );


    // Start of bridge.   
    createPlank( 1, -53, TruckToy.FloorLevel + 0.5, 0, true );
    createPlank( 1, -50.1522, -2.3, 21.267, true );
    createWreckedCar( 2, -47, TruckToy.FloorLevel + 1.9, -100, true );
    createWreckedCar( 3, -45.5, TruckToy.FloorLevel + 1.9, 100, true );
    createPlank( 2, -44, TruckToy.FloorLevel + 2, -90, true );
    createPlank( 1, -43, TruckToy.FloorLevel + 2, -90, true );
    createPlank( 2, -42, TruckToy.FloorLevel + 2, -90, true );
    createPlank( 1, -41, TruckToy.FloorLevel + 2, -90, true );  
    createForegroundWall( 2, -42, -4.5 );  
    createBridge( -41, TruckToy.FloorLevel + 4, 40 );
    for ( %n = 0; %n < 10; %n++ )
    {
      %brick = createBrick( getRandom(1,5), -39 + getRandomF(0,16), TruckToy.FloorLevel + 5, false );     
      %brick.setAwake(true);
    }   
    createPlank( 1, -20.5, TruckToy.FloorLevel + 1.5, -90, true );
    createPlank( 3, -19, TruckToy.FloorLevel + 4, 0, true );
    createPlank( 1, -16.5, TruckToy.FloorLevel + 1.5, -90, true );
    createForegroundBrickWall( 2, -19, -6 );

    // More wrecked cars.
    createWreckedCar( 1, -12, TruckToy.FloorLevel + 0.75, 0, true );
    createWreckedCar( 2, -7, TruckToy.FloorLevel + 0.75, 0, true );
    createWreckedCar( 3, -4, TruckToy.FloorLevel + 0.75, 0, true );
     
    // ************************************************************************   
    // Start of pyramid.
    // ************************************************************************   
    createPyramid( 2, TruckToy.FloorLevel + 0.25, 19, true );   
    createForegroundWall( 1, 9, -6 );
    createPyramid( 2+21, TruckToy.FloorLevel + 0.25, 13, true );
    createForegroundBrickWall( 1, 9, -7 );


    // ************************************************************************   
    // Start of brick stacks.
    // ************************************************************************      
    createBrickStack( 45, TruckToy.FloorLevel + 0.25, 10, false );
    createBrickStack( 47, TruckToy.FloorLevel + 0.25, 1, true );
    createBrickStack( 49, TruckToy.FloorLevel + 0.25, 10, false );
     
    createBrickStack( 72, TruckToy.FloorLevel + 0.25, 1, true );
    createBrickStack( 74, TruckToy.FloorLevel + 0.25, 10, false );
    createBrickStack( 76, TruckToy.FloorLevel + 0.25, 1, true );
    createBrickStack( 78, TruckToy.FloorLevel + 0.25, 10, false );

    // Truck.
    createTruck( TruckToy.WorldLeft + (TruckToy.CameraWidth/6), 15 );     
}

// -----------------------------------------------------------------------------

function createBackground()
{  
    // Atmosphere
    %obj = new Sprite();
    %obj.setBodyType( "static" );
    %obj.setImage( "TruckToy:background_day" );
    %obj.setSize( TruckToy.WorldWidth * (TruckToy.CameraWidth*2), 75 );
    %obj.setSceneLayer( TruckToy.BackdropDomain );
    %obj.setSceneGroup( TruckToy.BackdropDomain );
    %obj.setCollisionSuppress();
    %obj.setAwake( false );
    %obj.setActive( false );
    SandboxScene.add( %obj );  


    // Industrial Background
    %obj = new Scroller();
    %obj.setBodyType( "static" );
    %obj.setImage( "TruckToy:industrial_02" );
    %obj.setPosition( 0, -1 );
    %obj.setSize( TruckToy.WorldWidth, 8 );
    %obj.setRepeatX( TruckToy.WorldWidth / 8 );   
    %obj.setSceneLayer( TruckToy.BackgroundDomain);
    %obj.setSceneGroup( TruckToy.BackgroundDomain);
    %obj.setCollisionSuppress();
    %obj.setAwake( false );
    %obj.setActive( false );
    SandboxScene.add( %obj );    
}

// -----------------------------------------------------------------------------

function createFloor()
{
    // Ground  
    %obj = new Scroller();
    %obj.setBodyType( "static" );
    %obj.setImage( "ToyAssets:woodGround" );
    %obj.setSize( TruckToy.WorldWidth, 3 );
    %obj.setPosition( 0, TruckToy.FloorLevel - (%obj.getSizeY()/2) );
    %obj.setRepeatX( TruckToy.WorldWidth / 12 );   
    %obj.setSceneLayer( TruckToy.ObstacleDomain );
    %obj.setSceneGroup( TruckToy.ObstacleDomain );
    %obj.setCollisionGroups( TruckToy.ObstacleDomain );
    %obj.setAwake( false );
    %obj.createEdgeCollisionShape( TruckToy.WorldWidth/-2, 1.5, TruckToy.WorldWidth/2, 1.5 );
    %obj.createEdgeCollisionShape( TruckToy.WorldWidth/-2, 3, TruckToy.WorldWidth/-2, 50 );
    %obj.createEdgeCollisionShape( TruckToy.WorldWidth/2, 3, TruckToy.WorldWidth/2, 50 );
    SandboxScene.add( %obj );   
}

// -----------------------------------------------------------------------------

function createBrokenCementWall( %posX, %posY )
{
    %obj = new Sprite();   
    %obj.setBodyType( "static" );
    %obj.setImage( "TruckToy:brokenCementWall" );
    %obj.setPosition( %posX, %posY );
    %obj.setSize( 6, 6 );
    %obj.setSceneLayer( TruckToy.BackgroundDomain-2 );
    %obj.setSceneGroup( TruckToy.BackgroundDomain);
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
    %obj.setSceneLayer( TruckToy.BackgroundDomain-2 );
    %obj.setSceneGroup( TruckToy.BackgroundDomain);
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
   %rootObj.setImage( "ToyAssets:cable" );
   %rootObj.setPosition( %posX, %posY );
   %rootObj.setSize( %linkWidth, %linkHeight );
   %rootObj.setSceneLayer( TruckToy.BackgroundDomain-3 );
   %rootObj.setSceneGroup( TruckToy.ObstacleDomain );
   %rootObj.setCollisionSuppress();
   SandboxScene.add( %rootObj );

   %lastLinkObj = %rootObj;
   
   for ( %n = 1; %n <= %linkCount; %n++ )
   {
      %obj = new Sprite();
         
      %obj.setImage( "ToyAssets:cable" );
      %obj.setPosition( %posX + (%n*%linkWidth), %posY );
      %obj.setSize( %linkWidth, %linkHeight );
      %obj.setSceneLayer( TruckToy.BackgroundDomain-3 );
      %obj.setSceneGroup( TruckToy.ObstacleDomain );
      
      if ( %n == %linkCount )
      {
         %obj.setBodyType( "static" );
         %obj.setCollisionSuppress();
      }
      else
      {      
         %obj.setCollisionGroups( TruckToy.ObstacleDomain );   
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
    %rootObj.setImage( "ToyAssets:chain" );
    %rootObj.setPosition( %posX, %posY );
    %rootObj.setSize( %linkWidth, %linkHeight );
    %rootObj.setSceneLayer( TruckToy.BackgroundDomain-3 );
    %rootObj.setSceneGroup( TruckToy.ObstacleDomain );
    %rootObj.setCollisionSuppress();
    SandboxScene.add( %rootObj );

    %lastLinkObj = %rootObj;

    for ( %n = 1; %n <= %linkCount; %n++ )
    {
        %obj = new Sprite();
        %obj.setImage( "ToyAssets:chain" );
        %obj.setPosition( %posX, %posY - (%n*%linkHeight) );
        %obj.setSize( %linkWidth, %linkHeight );
        %obj.setSceneLayer( TruckToy.BackgroundDomain-3 );
        %obj.setSceneGroup( TruckToy.ObstacleDomain );
        %obj.setCollisionGroups( TruckToy.ObstacleDomain );
        %obj.setDefaultDensity( 1 );
        %obj.setDefaultFriction( 0.2 );
        %obj.createPolygonBoxCollisionShape( %linkWidth, %linkHeight );
        %obj.setAngularDamping( 1.0 );
        %obj.setLinearDamping( 0.5 );
        SandboxScene.add( %obj );   

        SandboxScene.createRevoluteJoint( %lastLinkObj, %obj, 0, -%halfLinkHeight, 0, %halfLinkHeight, false );

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
    %obj.setSceneLayer( TruckToy.BackgroundDomain-1 );
    %obj.setSceneGroup( TruckToy.BackgroundDomain);
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
    %obj.setSceneLayer( TruckToy.ForegroundDomain-1 );
    %obj.setSceneGroup( TruckToy.ForegroundDomain );
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
    %obj.setSceneLayer( TruckToy.ForegroundDomain-1 );
    %obj.setSceneGroup( TruckToy.ForegroundDomain );
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

    %image = "ToyAssets:brick_0" @ %brickNumber;

    %obj = new Sprite();   
    if ( %static ) %obj.setBodyType( "static" );
    %obj.setImage( %image );
    %obj.setPosition( %posX, %posY );
    %obj.setSize( 1, 0.5 );
    %obj.setSceneLayer( TruckToy.ObstacleDomain );
    %obj.setSceneGroup( TruckToy.ObstacleDomain );
    %obj.setCollisionGroups( TruckToy.ObstacleDomain );
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
    %obj.setSceneLayer( TruckToy.BackgroundDomain-3 );
    %obj.setSceneGroup( TruckToy.BackgroundDomain);
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
    %obj.setSceneLayer( TruckToy.ObstacleDomain );
    %obj.setSceneGroup( TruckToy.ObstacleDomain );
    %obj.setCollisionGroups( TruckToy.ObstacleDomain );
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
    %obj.setSceneLayer( TruckToy.ObstacleDomain );
    %obj.setSceneGroup( TruckToy.ObstacleDomain );
    %obj.setCollisionGroups( TruckToy.ObstacleDomain );
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
    // Truck Body.
    %exhaustParticles = new ParticlePlayer();
    %exhaustParticles.setPosition( %posX-3, %posY );
    %exhaustParticles.setSceneLayer( TruckToy.TruckDomain );
    %exhaustParticles.Particle = "TruckToy:exhaust";
    %exhaustParticles.SizeScale = 0.1;
    %exhaustParticles.ForceScale = 0.4;
    %exhaustParticles.EmissionRateScale = 4;
    SandboxScene.add( %exhaustParticles );
    %exhaustParticles.play();
    TruckToy.TruckExhaust = %exhaustParticles;

    TruckToy.TruckBody = new Sprite();
    TruckToy.TruckBody.setPosition( %posX, %posY );
    TruckToy.TruckBody.setImage( "TruckToy:truckBody" );
    TruckToy.TruckBody.setSize( 5, 2.5 );
    TruckToy.TruckBody.setSceneLayer( TruckToy.TruckDomain );
    TruckToy.TruckBody.setSceneGroup( TruckToy.ObstacleDomain );
    TruckToy.TruckBody.setCollisionGroups( TruckToy.ObstacleDomain );
    TruckToy.TruckBody.createPolygonCollisionShape( "-2 0.2 -2 -0.5 0 -.95 2 -0.5 2 0.0 0 0.7 -1.5 0.7" ); 
    //TruckToy.TruckBody.setDebugOn( 5 );
    SandboxScene.add( TruckToy.TruckBody );

    // Attach the exhaust output to the truck body.   
    %joint = SandboxScene.createRevoluteJoint( TruckToy.TruckBody, TruckToy.TruckExhaust, "-2.3 -0.6", "0 0" );
    SandboxScene.setRevoluteJointLimit( %joint, 0, 0 );


    // Mount camera to truck body.
    SandboxWindow.mount( TruckToy.TruckBody, "0 0", 0, true, true );

    //SandboxScene.setDebugSceneObject( TruckToy.TruckBody );


    // Tires.
    // Suspension = -1.0 : -1.5   

    // Rear tire.   
    %tireRear = new Sprite();
    %tireRear.setPosition( %posX-1.4, %posY-1.0 );
    %tireRear.setImage( "ToyAssets:tires" );
    %tireRear.setSize( 1.7, 1.7 );
    %tireRear.setSceneLayer( TruckToy.TruckDomain-1 );
    %tireRear.setSceneGroup( TruckToy.ObstacleDomain );
    %tireRear.setCollisionGroups( TruckToy.ObstacleDomain );
    %tireRear.setDefaultFriction( 1.0 );
    %tireRear.setDefaultDensity( 3.0 );
    %tireRear.createCircleCollisionShape( 0.8 ); 
    SandboxScene.add( %tireRear );

    // Front tire.
    %tireFront = new Sprite();
    %tireFront.setPosition( %posX+1.7, %posY-1.0 );
    %tireFront.setImage( "ToyAssets:tires" );
    %tireFront.setSize( 1.7, 1.7 );
    %tireFront.setSceneLayer( TruckToy.TruckDomain-1 );
    %tireFront.setSceneGroup( TruckToy.ObstacleDomain );
    %tireFront.setCollisionGroups( TruckToy.ObstacleDomain );
    %tireFront.setDefaultFriction( 1.0 );
    %tireFront.setDefaultDensity( 6.0 );
    %tireFront.createCircleCollisionShape( 0.8 ); 
    SandboxScene.add( %tireFront );   

    // Suspension joints.
    TruckToy.RearMotorJoint = SandboxScene.createWheelJoint( TruckToy.TruckBody, %tireRear, "-1.4 -1.25", "0 0", "0 1" );
    TruckToy.FrontMotorJoint = SandboxScene.createWheelJoint( TruckToy.TruckBody, %tireFront, "1.7 -1.25", "0 0", "0 1" );     
}


// -----------------------------------------------------------------------------

function truckForward(%val)
{
    if(%val)
    { 
        if ( !TruckToy.TruckMoving )
        {
            SandboxScene.setWheelJointMotor( TruckToy.RearMotorJoint, true, TruckToy.WheelSpeed, 10000 );
            SandboxScene.setWheelJointMotor( TruckToy.FrontMotorJoint, true, TruckToy.WheelSpeed, 10000 );
            TruckToy.TruckExhaust.SizeScale *= 4;
            TruckToy.TruckExhaust.ForceScale /= 2;
        }
              
        TruckToy.TruckMoving = true;
    }
    else
    {
        truckStop();
    }
}

// -----------------------------------------------------------------------------

function truckReverse(%val)
{
    if(%val)
    {
        if ( !TruckToy.TruckMoving )
        {
            SandboxScene.setWheelJointMotor( TruckToy.RearMotorJoint, true, TruckToy.WheelSpeed*-1, 10000 );
            SandboxScene.setWheelJointMotor( TruckToy.FrontMotorJoint, true, TruckToy.WheelSpeed*-1, 10000 );
            TruckToy.TruckExhaust.SizeScale *= 4;
            TruckToy.TruckExhaust.ForceScale /= 2;
        }
              
        TruckToy.TruckMoving = true;
    }
    else
    {
        truckStop();
    }
}

//-----------------------------------------------------------------------------

function truckStop()
{
    // Finish if truck is not moving.
    if ( !TruckToy.TruckMoving )
        return;

    // Stop truck moving.
    SandboxScene.setWheelJointMotor( TruckToy.RearMotorJoint, true, 0, 10000 );
    SandboxScene.setWheelJointMotor( TruckToy.FrontMotorJoint, true, 0, 10000 );
    TruckToy.TruckExhaust.SizeScale /= 4;
    TruckToy.TruckExhaust.ForceScale *= 2;

    // Flag truck as not moving.    
    TruckToy.TruckMoving = false;
}

//-----------------------------------------------------------------------------

package TruckToyPackage
{

function SandboxWindow::onTouchDown(%this, %touchID, %worldPos)
{
    // Finish if truck is already moving.
    if ( TruckToy.TruckMoving )
        return;

    // If we touch in-front of the truck then move forward else reverse.
    if ( getWord(%worldPos,0) >= TruckToy.TruckBody.getPositionX() )
    {
        truckForward( true );
    }
    else
    {
        truckReverse( true );
    }
}

//-----------------------------------------------------------------------------

function SandboxWindow::onTouchUp(%this, %touchID, %worldPos)
{
    // Stop the truck.
    truckStop();
}
    
};
