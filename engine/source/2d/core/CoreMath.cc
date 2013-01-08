//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#include "platform/platform.h"
#include "platform/platformGL.h"
#include "console/consoleTypes.h"
#include "console/console.h"
#include "math/mRandom.h"
#include "2d/core/Vector2.h"
#include "2d/core/CoreMath.h"

//-----------------------------------------------------------------------------

namespace CoreMath
{

MRandomLCG gRandomGenerator;

//---------------------------------------------------------------------------------------------

/// Returns a point on the given line ab that is closest to 'point'.
/// @param a The start of the line.
/// @param b The end of the line.
/// @param point The point to test with
/// @return A Point2F of the nearest point that lies on the line
Vector2 mGetClosestPointOnLine( Vector2 &a, Vector2 &b, Vector2 &point)
{
   // Get the vector from a to the point
   Vector2 c = point - a;

   // The line's vector
   Vector2 v = b - a;
   v.Normalize();

   // Intersection point distance from a
   F32 t = v.dot(c);

   // The distance to move from point a
   v *= t;

   // The actual point starting at a
   return a + v;
}

//---------------------------------------------------------------------------------------------

bool mPointInRectangle( const Vector2& point, const Vector2& rectMin, const Vector2& rectMax )
{
    // Do the trivial check to see if point is within the rectangle.
    if (    point.x < getMin(rectMin.x,rectMax.x) || point.x > getMax(rectMin.x,rectMax.x) ||
            point.y < getMin(rectMin.y,rectMax.y) || point.y > getMax(rectMin.y,rectMax.y) )
                // We possibly haven't a collision!
                return false;
            else
                // We *must* have a collision!
                return true;
}

//---------------------------------------------------------------------------------------------

bool mLineRectangleIntersect( const Vector2& startPoint, const Vector2& endPoint, const Vector2& rectMin, const Vector2& rectMax, F32* pTime )
{
    // Reset Times.
    F32 startTime, endTime = 0;
    F32 finishStartTime = 0.0f;
    F32 finishEndTime = 1.0f;

    // Initialise Element Pointers.
    F32 const* pRectMin     = &(rectMin.x);
    F32 const* pRectMax     = &(rectMax.x);
    F32 const* pStartPoint  = &(startPoint.x);
    F32 const* pEndPoint    = &(endPoint.x);

    // Step through elements.
    for (int i = 0; i < 2; i++)
    {
        if (*pStartPoint < *pEndPoint)
        {
            // Are we outside the bounds?
            if (*pStartPoint > *pRectMax || *pEndPoint < *pRectMin)
                // Yes, so return 'No Collision'.
                return false;

            // Calculate Intercept Times.
            F32 deltaPoint = *pEndPoint - *pStartPoint;
            startTime = (*pStartPoint < *pRectMin) ? (*pRectMin - *pStartPoint) / deltaPoint : 0.0f;
            endTime = (*pEndPoint > *pRectMax) ? (*pRectMax - *pStartPoint) / deltaPoint : 1.0f;
        }
        else
        {
            // Are we outside the bounds?
            if (*pEndPoint > *pRectMax || *pStartPoint < *pRectMin)
                // Yes, so return 'No Collision'.
                return false;

            // Calculate Intercept Times.
            F32 deltaPoint = *pEndPoint - *pStartPoint;
            startTime = (*pStartPoint > *pRectMax) ? (*pRectMax - *pStartPoint) / deltaPoint : 0.0f;
            endTime = (*pEndPoint < *pRectMin) ? (*pRectMin - *pStartPoint) / deltaPoint : 1.0f;
        }

        // Bound our times.
        if (startTime > finishStartTime) finishStartTime = startTime;
        if (endTime < finishEndTime) finishEndTime = endTime;

            // Are we outside the bounds?
        if (finishEndTime < finishStartTime)
            // Yes, so return 'No Collision'.
            return false;

        // Next Element.
        pRectMin++; pRectMax++;
        pStartPoint++; pEndPoint++;
    }

    // Store Finish Start Time ( if requested )
    if ( pTime )
        *pTime = finishStartTime;

    // Return 'Collision'.
    return true;
}

} // Namespace CoreMath