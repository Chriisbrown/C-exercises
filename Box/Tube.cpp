#include "Tube.hpp"
#include <cmath>

Tube::Tube(double r_min,double r_max, double z)
{
    R_min = r_min;
    R_max = r_max;
    Z_len = z;
}


bool Tube::contains(double x, double y, double z) 
{
    double r = std::sqrt(x*x+ y*y);
    if (R_min <= r && r <= R_max && -Z_len/2 <= z && z <= Z_len/2) return true;

    else return false;  
}

bool Tube::contains(const point& p) 
{
    double r = std::sqrt(p.x*p.x+ p.y*p.y);
    if (R_min <= r && r <= R_max && -Z_len/2 <= p.z && p.z <= Z_len/2) return true;

    else return false; 
}
