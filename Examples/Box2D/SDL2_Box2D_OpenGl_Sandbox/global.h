#ifndef GLOBAL_H
#define GLOBAL_H


#include <string>


// Useful precomputed constants
#define TWO_PIE                             6.28318530717f   // 360 degrees
#define THREE_PIE_OVER_TWO                  4.71238898038f   // 270 degrees
#define PIE                                 3.14159265359f   // 180 degrees
#define PIE_OVER_TWO                        1.57079632679f   // 90  degrees
#define PIE_OVER_TWO                        1.57079632679f   // 90  degrees
#define PIE_OVER_FOUR                       0.78539816339f   // 45  degrees

// Useful macros
#define RADIANS_TO_DEGREES(radians) ((radians) * (180.0f / PIE))
#define DEGREES_TO_RADIANS(angle) ((angle) / 180.0f * PIE)


struct b2Vec2;


namespace global
{


    const std::string companyName       = "Pixelated Monkey";
    const std::string applicationName   = "Chain Cast";


} // global


// Geometry functions for use in prerender computations
namespace geom
{
    void RotatePoint( float rotationX, float rotationY, float angleRads, float& x, float& y );
    void GetCoordForAngleAroundPoint( float x, float y, float& radius, float& angle, b2Vec2& p_out );
    void GetHalfWayPointBetweenPoints( b2Vec2& p1, b2Vec2& p2, b2Vec2& pMid_out );

} // geom


// Useful math functions
namespace math
{
    float GetRandomFloat( float min, float max );
    int GetRandomInt( int min, int max );

} // math


#endif // GLOBAL_H
