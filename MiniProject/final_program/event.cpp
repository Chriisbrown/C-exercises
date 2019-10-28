#include "event.hpp"

event& event::read_hit(hit& h1)
{
    event_hits.push_back(h1);
    return *this;
}
//Adds hits to the vector of hits in each event

void event::print()
{
    std::cout << "Event_id: " << event_id << std::endl;
    for (auto h : event_hits){
        std::cout << h;
    }
}
//Full printing of each event with all the hit coorindates and timings, using the overloaded << operator on the h1

void event::quick_print()
{
    std::cout << "Event id: " << event_id << std::endl;
    std::cout << "Hit number: " << get_hit_number() << std::endl;
    std::cout << "Event fit: " << fit_gradient << " +/- " << fit_error << std::endl;
    std::cout << "Event drift velocity: " << drift_velocity*10e4 << " +/- " << drift_error*10e4 << " microns/ns" << std::endl;
}
//Printing of fit parameters used for single event analysis 

void event::update_fit(double& I, double& G, double& E)
{
    fit_intercept = I;
    fit_gradient = G;
    fit_error = E;
}


