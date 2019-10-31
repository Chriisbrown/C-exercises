#include <iostream>

#ifndef HIT_H
#define HIT_H

/*
hit class, holds a single wire hit with wire coordinate and drift time
Also contains a hit_position, used for more accurate fitting
*/

class hit
{
    private:
        double id;
        double x_coordinate;
        double y_coordinate;
        double drift_time;
        double hit_position[2];
        //Data members, kept private to prevent accidental updating of these values without the use of the proper functions
    public:
        hit() : id{0}, x_coordinate{0} , y_coordinate{0} , drift_time{0} {}; //default constructor, set all to 0.

        hit(double number, double x, double y, double t) : id{number}, x_coordinate{x} , y_coordinate{y} , drift_time{t} {}; //constructor accepts values for the 4 variables

        hit& operator=(const hit &); //copy assignment, used if a hit is set equal to a different hit

        double &operator[] (int); //Overload of indexing for the hit allows for iterations invloving some hit[i]

        hit(const hit& h) : id{h.id}, x_coordinate{h.x_coordinate} , y_coordinate{h.y_coordinate} , drift_time{h.drift_time} {}; //copy constructor
        //Used if a new hit is initialized as a different hit
        void set_values(int&, int& , int& , int&); //Allows the private data members to be updated

        void set_hit_pos(double& , double& ); 
        
        friend std::ostream& operator<<(std::ostream& os, const hit& h); //Overload the << operator which allows for easier printing of hit members
        
        ~hit(){}; //Default destructor
};


#endif
 
