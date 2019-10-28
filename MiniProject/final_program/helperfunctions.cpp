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
    double final_fit_tolerance = 0.000003; // Final tolerance, the veloicity must change by less than this value in fit iterations in order to be a good fit
    int number_hits_tolerance = 4; //If an event has fewer than this value of hits, lost due to erranous hit points, it is deemed a bad fit and ignored
    int iteration_cap = 10; //Number of fit iterations allowed until the fitting terminates
// This is a set of data entries that are used in the fitting in various ways, they are provided here to allow quick updates to
// these parameters without having to change large amounts of code. Thet also can be accessed and updated from the main function
// allowing for a possible extension of the code to include user updating of these parameters.
};

template <typename T>
T square(T t)
{
    return t*t;
}

//Simple squaring function to reduce code complexity, used for either double or int so is left as a template function

double bitextractor(short buf, int length, int position) 
{ 
    return static_cast<double>(((1 << length) - 1) & (buf >> (position))); 
}
//This function takes a short (16 bit) buffer of bits and extracts a given number of them from a given position used for extracting the X,Y and T coordinates
// First the buffer is shifted right by the given position. It is then compared via a bitwise & to a set of 1s of the given length. e.g extract 3 bits from positon 3
//want to extract                                                     ↓↓↓
//buffer                                                    buf = 0110001001
//                                                                       ↓↓↓
//shifted by 3                                              buf = 0000110001
//bitwsise compare & to set of 3 1s                                      111 
//1 comapered to 1 is true and 0 visa versa                              001
// Return these bits casted as a double

std::vector<event> reading(std::string s,int start, int buffer_size)
{
    std::ifstream is (s,std::ios::binary);
    //Open file of file name s passed to function, in binary mode
    std::vector<event> event_buffer(buffer_size);
    // create a vector of blank events the size of the buffer
    if (is){
        //Prevents code from running if the file is not present
        is.seekg (start*16*buffer_size);
        // Set a starting point in the file, given as a number of bytes
        char* buffer = new char[16*buffer_size];
        // Create a buffer array in memory, size is given by buffer size
        is.read(buffer,16*buffer_size);
        // Read file into the buffer

        for (int j=0;j<buffer_size;++j)
        {
            //Iterate through sets of 16 bytes in the buffer
            for (int i=0;i<8;++i)
            {
                //Iterate through sets of 16 bits in the 16 byte subbuffer - the 16 bits in the buffer need to be reversed to be read correctly
                short val;
                val = buffer[j*16+i*2+1];
                // read the first 8 bits into a short
                val = ((val << 8) | buffer[j*16+i*2] & 0xff);
                // shift the 8 bits to the left and place the second 8 bits into the rest of the short
                // the & 0xff prevents the bits being defaulted to 1s in certain cases.

                double x_pos = (bitextractor(val,3,0));
                //extract the first 3 bits from the short. convert to double
                
                double Y_adjusted = bitextractor(val,3,3);
                // extract the second 3 bits from the short

                if (i%2 !=0) {Y_adjusted += 0.5;}
                // shift every other y value by a half - a feature of the detector geometry

                hit h1(start*buffer_size*8+j*8+i,x_pos,Y_adjusted,bitextractor(val,10,6));
                // create a hit, event id given by where in the total file this hit has been read from
                // the final 10 bits are converted to the time
                event_buffer[j].read_hit(h1);
                // read this hit into the event buffer vector, this happens 8 times per event as there are 8 hits per event
               

            }
            event_buffer[j].update_event_id(start*buffer_size+j);
            // update the event id for this particulat event
        }
        delete buffer;
        //Remove buffer from memory to prevent the whole file from being stored in memory
    }
    else{
        std::cout << "Reading Error" << std::endl;
    }
    return event_buffer;
    //return the vector of events
}

double short_res(double I,double G,double X, double Y){ return std::fabs((-G*X + Y - I)/std::sqrt(G*G + 1));}
// Quick function that finds the shortest distance between a point and a line analytically
double inverse_weight(double r){ return (1/(r*r + 1));}
// One of the weighting functions passed to the least squares calculator. It is inversely weighted by r squared
// a +1 is used to prevent any r=0 from overflowing

double weight(double r){ return (r*r); }
// A second weighting function

double no_weight(double r){ return 1.0; }
// No weight, it is defined like this to allow a non weighted linear fit to be used while preserving the weighting function format

