#include <iostream>
#include "volume.hpp"


class Tube : public volume
{
private:
    double R_min = 0.0;
    double R_max = 1.0;
    double Z_len = 1.0;

public:
    Tube(double, double, double);
    Tube() {};

    bool contains(double, double, double) override;
    bool contains(const point& ) override;

    ~Tube() {};
};






