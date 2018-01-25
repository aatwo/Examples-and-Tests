#ifndef COMMON_H
#define COMMON_H

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

// Efficient geometry functions for use in prerender computations
namespace geom
{
    void GetCoordForAngleAroundPoint( b2Vec2& pCenter, float& radius, float& angle, b2Vec2& p_out );
    void GetHalfWayPointBetweenPoints( b2Vec2& p1, b2Vec2& p2, b2Vec2& pMid_out );
}

// Useful math functions
namespace math
{
    float GetRandomFloat( float min, float max );
    int GetRandomInt( int min, int max );
}

#endif // COMMON_H
