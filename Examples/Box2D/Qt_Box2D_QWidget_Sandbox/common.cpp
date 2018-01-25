#include "common.h"

#include <Box2D/Common/b2Math.h>
#include <cstdlib>

//------------------------------------------------------------------------------
// GEOM
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void geom::GetCoordForAngleAroundPoint( b2Vec2& pCenter, float& radius, float& angle, b2Vec2& p_out )
{
    if( angle >= 0.0f && angle < PIE_OVER_TWO )
    {
        p_out.x = static_cast<float>( sin(angle) );
        p_out.y = static_cast<float>( -cos(angle) );
    }
    else if( angle >= PIE_OVER_TWO && angle < PIE )
    {
        p_out.x = static_cast<float>( cos(angle - PIE_OVER_TWO) );
        p_out.y = static_cast<float>( sin(angle - PIE_OVER_TWO) );
    }
    else if( angle >= PIE && angle < THREE_PIE_OVER_TWO )
    {
        p_out.x = static_cast<float>( -sin(angle - (PIE)) );
        p_out.y = static_cast<float>( cos(angle - (PIE)) );
    }
    else
    {
        p_out.x = static_cast<float>( -cos(angle - THREE_PIE_OVER_TWO) );
        p_out.y = static_cast<float>( -sin(angle - THREE_PIE_OVER_TWO) );
    }

    p_out.x = (p_out.x * radius) + pCenter.x;
    p_out.y = (p_out.y * radius) + pCenter.y;
}

//------------------------------------------------------------------------------
void geom::GetHalfWayPointBetweenPoints( b2Vec2& p1, b2Vec2& p2, b2Vec2& pMid_out )
{
    pMid_out.x = p1.x + (p2.x - p1.x) / 2.0f;
    pMid_out.y = p1.y + (p2.y - p1.y) / 2.0f;
}


//------------------------------------------------------------------------------
// MATH
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
float math::GetRandomFloat( float min, float max )
{
    return (min + (float)rand()/((float)RAND_MAX/(max - min)));
}

//------------------------------------------------------------------------------
int math::GetRandomInt( int min, int max )
{
    return (min + (rand() % (max - min)));
}
