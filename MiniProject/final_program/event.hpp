#include <vector>
#include "hit.hpp"

#ifndef EVENT_H
#define EVENT_H

/*
Event class includes all information assocaiated with an event including which hits it includes and its velocity and fit gradient
*/

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

    //Private date members, prevents accidental access and changing
    //Drift error and good_fit are provided with default values as they are used in the code before being updated

    public:
        event() : event_id{0} {};
        event(int n) : event_id{n} {};

        //default constructors and constructor of specific even

        int get_hit_number() {return event_hits.size();}
        // quick one line functions are included in the header file due to their small size and fast completion
        // This function tells the user how many hits are in this event, useful when hits are removed for being erraneous

        ~event() {}; //Default destructor

        hit &operator[] (int index){ return event_hits[index]; } //Overloading of the [] operator, allows for simple iteration through hits

        void update_event_id (int n){ event_id = n; }

        void update_fit(double&, double&, double&);
        double get_fit_gradient() {return fit_gradient;}
        double get_fit_intercept() {return fit_intercept;}
        double get_fit_error() {return fit_error;}

        //Simple functions for updating and accesing fit parameters

        void update_fit_status (bool fit) { good_fit = fit;}
        bool check_fit_status() { return good_fit;}

        //Simple functions for updating and accessing fit status
        
        

        void update_velocity(double V, double E) { drift_velocity = V; drift_error = E; }
        double get_velocity() { return drift_velocity; }
        double get_velocity_error() {return drift_error;}

        //Simple functions for updating and accessing velocity parameters


        event& read_hit(hit&); //Reads a hit into a event, used in reading files
        void remove_hit(int index){ event_hits.erase(event_hits.begin() + index); }
        // Remove hit functionality allows for hits to be erased from an event in case a hit is erraneous, the .begin() provides an
        // iterator to the first value in the event_hits vector


        void print();
        void quick_print();

};


#endif