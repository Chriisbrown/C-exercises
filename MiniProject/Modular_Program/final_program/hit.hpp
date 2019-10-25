#include <iostream>

#ifndef HIT_H
#define HIT_H

class hit
{
    public:
        hit() : id{0}, x_coordinate{0} , y_coordinate{0} , drift_time{0} {}; //default constructor

        hit(double number, double x, double y, double t) : id{number}, x_coordinate{x} , y_coordinate{y} , drift_time{t} {}; //constructor

        hit& operator=(const hit &); //copy assignment

        double &operator[] (int);

        
        hit(const hit& h) : id{h.id}, x_coordinate{h.x_coordinate} , y_coordinate{h.y_coordinate} , drift_time{h.drift_time} {}; //copy constructor

        void set_values(int&, int& , int& , int&);

        void set_hit_pos(double& , double& );
        
        friend std::ostream& operator<<(std::ostream& os, const hit& h);
        
        ~hit(){};

    private:
        double id;
        double x_coordinate;
        double y_coordinate;
        double drift_time;
        double hit_position[2];
};


#endif