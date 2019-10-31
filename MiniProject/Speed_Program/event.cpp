#include "event.hpp"
//Extended event class function decleration

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
//Full printing of each event with all the hit coordindates and timings, using the overloaded << operator on the hits

void event::quick_print()
{
    std::cout << "Number of fitted hits: " << get_hit_number() << std::endl;
    std::cout << "Fit tolerance met: " << good_fit << std::endl;
    std::cout << "Event Intercept: " << fit_intercept << std::endl;
    std::cout << "Event gradient: " << fit_gradient << " +/- " << fit_error << std::endl;
    std::cout << "Event drift velocity: " << drift_velocity*1e4 << " +/- " << drift_error*1e4 << " microns/ns" << std::endl;
}
//Printing of fit parameters used for single event analysis 

void event::update_fit(double& I, double& G, double& E)
{
    fit_intercept = I;
    fit_gradient = G;
    fit_error = E;
}


