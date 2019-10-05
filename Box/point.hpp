#ifndef POINT_H
#define POINT_H


struct point
{
    double x = 0;
    double y = 0;
    double z = 0;
    
    point() {};
    point(double X, double Y, double Z) : x{X}, y{Y}, z{Z} {};
    ~point() {};
};

#endif