void weighted_least_squares(event& e, double (*f)(double), bool hit,fitting_parameters& fp)
{
    //Initial fitting function, is is passed a single event, a weighting function and boolean switch used to update which tolerance and hit values to use.
    // THe first fits are based on the X and Y coordiantes of the sensor wires. Once these fits have provided a sutiable drift velocity an estimated positon
    // where the track produced the drift electrons passed through. These points are far closer to the fit line and so a more stringent tolerance is used to 
    // remove any hits that are too far from the fit line
    int shift = 0;
    double tolerance = fp.initial_tolerance;

    if (hit)
    { 
        shift = 3;
        // Shift change the fit from being performed on the X and Y wire coorindates to the x,y positons of the estimated hit position
        tolerance = fp.hit_tolerance;
    }
    
    int n = e.get_hit_number();
    // n could have changed if hits are removed
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;
    double error = 0.0;
    double r = 0.0;
    // declare temporary variables, used as many are summed over and need an initial value.

    for (int i=0; i<n; ++i)
    {
        r = e[i][3];
        X_mean += f(r)*e[i][1+shift];
        Y_mean += f(r)*e[i][2+shift];
        W_mean += f(r);
        // f(r) passed r to the given weighting function, when the no weight function is passed this algorithm does a least squares fit
    }
    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;
    //Calculation of means 

    for (int i=0; i<n; ++i)
    {
        r = e[i][3];
        numerator += f(r)*(e[i][1+shift]-X_mean)*(e[i][2+shift]-Y_mean);
        denominator += f(r)*square(e[i][1+shift]-X_mean);
    }

    double gradient = numerator/denominator;
    double intercept = Y_mean - gradient*X_mean;
    //Calculation of fit parameters using a weighted least squares algorithm

    e.update_fit(intercept,gradient,error);
    //Write new values of the fit to event, a 0 error is passed as the update fit takes it as a parameter but it is not calculated until later
    for (int i=0; i<n; ++i)
    {
        double res = short_res(intercept,gradient,e[i][1+shift],e[i][2+shift]);
        // Calculate residuals, the distance between each data point used in fitting to the fitter line
        if (res > tolerance)
        {
            e.remove_hit(i);
            //Remove any coordinates that have a large residual
        }
    }
}

void error_least_squares(event& e, fitting_parameters& fp)
{
    // This is a second fitting function but instead of taking a weighting function it uses a weight based on the error on the parameters.
    int n = e.get_hit_number();
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;
    double d_error = e.get_velocity()/e.get_velocity_error();
    /*
    Error based on the error on the velocity which propagates as an error in the hit positions that are used to calculate the fitting line
    Strictly these errors on the hit positions are x and y dependant but this weighted fit assumes they are just y dependant
    While not the most accurate possible a fit with x and y errors on the fitted points requires a iterative fit whereas jsut y dependance can 
    be performed analytically. In the interest of efficiency this choice is made, and the errors in the fit are very low making this a reasonable
    choice to make.

    Just the velocity errors are included here and not the propagation of errors in the timing that woudl further increase the error on the hit positions
    This choice is made as the velocity error is calculated from the error in the time and to prevent the same errors being propagated twice just the velocity
    error is considered.
    */ 

    for (int i=0; i<n; ++i)
    {
        X_mean += d_error*e[i][4];
        Y_mean += d_error*e[i][5];
        W_mean += d_error;
    }

    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;

    for (int i=0; i<n; ++i)
    {
        numerator += d_error*(e[i][4]-X_mean)*(e[i][5]-Y_mean);
        denominator += d_error*(e[i][4]-X_mean)*(e[i][4]-X_mean);
    }

    double gradient = numerator/denominator;
    double intercept = Y_mean - gradient*X_mean;

    double X_var = 0.0;
    double Y_var = 0.0;

    for (int i=0; i<n; ++i){
        X_var += square(d_error*(e[i][4] - X_mean));
    }

    double standard_Error = std::sqrt(1/X_var);
    //Analytical calculation of the error in the fit, used to analyse the fit later
    

    e.update_fit(intercept,gradient,standard_Error);

    for (int i=0; i<n; ++i){
        double res = short_res(intercept,gradient,e[i][4],e[i][5]);
        if (res > 0.3){
            e.remove_hit(i);
        }
    }
}

