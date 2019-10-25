#include "event.hpp"

event& event::read_hit(hit& h1)
{
    event_hits.push_back(h1);
    return *this;
}


void event::print()
{
    std::cout << "Event_id: " << event_id << std::endl;
    for (auto h : event_hits){
        std::cout << h;
    }
}

void event::quick_print()
{
    std::cout << "Event id: " << event_id << std::endl;
    std::cout << "Hit number: " << get_hit_number() << std::endl;
    std::cout << "Event fit: " << fit_gradient << " +/- " << fit_error << std::endl;
    std::cout << "Event drift velocity: " << drift_velocity << " +/- " << drift_error << " cm/ns" << std::endl;
}


void event::update_fit(double& I, double& G, double& E)
{
    fit_intercept = I;
    fit_gradient = G;
    fit_error = E;
    
}


