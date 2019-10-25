#include <fstream>
#include <cmath>
#include "hit.hpp"
#include "event.hpp"

#ifndef FUNC_H
#define FUNC_H

struct fitting_parameters
{

    double timing_error = 0.005;
    double initial_tolerance = 1;
    double hit_tolerance = 0.3;
    double final_fit_tolerance = 0.00001;
    int number_hits_tolerance = 4;
    int iteration_cap = 10; 

};

template <typename T>
T square(T t){
    return t*t;
}

double bitextractor(short buf, int length, int position) 
{ 
    return static_cast<double>(((1 << length) - 1) & (buf >> (position))); 
}

std::vector<event> reading(std::string s,int start, int buffer_size)
{
    std::ifstream is (s,std::ios::binary);
    std::vector<event> event_buffer(buffer_size);

    if (is){
        is.seekg (start*16*buffer_size);
        char* buffer = new char[16*buffer_size];
        is.read(buffer,16*buffer_size);

        for (int j=0;j<buffer_size;++j)
        {
            

            for (int i=0;i<8;++i)
            {
                short val;
                val = buffer[j*16+i*2+1];
                val = ((val << 8) | buffer[j*16+i*2] & 0xff);

                double x_pos = (bitextractor(val,3,0));
                
                double Y_adjusted = bitextractor(val,3,3);

                if (i%2 !=0) {Y_adjusted += 0.5;}

                hit h1(start*buffer_size*8+j*8+i,x_pos,Y_adjusted,bitextractor(val,10,6));
                event_buffer[j].read_hit(h1);
                event_buffer[j].update_event_id(start*buffer_size+j);

            }
        }
        delete buffer;
    }
    else{
        std::cout << "Reading Error" << std::endl;
    }
    return event_buffer;
}

double short_res(double I,double G,double X, double Y){ return std::fabs((-G*X + Y - I)/std::sqrt(G*G + 1));}

double inverse_weight(double r){ return (1000.0/(r*r + 25));}

double weight(double r){ return (r*r/1000.0); }

double no_weight(double r){ return r; }

void weighted_least_squares(event& e, double (*f)(double), bool hit,fitting_parameters& fp)
{
    int shift = 0;
    double tolerance = fp.initial_tolerance;
    if (hit){ 
        shift = 3;
        tolerance = fp.hit_tolerance;
        }
    
    int n = e.get_hit_number();
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;
    double error = 0.0;

    for (int i=0; i<n; ++i){

        X_mean += f(e[i][3])*e[i][1+shift];
        Y_mean += f(e[i][3])*e[i][2+shift];
        W_mean += f(e[i][3]);
    }
    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;

    for (int i=0; i<n; ++i){
        numerator += f(e[i][3])*(e[i][1+shift]-X_mean)*(e[i][2+shift]-Y_mean);
        denominator += f(e[i][3])*square(e[i][1+shift]-X_mean);
    }

    double gradient = numerator/denominator;
    double intercept = Y_mean - gradient*X_mean;

    e.update_fit(intercept,gradient,error);

    for (int i=0; i<n; ++i){
        
        double res = short_res(intercept,gradient,e[i][1+shift],e[i][2+shift]);
        if (res > tolerance){
            e.remove_hit(i);
        }
    }
    
}

void error_least_squares(event& e, fitting_parameters& fp)
{
    int n = e.get_hit_number();
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;
    double d_error = 0.0;



    for (int i=0; i<n; ++i){
        d_error = 1/std::sqrt(square(e.get_velocity_error()/e.get_velocity()) + square(e[i][3]/fp.timing_error));
        X_mean += d_error*e[i][4];
        Y_mean += d_error*e[i][5];
        W_mean += d_error;
    }

    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;

    for (int i=0; i<n; ++i){
        d_error = 1/std::sqrt(square(e.get_velocity_error()/e.get_velocity()) + square(e[i][3]/fp.timing_error));
        numerator += d_error*(e[i][4]-X_mean)*(e[i][5]-Y_mean);
        denominator += d_error*(e[i][4]-X_mean)*(e[i][4]-X_mean);
    }

    double gradient = numerator/denominator;
    double intercept = Y_mean - gradient*X_mean;

    double X_var = 0.0;
    double Y_var = 0.0;

    for (int i=0; i<n; ++i){
        d_error = square(e.get_velocity_error()/e.get_velocity()) + square(e[i][3]/fp.timing_error);
        X_var += d_error*(e[i][4] - X_mean)*(e[i][4] - X_mean);
    }

    double standard_Error = std::sqrt(1/X_var);
    

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
    int n = e.get_hit_number();
    int mean_n = 0.0;
    double speed = 0.0;

    for (int i=0; i<n; ++i)
    {
        speed += short_res(e.get_fit_intercept(),e.get_fit_gradient(),e[i][1],e[i][2]);
        mean_n += e[i][3];
    }
    e.update_velocity(speed/mean_n,(fp.timing_error/mean_n)*(speed/mean_n));
}

void hit_finder(event& e)
{
    int n = e.get_hit_number();
    for (int i=0; i<n;++i)
    {
        double x = e[i][1];
        double y = e[i][2];
        double r = e[i][3] * e.get_velocity();

        if (e[i][3] == 0)
        {
            e[i].set_hit_pos(e[i][1],e[i][2]);
        }

        else
        {
            double m = -1/e.get_fit_gradient();
            double c = -m*x+ y;

            double A = (1+m*m);
            double B = 2*(m*c-m*y - x);
            double C = (y*y - r*r + x*x - 2*c*y + c*c);

            double x_plus = (-B+std::sqrt(B*B - 4*A*C))/(2*A);
            double x_minus = (-B-std::sqrt(B*B - 4*A*C))/(2*A);

            double y_plus = m*x_plus + c;
            double y_minus = m*x_minus + c;

            if (short_res(e.get_fit_intercept(),e.get_fit_gradient(),x_plus,y_plus) < short_res(e.get_fit_intercept(),e.get_fit_gradient(),x_minus,y_minus)){
                e[i].set_hit_pos(x_plus,y_plus);
            }
            else{
                e[i].set_hit_pos(x_minus,y_minus);
            }
        }
    }
}

event calculation(event& E, fitting_parameters& fp)
{

    weighted_least_squares(E,no_weight,false,fp);

    weighted_least_squares(E,inverse_weight,false,fp);
    drift_time_calculation(E,fp);

    hit_finder(E);
    weighted_least_squares(E,weight,true,fp);
    drift_time_calculation(E,fp);

    double drift_v_new = 0.0;
    double drift_v_old = 0.0;
    double ratio = 1.0;
    int iteration_number = 0;

    while (!E.check_fit_status()){
        drift_v_new = E.get_fit_gradient();

        hit_finder(E);
        error_least_squares(E,fp);
        drift_time_calculation(E,fp);

        drift_v_old = E.get_fit_gradient();
        ratio = drift_v_old - drift_v_new;

        if (E.get_hit_number() < fp.number_hits_tolerance | iteration_number >= fp.iteration_cap)
        {
            break;
        }

        else if (std::fabs(ratio) < fp.final_fit_tolerance)
        {
            E.update_fit_status(true);
            break;
        }

        else
        {
            ++iteration_number;
        }
    } 
    return E;
}

#endif