void drift_time_calculation(event& e, fitting_parameters& fp)
{
    /*
    Function for finding the drift speed, it calculates the distance between the X and Y wire coordinates
    And the time recorded. This distance/time gives the speed which is averaged and an error calulcated
    based on the error in the timing given in the fitting parameters.
    */
    int n = e.get_hit_number();
    int mean_t = 0.0;
    double speed = 0.0;

    for (int i=0; i<n; ++i)
    {
        speed += short_res(e.get_fit_intercept(),e.get_fit_gradient(),e[i][1],e[i][2]);
        mean_t += e[i][3];
    }
    e.update_velocity(speed/mean_t,(fp.timing_error/mean_t)*(speed/mean_t));
}

void hit_finder(event& e)
{
    /*
    Function for estimating the hit points where the electrons drifting to the wires originate. These points are better to fit to as they
    actually represent the physical points which the track went through
    This function finds the line passing through the wire coordinate, perpendicular to the fit line, and finds the point on this line a distance
    drift_time*drift velocity away from the wire coordinate in the direction towards the fit line.
    */
    int n = e.get_hit_number();
    for (int i=0; i<n;++i)
    {
        
        if (e[i][3] == 0)
        {
            e[i].set_hit_pos(e[i][1],e[i][2]);
        }

        else
        {
            double x = e[i][1];
            double y = e[i][2];
            double r = e[i][3] * e.get_velocity();
            double d = short_res(e.get_fit_intercept(),e.get_fit_gradient(),x,y);

            double m = -1/e.get_fit_gradient();
            double c = -m*x+ y;
            double x_intercept = (c - e.get_fit_intercept())/(e.get_fit_gradient() - m);
            
            double y_intercept = m*x_intercept + c;
            double x_new = ((x_intercept-x) * (r/d))+x;
            double y_new = ((y_intercept-y) * (r/d))+y;
            e[i].set_hit_pos(x_new,y_new);

        }
    }
}

event track_finder(event& E, fitting_parameters& fp)
{
    /*
    Final track finding algorithm that takes the above functions and uses them to find the best fit and best drift velocity.
    THere are a number of stages to the algorithm
    1. Find a unweighted fit of the wire coordinates hit in the event. This is used to remove any hits too far from the line
    2. Find a weighted fit on the line based on 1/(drift_time)^2 this emphasises any points with small drift times as these
    have the highest uncertainty as to which side of the wire coordinate the wire sits on. Weighting these ones heavier makes a better
    guess as to which side the track should go
    3. Find an intial drift velocity based on this new line
    4. Find an initial set of hit coordinates with this new drift velocity
    4. Perform a weighted fit based on the drift_time^2 using the calculated hit positions. This provides the best estimate for the hit
    position
    5. The next steps are iterated until a good fit is found, a good fit is defined as one that sees its drift velocity not change by 0.000003 over iterations
    A fit can be deemed as bad if it has fewer than 4 hit points left, or it iterates more than 10 times, these events are discarded 
    */
    weighted_least_squares(E,no_weight,false,fp);

    weighted_least_squares(E,inverse_weight,false,fp);
    drift_time_calculation(E,fp);

    hit_finder(E);
    weighted_least_squares(E,weight,true,fp);
    drift_time_calculation(E,fp);

    double drift_v_new = 0.0;
    double drift_v_old = 0.0;
    double difference = 1.0;
    // values used to check fit progress
    int iteration_number = 0;

    while (!E.check_fit_status()){
        //Intiial fit status is false so this is a while(True) loop, waiting for the fit to be good or the loop broken due to other constraints
        drift_v_new = E.get_velocity();

        hit_finder(E);
        error_least_squares(E,fp);
        drift_time_calculation(E,fp);

        drift_v_old = E.get_velocity();
        difference = drift_v_old - drift_v_new;

        if (E.get_hit_number() < fp.number_hits_tolerance | iteration_number >= fp.iteration_cap)
        {
            break;
            //Break when tolerance is met
        }

        else if (std::fabs(difference) < fp.final_fit_tolerance)
        {
            E.update_fit_status(true);
            break;
            // Check the difference is small, if so good fit and break
        }

        else
        {
            ++iteration_number;
            //Continue the iterations if tolerance has not been met and the fit is still bad
        }
    } 
    return E;
}
#endif