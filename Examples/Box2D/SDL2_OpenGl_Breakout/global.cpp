#include "global.h"
#include <cstdlib>
#include <cmath>


//------------------------------------------------------------------------------
// GEOM
void geom::RotatePoint( float rotationX, float rotationY, float angleRads, float& x, float& y )
{
    float s = std::sin( angleRads );
    float c = std::cos( angleRads );

    // translate point back to origin:
    x -= rotationX;
    y -= rotationY;

    // rotate point
    float xnew = x * c - y * s;
    float ynew = x * s + y * c;

    // translate point back:
    x = xnew + rotationX;
    y = ynew + rotationY;
}


//------------------------------------------------------------------------------
// MATH
float math::GetRandomFloat( float min, float max )
{
    return (min + (float)rand()/((float)RAND_MAX/(max - min)));
}

int math::GetRandomInt( int min, int max )
{
    return (min + (rand() % (max - min)));
}
