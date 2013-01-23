//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

function createPyramidToy( %scopeSet )
{
	$PyramidBrickCount = 21;
	$GroundWidth = 150;
	
	SandboxWindow.setCurrentCameraZoom( 2 );
	SandboxScene.setGravity( 0, -15 );
	
	createPyramidGround();
	createToyPyramid(-15, -8, $PyramidBrickCount);
	
    // Set the sandbox drag mode.
    setSandboxDragMode( "pull" ); 
}

//-----------------------------------------------------------------------------

function destroyPyramidToy( %scopeSet )
{
}

function createPyramidGround()
{
    %ground = new Scroller();
	%ground.setBodyType( "static" );
	%ground.Image = "PyramidToy:ground";
	%ground.setSize($GroundWidth, 2);
	%ground.setRepeatX( $GroundWidth / 12 );   
	%ground.setPosition(0, -10);
	%ground.createEdgeCollisionShape( $GroundWidth/-2, 1, $GroundWidth/2, 1 );
    SandboxScene.add( %ground );
}

function createToyPyramid( %posX, %posY, %brickBaseCount)
{
   if ( %brickBaseCount < 2 )
   {
      echo( "Invalid pyramid brick base count of" SPC %brickBaseCount );
      return;
   }
   
   for( %stack = 0; %stack < %brickBaseCount; %stack++ )
   {
      %stackIndexCount = %brickBaseCount - (%stack*2);
      %stackX = %posX + ( %stack * 1.5 );
      %stackY = %posY + ( %stack * 1.5 );
      for ( %stackIndex = 0; %stackIndex < %stackIndexCount; %stackIndex++ )
      {
         createCrate( %stackX + (%stackIndex*1.5), %stackY);
      }
   }
}

function createCrate( %posX, %posY)
{
   %imageMap = "PyramidToy:crate";
   %obj = new Sprite();
   %obj.setUseInputEvents(true);
   %obj.setImage( %imageMap );
   %obj.setPosition( %posX, %posY );
   %obj.setSize( 1.5 );
   %obj.setDefaultFriction( 1.0 );
   %obj.createPolygonBoxCollisionShape( 1.5, 1.5 );
   SandboxScene.add( %obj );
}
