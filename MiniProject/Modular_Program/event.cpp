#include "event.hpp"

event& event::read_hit(hit& h1)
{
    event_hits.push_back(h1);
    return *this;
}


void event::print()
{
    std::cout << "Event_id: " << event_id << '\n';
    for (auto h : event_hits){
        std::cout << h;
    }
}

void event::quick_print()
{
    std::cout << "Event id: " << event_id << '\n';
    std::cout << "Hit number: " << get_hit_number() << '\n';
    std::cout << "Event fit: " << fit_gradient << '\n';
    std::cout << "Event drift velocity: " << drift_velocity << '\n';
}


void event::update_fit(double& I, double& G, double& E)
{
    fit_intercept = I;
    fit_gradient = G;
    fit_error = E;
    
}


