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
    
    //%effectAsset.LifeMode = "infinite";    
    %effectAsset.LifeMode = "kill";    
    %effectAsset.Lifetime = 1;

    //%effectAsset.selectField( "EmissionArc" );    
        //%effectAsset.setSingleDataKey( 15 );

    //%effectAsset.selectField( "EmissionAngle" );    
        //%effectAsset.setSingleDataKey( 180 );
    
    // ---------------------------------------------------------   
    
    %emitter2 = %effectAsset.createEmitter();
    %emitter2.EmitterName = "debris";
    %emitter2.EmitterType = "box";
    %emitter2.EmitterSize = "2 1";
    %emitter2.EmitterAngle = 90;
    %emitter2.EmitterOffset = "0 0";
    %emitter2.IntenseParticles = false;
    %emitter2.Image = "ToyAssets:Asteroids";
    %emitter2.RandomImageFrame = true;
    %emitter2.FixedAspect = true;
    %emitter2.OldestInFront = true;
    %emitter2.AttachPositionToEmitter = false;
    %emitter2.AttachRotationToEmitter = false;

    %emitter2.selectField( "Quantity" );    
        %emitter2.addDataKey( 0, 150 );
        %emitter2.addDataKey( 0.1, 150 );
        %emitter2.addDataKey( 0.11, 0 );
        
    %emitter2.selectField( "Lifetime" ); 
        %emitter2.setSingleDataKey( 2 );

    %emitter2.selectField( "LifetimeVariation" ); 
        %emitter2.setSingleDataKey( 1 );

    %emitter2.selectField( "Speed" );    
        %emitter2.setSingleDataKey( 0.8 );

    %emitter2.selectField( "SpeedVariation" );    
        %emitter2.setSingleDataKey( 0.3 );

    %emitter2.selectField( "SizeXVariation" );
        %emitter2.setSingleDataKey( 2.1 );
       
    %emitter2.selectField( "SizeXLife" );
        %emitter2.addDataKey( 0, 0 );
        %emitter2.addDataKey( 0.1, 0.1 );
        //%emitter2.addDataKey( 0.8, 1 );
        //%emitter2.addDataKey( 1, 2 );
        
    %emitter2.selectField( "EmissionAngle" );
        %emitter2.setSingleDataKey( 90 );        

    %emitter2.selectField( "EmissionArc" );
        %emitter2.setSingleDataKey( 90 );

    //%emitter2.selectField( "Spin" );    
        //%emitter2.setSingleDataKey( 0 );

    %emitter2.selectField( "SpinVariation" );
        %emitter2.setSingleDataKey( 360 );
        
    %emitter2.selectField( "RandomMotion" );
        %emitter2.setSingleDataKey( 10 );

    %emitter2.selectField( "FixedForce" );
        %emitter2.setSingleDataKey( 8 );
        %emitter2.FixedForceAngle = -90;

    %emitter2.selectField( "FixedForceVariation" );
        %emitter2.setSingleDataKey( 2 );

    //%emitter2.selectField( "RedChannel" );
        //%emitter2.addDataKey( 0, 0 );
        //%emitter2.addDataKey( 1, 0.5 );    
//
    //%emitter2.selectField( "GreenChannel" );
        //%emitter2.addDataKey( 0, 0 );
        //%emitter2.addDataKey( 0.3, 1 );    
        //%emitter2.addDataKey( 1, 0 );    
//
    //%emitter2.selectField( "BlueChannel" );
        //%emitter2.addDataKey( 0, 1 );
        //%emitter2.addDataKey( 0.6, 1 );    
        //%emitter2.addDataKey( 1, 0 );    

    %emitter2.selectField( "AlphaChannel" );
        %emitter2.addDataKey( 0, 0 );
        %emitter2.addDataKey( 0.1, 1 );
        %emitter2.addDataKey( 0.8, 1 );
        %emitter2.addDataKey( 1, 0 );    


    // ---------------------------------------------------------
           
    %emitter = %effectAsset.createEmitter();
    %emitter.EmitterName = "flames";
    %emitter.EmitterType = "line";
    %emitter.EmitterSize = "1 0";
    %emitter.EmitterAngle = 0;
    %emitter.EmitterOffset = "0 0";
    %emitter.IntenseParticles = true;
    //%emitter.Animation = "ToyAssets:Projectile_FireballAnim";
    %emitter.Animation = "ToyAssets:Impact_ExplosionAnimation";
    //%emitter.Image = "ToyAssets:Impact_ExplosionSprite";
    //%emitter.Image = "ToyAssets:Particles4";
    //%emitter.Frame = 0;
    %emitter.FixedAspect = true;
    %emitter.RandomImageFrame = false;
    %emitter.OldestInFront = true;
    %emitter.AttachPositionToEmitter = false;
    %emitter.AttachRotationToEmitter = false;

    %emitter.selectField( "Quantity" );    
        %emitter.addDataKey( 0, 20 );
        %emitter.addDataKey( 0.1, 20 );
        %emitter.addDataKey( 0.11, 0 );
        
    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 1 );

    //%emitter.selectField( "LifetimeVariation" ); 
        //%emitter.setSingleDataKey( 1 );

    %emitter.selectField( "Speed" );    
        %emitter.setSingleDataKey( 0 );

    %emitter.selectField( "SpeedVariation" );    
        %emitter.setSingleDataKey( 0 );

    %emitter.selectField( "SizeXVariation" );
        %emitter.setSingleDataKey( 2 );
       
    %emitter.selectField( "SizeXLife" );
        %emitter.addDataKey( 0, 0 );
        %emitter.addDataKey( 0.1, 3 );
        //%emitter.addDataKey( 0.8, 1 );
        //%emitter.addDataKey( 1, 2 );
        
    //%emitter.selectField( "EmissionAngle" );
        //%emitter.setSingleDataKey( 90 );        

    //%emitter.selectField( "EmissionArc" );
        //%emitter.setSingleDataKey( 30 );        

    //%emitter.selectField( "Spin" );    
        //%emitter.setSingleDataKey( 0 );

    //%emitter.selectField( "SpinVariation" );
        //%emitter.setSingleDataKey( 360 );
        
    //%emitter.selectField( "RandomMotion" );
        //%emitter.setSingleDataKey( 80 );

    //%emitter.selectField( "FixedForce" );
        //%emitter.setSingleDataKey( 1 );
        //%emitter.FixedForceAngle = 90;

    //%emitter.selectField( "FixedForceVariation" );
        //%emitter.setSingleDataKey( 1 );

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
