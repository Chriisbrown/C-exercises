#include <fstream>
#include <cmath>
#include "hit.hpp"
#include "event.hpp"

#ifndef FUNC_H
#define FUNC_H

/*
This file contains a set of functions used by the main script for reading files and calcating fits, it is given here instead of in the
Main.cpp in order to simplify the program. This file is included in the main.cpp and so is not compiled as a seperate argument
*/

struct fitting_parameters
{
    double timing_error = 0.5; //Error in the drift times
    double initial_tolerance = 1; //Tolerance for erranous hit points, if a hit is greater than this value away from the initial fit line it is removed
    double hit_tolerance = 0.3; //Second tolerance for erranous hit points, used for more accurate hit selection
    double final_fit_tolerance = 0.000003; // Final tolerance, the velocity must change by less than this value in fit iterations in order to be a good fit
    int number_hits_tolerance = 4; //If an event has fewer than this value of hits, lost due to erranous hit points, it is deemed a bad fit and ignored
    int iteration_cap = 10; //Number of fit iterations allowed until the fitting terminates
// This is a set of data entries that are used in the fitting in various ways, they are provided here to allow quick updates to
// these parameters without having to change large amounts of code. Thet also can be accessed and updated from the main function
// allowing for a possible extension of the code to include user updating of these parameters.
};

template <typename T>
T square(T t);


double bitextractor(short, int, int); 


std::vector<event> reading(std::string s,int start, int buffer_size);


double short_res(double I,double G,double X, double Y){ return std::fabs((-G*X + Y - I)/std::sqrt(G*G + 1));}

double inverse_weight(double r){ return (1/(r*r + 1));}

double weight(double r){ return (r*r); }
double no_weight(double r){ return 1.0; }

void weighted_least_squares(event& e, double (*f)(double), bool hit,fitting_parameters& fp);


void error_least_squares(event& e, fitting_parameters& fp);

void drift_time_calculation(event& e, fitting_parameters& fp);

void hit_finder(event& e);


event track_finder(event& E, fitting_parameters& fp);

#endif
