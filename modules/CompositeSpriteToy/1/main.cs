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

function CompositeSpriteToy::create( %this )
{
    // Load scripts.
    exec( "./scripts/noLayout.cs" );
    exec( "./scripts/rectLayout.cs" );
    exec( "./scripts/isoLayout.cs" );
    exec( "./scripts/customLayout.cs" );
        
    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", true );
    setSandboxDragModeAvailable( "pull", true );
    
    // Set the drag mode.
    setSandboxDragMode( "pull" );   

    // Configure the toy.
    CompositeSpriteToy.LayoutMode = "None";
    CompositeSpriteToy.AngularVelocity = 15;

    // Add the configuration options.
    addSelectionOption( "None,Rectilinear,Isometric,Custom", "Layout Mode", "setLayoutMode", true );
    addIntegerOption("Angular Velocity", -180, 180, "setAngularVelocity", CompositeSpriteToy.AngularVelocity, false );
        
    // Reset the toy.
    %this.reset();     
}

//-----------------------------------------------------------------------------

function CompositeSpriteToy::destroy( %this )
{
}

//-----------------------------------------------------------------------------

function CompositeSpriteToy::reset( %this )
{
    // Clear the scene.
    SandboxScene.clear();
    
    // Create the appropriate layout.
    switch$( CompositeSpriteToy.LayoutMode )
    {
        case "None":
            %this.createNoLayout();
            
        case "Rectilinear":
            %this.createRectLayout();
            
        case "Isometric":
            %this.createIsoLayout();
            
        case "Custom":
            %this.createCustomLayout();
    }
}

//-----------------------------------------------------------------------------

function CompositeSpriteToy::setLayoutMode( %this, %value )
{
    CompositeSpriteToy.LayoutMode = %value;
}

//-----------------------------------------------------------------------------

function CompositeSpriteToy::setAngularVelocity( %this, %value )
{
    CompositeSpriteToy.AngularVelocity = %value;
    
    // Update any active composite sprite.
	if ( isObject(CompositeSpriteToy.CompositeSprite) )
	    CompositeSpriteToy.CompositeSprite.setAngularVelocity( %value );
    
}