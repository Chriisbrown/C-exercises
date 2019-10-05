#include "Box.hpp"

Box::Box(double x,double y, double z)
{
    X_len = x;
    Y_len = y;
    Z_len = z;
}

Box::Box(point& p)
{
    X_len = p.x;
    Y_len = p.y;
    Z_len = p.z;
}


bool Box::contains(double x, double y, double z) 
{
    if (-X_len/2 <= x && x <= X_len/2 && -Y_len/2 <= y  && y <= Y_len/2 && -Z_len/2 <= z && z <= Z_len/2) return true;

    else return false;  
}

bool Box::contains(const point& p) 
{
    if (-X_len/2 <= p.x && p.x <= X_len/2 && -Y_len/2 <= p.y && p.y <= Y_len/2 && -Z_len/2 <= p.z && p.z <= Z_len/2) return true;
    
    else return false;
}
