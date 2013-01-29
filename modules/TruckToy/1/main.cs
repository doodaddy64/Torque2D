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

function createTruckToy( %scopeSet )
{
    // Set the sandbox drag mode availability.
    setSandboxDragModeAvailable( "pan", false );
    
    // Set the drag mode as "pull".
    setSandboxDragMode( "pull" );
    
    // Load scripts.
    exec( "./scripts/truck.cs" );

    // Create truck keys.    
    new ActionMap(truckToyMap);
    %scopeSet.add( truckToyMap );
    truckToyMap.bind(keyboard, "ctrl tilde", toggleConsole);
    truckToyMap.bind(keyboard, "left", truckReverse);
    truckToyMap.bind(keyboard, "right", truckForward);
    truckToyMap.push();
    
    // Initialize truck world.
    initializeTruckWorld();
}

//-----------------------------------------------------------------------------

function destroyTruckToy( %scopeSet )
{
    // Delete the input map.
    truckToyMap.pop();
    truckToyMap.delete();
}
