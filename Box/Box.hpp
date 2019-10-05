#include <iostream>
#include "volume.hpp"


class Box : public volume
{
private:
    double X_len = 0.0;
    double Y_len = 0.0;
    double Z_len = 0.0;

public:
    Box(double, double, double);
    Box(point&);
    Box() {};
    bool contains(double, double, double) override;
    bool contains(const point&) override;

    ~Box() {};
};






