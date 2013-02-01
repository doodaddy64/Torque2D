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
    %particleAssetName = "Test";
    
    %effectAsset = new ParticleAsset();
    %effectAsset.assetName = %particleAssetName;   
    
    %effectAsset.LifeMode = "infinite";    
    //%effectAsset.Lifetime = 1;        

    //%effectAsset.selectField( "EmissionArc" );    
        //%effectAsset.setSingleDataKey( 15 );

    //%effectAsset.selectField( "EmissionAngle" );    
        //%effectAsset.setSingleDataKey( 180 );

           
    %emitter = %effectAsset.createEmitter();
    %emitter.EmitterName = "emitter1";
    %emitter.EmitterType = "box";
    %emitter.EmitterSize = "1 1";
    %emitter.EmitterAngle = 0;
    %emitter.EmitterOffset = "0 0";
    %emitter.IntenseParticles =false;
    %emitter.Image = "ToyAssets:football";
    %emitter.Frame = 0;
    %emitter.FixedAspect = true;
    %emitter.RandomImageFrame = false;
    %emitter.OldestInFront = false;
    %emitter.AttachPositionToEmitter = false;
    %emitter.AttachRotationToEmitter = false;

    %emitter.selectField( "Quantity" );    
        %emitter.setSingleDataKey( 50 );

    %emitter.selectField( "Lifetime" ); 
        %emitter.setSingleDataKey( 3 );

    //%emitter.selectField( "LifetimeVariation" ); 
        //%emitter.setSingleDataKey( 1 );

    %emitter.selectField( "Speed" );    
        %emitter.setSingleDataKey( 5 );

    //%emitter.selectField( "SpeedVariation" );    
        //%emitter.setSingleDataKey( 3 );
       
    %emitter.selectField( "SizeXLife" );
        %emitter.setSingleDataKey( 3 );
        
    %emitter.selectField( "EmissionAngle" );
        %emitter.setSingleDataKey( 0 );        

    %emitter.selectField( "EmissionArc" );
        %emitter.setSingleDataKey( 30 );        

    //%emitter.selectField( "SizeXVariation" );
        //%emitter.setSingleDataKey( 4 );

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
    
    //%emitter2 = %emitter.clone();
    //%emitter2.EmitterName = "emitter2";
    //%emitter2.Frame = 14;
    //%emitter2.EmitterType = "line";
    //%emitter2.EmitterAngle = 80;
    //%emitter2.EmitterOffset = "-35 15";
    //%effectAsset.addEmitter( %emitter2 );
//
    //%emitter3 = %emitter.clone();
    //%emitter3.EmitterName = "emitter3";
    //%emitter3.Frame = 15;
    //%emitter3.EmitterType = "box";
    //%emitter3.EmitterAngle = 30;
    //%emitter3.EmitterOffset = "-10 20";
    //%effectAsset.addEmitter( %emitter3 );
//
    //%emitter4 = %emitter.clone();
    //%emitter4.name = "emitter4";
    //%emitter4.Frame = 16;
    //%emitter4.EmitterType = "disk";
    //%emitter4.EmitterAngle = 30;
    //%emitter4.EmitterOffset = "20 -20";
    //%effectAsset.addEmitter( %emitter4 );
    //
    //%emitter5 = %emitter.clone();
    //%emitter5.EmitterName = "emitter5";
    //%emitter5.Frame = 17;
    //%emitter5.EmitterType = "ellipse";
    //%emitter5.EmitterAngle = -45;
    //%emitter5.EmitterOffset = "30 15";
    //%effectAsset.addEmitter( %emitter5 );    
    //
    //%emitter6 = %emitter.clone();
    //%emitter6.EmitterName = "emitter6";
    //%emitter6.Frame = 17;
    //%emitter6.EmitterType = "torus";
    //%emitter6.EmitterSize = "30 15";
    //%emitter6.EmitterAngle = -45;
    //%emitter6.EmitterOffset = "-20 -20";
    //%effectAsset.addEmitter( %emitter6 );  
    
    %assetFilePath = expandPath( "^MelvToy/particle.asset.taml" );
    
	TamlWrite( %effectAsset, %assetFilePath );
	
    if ( !AssetDatabase.addSingleDeclaredAsset( ModuleDatabase.findLoadedModule( "MelvToy" ), %assetFilePath ) )
    {
        error( "Could not load the asset file:" SPC %assetFilePath );
    }

    //TamlRead( %assetFilePath );   
    
    %particlePlayer = new ParticlePlayer();
    %particlePlayer.Position = "-40 0";
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
