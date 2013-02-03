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

function MelvToy::particleTest( %this )
{
    SandboxWindow.setCurrentCameraSize( 20, 15 );
    
    %particleAssetName = "Test";
    
    %effectAsset = new ParticleAsset();
    %effectAsset.assetName = %particleAssetName;   
    
    %effectAsset.LifeMode = "infinite";    
    //%effectAsset.LifeMode = "kill";    
    //%effectAsset.Lifetime = 1;

    //%effectAsset.selectField( "EmissionArc" );    
        //%effectAsset.setSingleDataKey( 15 );

    //%effectAsset.selectField( "EmissionAngle" );    
        //%effectAsset.setSingleDataKey( 180 );
    
    // ---------------------------------------------------------
           
    %emitter = %effectAsset.createEmitter();
    %emitter.EmitterName = "flames";
    %emitter.EmitterType = "line";
    %emitter.EmitterSize = "1 0";
    %emitter.EmitterAngle = 0;
    %emitter.EmitterOffset = "0 0";
    %emitter.IntenseParticles = true;
    //%emitter.Animation = "ToyAssets:Projectile_FireballAnim";
    %emitter.Animation = "ToyAssets:Projectile_Meteor_StrikeAnimation";
    //%emitter.Image = "ToyAssets:Impact_ExplosionSprite";
    //%emitter.Image = "ToyAssets:Particles2";
    //%emitter.Frame = 3;
    %emitter.FixedAspect = true;
    %emitter.RandomImageFrame = false;
    %emitter.OldestInFront = true;
    %emitter.AttachPositionToEmitter = false;
    %emitter.AttachRotationToEmitter = false;

    %emitter.selectField( "Quantity" );    
        %emitter.addDataKey( 0, 20 );
        
    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 1 );

    //%emitter.selectField( "LifetimeVariation" ); 
        //%emitter.setSingleDataKey( 1 );

    %emitter.selectField( "Speed" );    
        %emitter.setSingleDataKey( 0.1 );

    %emitter.selectField( "SpeedVariation" );    
        %emitter.setSingleDataKey( 0 );

    %emitter.selectField( "SizeXVariation" );
      %emitter.setSingleDataKey( 0.3 );
       
    %emitter.selectField( "SizeXLife" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.1, 0.5 );
        //%emitter.addDataKey( 0.8, 1 );
        //%emitter.addDataKey( 1, 2 );
        
    %emitter.selectField( "EmissionAngle" );
        %emitter.setSingleDataKey( 90 );        

    %emitter.selectField( "EmissionArc" );
        %emitter.setSingleDataKey( 30 );        

    //%emitter.selectField( "Spin" );    
        //%emitter.setSingleDataKey( 0 );

    %emitter.selectField( "SpinVariation" );
        %emitter.setSingleDataKey( 360 );
        
    %emitter.selectField( "RandomMotion" );
        %emitter.setSingleDataKey( 10 );

    %emitter.selectField( "FixedForce" );
        %emitter.setSingleDataKey( 30 );
        %emitter.FixedForceAngle = 90;

    %emitter.selectField( "FixedForceVariation" );
        %emitter.setSingleDataKey( 10 );

    //%emitter.selectField( "RedChannel" );
        //%emitter.addDataKey( 0, 0 );
        //%emitter.addDataKey( 1, 0.5 );    
//
    //%emitter.selectField( "GreenChannel" );
        //%emitter.addDataKey( 0, 0 );
        //%emitter.addDataKey( 0.3, 1 );    
        //%emitter.addDataKey( 1, 0 );    
//
    //%emitter.selectField( "BlueChannel" );
        //%emitter.addDataKey( 0, 1 );
        //%emitter.addDataKey( 0.6, 1 );    
        //%emitter.addDataKey( 1, 0 );    

    %emitter.selectField( "AlphaChannel" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.1, 1 );
        %emitter.addDataKey( 0.8, 1 );
        %emitter.addDataKey( 1, 0 );    
        
    %emitter.deselectField();
             
    // ---------------------------------------------------------

    
    %assetFilePath = expandPath( "^MelvToy/particle.asset.taml" );
    
	TamlWrite( %effectAsset, %assetFilePath );
	
    if ( !AssetDatabase.addSingleDeclaredAsset( ModuleDatabase.findLoadedModule( "MelvToy" ), %assetFilePath ) )
    {
        error( "Could not load the asset file:" SPC %assetFilePath );
    }

    //TamlRead( %assetFilePath );   
    
    %particlePlayer = new ParticlePlayer();
    %particlePlayer.Position = "0 0";
    %particlePlayer.Size = 10;
    %particlePlayer.ParticleInterpolation = true;
    //%particlePlayer.CameraIdleDistance = 50;
    %particlePlayer.Particle = "MelvToy:" @ %particleAssetName;
    SandboxScene.add( %particlePlayer );
    //%particlePlayer.play();
    //%particlePlayer.visible = false;
    //%particlePlayer.setLinearVelocity( 10, 0 );
    //%particlePlayer.setAngularVelocity( -180 );
    
    //SandboxScene.setDebugSceneObject( %particlePlayer );
    
    //%particlePlayerFilePath = expandPath( "^MelvToy/particlePlayer.taml" );
    //TamlWrite( %particlePlayer, %particlePlayerFilePath );
    //TamlRead( %particlePlayerFilePath );
    
    
    //schedule( 5000, 0, changeParticleAsset );
    
    //SandboxScene.setBatchingEnabled( true );
    
    //TamlWrite( SandboxScene, "scene.taml" );
    //%newScene = TamlRead( "scene.taml" );
	
	//quit();
}
