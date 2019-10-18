#include "hit.hpp"

hit& hit::operator=(const hit& h)
{
    id = h.id;
    x_coordinate = h.x_coordinate;
    y_coordinate = h.y_coordinate;
    drift_time = h.drift_time;
    return *this;
}


double& hit::operator[] (int index)
{
    double* def = 0;
    switch (index){
        case 0: return id;
        case 1: return x_coordinate;
        case 2: return y_coordinate;
        case 3: return drift_time;
        case 4: return hit_position[0];
        case 5: return hit_position[1];

        default: return *def;
        }
}

void hit::set_values(int& n, int&x, int& y, int& t)
{
    id = n;
    x_coordinate = x;
    y_coordinate = y;
    drift_time = t;
}

void hit::set_hit_pos(double& x, double& y)
{
    hit_position[0] = x;
    hit_position[1] = y;
}

std::ostream& operator<<(std::ostream& os, const hit& h)
{
    os << "Hit_id: " << h.id << " X:" << h.x_coordinate << " Y: " << h.y_coordinate << " Drift Time: " << h.drift_time << '\n';
    return os;
}