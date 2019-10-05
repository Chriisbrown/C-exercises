#include "point.hpp"

#ifndef VOL_H
#define VOL_H

class volume
{
  
public:
    bool virtual contains(double, double, double) =0;
    bool virtual contains(const point&) =0;

};

#endif


