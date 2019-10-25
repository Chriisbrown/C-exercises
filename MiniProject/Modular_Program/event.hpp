#include <vector>
#include "hit.hpp"

#ifndef EVENT_H
#define EVENT_H

class event{
    private:
        int event_id;
        std::vector<hit> event_hits;

        double drift_velocity;
        double fit_gradient;
        double fit_intercept; 
        double fit_error;
        double drift_error = 0;

        bool good_fit = false;

    public:
        event() : event_id{0} {};
        event(int n) : event_id{n} {};

        int get_hit_number() {return event_hits.size();}

        ~event() {};

        hit &operator[] (int index){ return event_hits[index]; } 

        void update_event_id (int n){ event_id = n; }

        void update_fit(double&, double&, double&);
        double get_fit_gradient() {return fit_gradient;}
        double get_fit_intercept() {return fit_intercept;}
        double get_fit_error() {return fit_error;}

        void update_fit_status (bool fit) { good_fit = fit;}
        bool check_fit_status() { return good_fit;}
        
        

        void update_velocity(double V, double E) { drift_velocity = V; drift_error = E; }
        double get_velocity() { return drift_velocity; }
        double get_velocity_error() {return drift_error;}



        hit& return_hit(int index){ return event_hits[index]; }
        event& read_hit(hit&);
        void remove_hit(int index){ event_hits.erase(event_hits.begin() + index); }

        void print();
        void quick_print();
};




#endif