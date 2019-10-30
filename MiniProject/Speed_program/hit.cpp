#include "hit.hpp"
//Extended hit function declerations

hit& hit::operator=(const hit& h)
{
    id = h.id;
    x_coordinate = h.x_coordinate;
    y_coordinate = h.y_coordinate;
    drift_time = h.drift_time;
    return *this;
}

//Full definition of hit copy assignment operator, it allows for hit1 = hit2 where hit2 is passed as a reference

double& hit::operator[] (int index)
{
    double* def = 0;
    switch (index)
    { //Allows for hit[i] functionality, decreasing code complexity, a switch is used to determine which variable is returned
        case 0: return id;
        case 1: return x_coordinate;
        case 2: return y_coordinate;
        case 3: return drift_time;
        case 4: return hit_position[0];
        case 5: return hit_position[1];

        default: return *def; //Though unused this default will return a reference to a 0 variable, included for completeness
    }
}

void hit::set_values(int& n, int&x, int& y, int& t)
{
    id = n;
    x_coordinate = x;
    y_coordinate = y;
    drift_time = t;
}
//simple set variables function

void hit::set_hit_pos(double& x, double& y)
{
    hit_position[0] = x;
    hit_position[1] = y;
}

std::ostream& operator<<(std::ostream& os, const hit& h)
{
    os << "Hit_id: " << h.id << '\t'  << " X: " << h.x_coordinate << " Y: " << h.y_coordinate << '\t' << " Drift Time: " << h.drift_time << std::endl;
    return os;
    //<< operator overload which simplifies printing function for the event class, instead of each variable needing to be printed
    // the overloaded function will print the entire hit data when the << call to print is used. Also allows the functionality for
    // this printing to be done to a file with the ostream instead of specifically cout
}
