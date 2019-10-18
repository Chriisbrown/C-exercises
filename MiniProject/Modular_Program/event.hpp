#include <vector>
#include "hit.hpp"

#ifndef EVENT_H
#define EVENT_H

class event{
    private:
        int event_id;
        std::vector<hit> event_hits;

    public:
        bool good_fit = false;

        double fit_gradient;
        double fit_intercept; 
        double drift_velocity;

        event() : event_id{0} {};
        event(int n) : event_id{n} {};

        int get_hit_number() {return event_hits.size();}

        ~event() {};

        hit &operator[] (int index){ return event_hits[index]; } 

        void update_event_id (int n){ event_id = n; }

        void update_fit(double&, double&);

        void update_velocity(double V) { drift_velocity = V; }

        hit& return_hit(int index){ return event_hits[index]; }

        event& read_hit(hit&);

        void remove_hit(int index){ event_hits.erase(event_hits.begin() + index); }

        void print();
        void quick_print();
};




#endif