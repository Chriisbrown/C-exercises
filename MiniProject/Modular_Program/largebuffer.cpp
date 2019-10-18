#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>
#include <chrono>
#include "hit.hpp"
#include "event.cpp"

const std::string s = "manytracks.raw";





double bitextractor(short buf, int length, int position) 
{ 
    return static_cast<double>(((1 << length) - 1) & (buf >> (position))); 
}


std::vector<event> reading(int start, int buffer_size)
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

    
                if (std::fmod(x_pos,2) != 0) {
                    Y_adjusted += 0.5;
                }
                //std::cout << (start+1)*buffer_size+j*8+i << '\n';
                hit h1(start*buffer_size*8+j*8+i,x_pos,Y_adjusted,bitextractor(val,10,6));
                event_buffer[j].read_hit(h1);
                event_buffer[j].update_event_id(start*buffer_size+j);

            }
        }
        delete buffer;
    }
    return event_buffer;
}

double short_res(double I,double G,double X, double Y){
    return std::fabs((-G*X + Y - I)/std::sqrt(G*G + 1));
}

void least_squares(event& e)
{
    int n = e.get_hit_number();
    double product = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double X_squared_mean = 0.0;

    for (int i=0; i<n; ++i){
        product += e[i][1]*e[i][2];
        X_mean += e[i][1];
        Y_mean += e[i][2];
        X_squared_mean += e[i][1]*e[i][1];
    }

    double gradient = ((product/n) - (X_mean/n)*(Y_mean/n))/((X_squared_mean/n)-((X_mean*X_mean)/(n*n)));
    double intercept = Y_mean/n - gradient*X_mean/n;

    for (int i=0; i<n; ++i){
        double res = short_res(intercept,gradient,e[i][1],e[i][2]);
        std::cout << res << '\n';
        if (res > 0.3){
            e.remove_hit(i);
        }
    }

    e.update_fit(intercept,gradient);    
}

double inverse_weight(double r){
    return (1000.0/(r*r + 25));
}

double weight(double r){
    return (r*r/1000.0);
}

double no_weight(double r){
    return r;
}

void weighted_least_squares(event& e, double (*f)(double), bool hit)
{
    int shift = 0;
    double tolerance = 1;
    if (hit){ 
        shift = 3;
        tolerance = 0.3;
        }
    
    int n = e.get_hit_number();
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;

    for (int i=0; i<n; ++i){

        X_mean += f(e[i][3])*e[i][1+shift];
        Y_mean += f(e[i][3])*e[i][2+shift];
        W_mean += f(e[i][3]);
    }
    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;

    for (int i=0; i<n; ++i){
        numerator += f(e[i][3])*(e[i][1+shift]-X_mean)*(e[i][2+shift]-Y_mean);
        denominator += f(e[i][3])*(e[i][1+shift]-X_mean)*(e[i][1+shift]-X_mean);
    }

    double gradient = numerator/denominator;
    double intercept = Y_mean - gradient*X_mean;

    e.update_fit(intercept,gradient);

    for (int i=0; i<n; ++i){
        
        double res = short_res(intercept,gradient,e[i][1+shift],e[i][2+shift]);
        if (res > tolerance){
            e.remove_hit(i);
        }
    }
    
}

void error_least_squares(event& e)
{
    int n = e.get_hit_number();
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;
    double error = 1/((0.5*e.drift_velocity)*(0.5*e.drift_velocity));

    for (int i=0; i<n; ++i){

        X_mean += error*e[i][4];
        Y_mean += error*e[i][5];
        W_mean += error;
    }

    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;

    for (int i=0; i<n; ++i){
        numerator += error*(e[i][4]-X_mean)*(e[i][5]-Y_mean);
        denominator += error*(e[i][4]-X_mean)*(e[i][4]-X_mean);
    }

    double gradient = numerator/denominator;
    double intercept = Y_mean - gradient*X_mean;

    e.update_fit(intercept,gradient);

    for (int i=0; i<n; ++i){
        double res = short_res(intercept,gradient,e[i][4],e[i][5]);
        if (res > 0.3){
            e.remove_hit(i);
        }
    }
}

void drift_time_calculation(event& e)
{
    int n = e.get_hit_number();
    int mean_n = n;
    double speed = 0.0;

    for (int i=0; i<n; ++i)
    {
        if (e[i][3] == 0) {mean_n-=1;}
        else{
            speed += short_res(e.fit_intercept,e.fit_gradient,e[i][1],e[i][2])/(e[i][3]);
        }
    }
    e.drift_velocity = speed/mean_n;
}

void hit_finder(event& e)
{
    int n = e.get_hit_number();
    for (int i=0; i<n;++i)
    {
        double x = e[i][1];
        double y = e[i][2];
        double r = e[i][3] * e.drift_velocity;

        if (e[i][3] == 0)
        {
            e[i].set_hit_pos(e[i][1],e[i][2]);
        }

        else
        {
            double m = -1/e.fit_gradient;
            double c = -m*x+ y;

            double A = (1+m*m);
            double B = 2*(m*c-m*y - x);
            double C = (y*y - r*r + x*x - 2*c*y + c*c);

            double x_plus = (-B+std::sqrt(B*B - 4*A*C))/(2*A);
            double x_minus = (-B-std::sqrt(B*B - 4*A*C))/(2*A);

            double y_plus = m*x_plus + c;
            double y_minus = m*x_minus + c;

            if (short_res(e.fit_intercept,e.fit_gradient,x_plus,y_plus) < short_res(e.fit_intercept,e.fit_gradient,x_minus,y_minus)){
                e[i].set_hit_pos(x_plus,y_plus);
            }
            else{
                e[i].set_hit_pos(x_minus,y_minus);
            }

        }

    }
}

event calculation(event& E){

    weighted_least_squares(E,no_weight,false);
    weighted_least_squares(E,inverse_weight,false);
    drift_time_calculation(E);

    hit_finder(E);
    weighted_least_squares(E,weight,true);
    drift_time_calculation(E);

    double drift_v_new = 0.0;
    double drift_v_old = 0.0;
    double ratio = 1.0;
    int iteration_number = 0;

    while (!E.good_fit){
        drift_v_new = E.fit_gradient;

        hit_finder(E);
        error_least_squares(E);
        drift_time_calculation(E);

        drift_v_old = E.fit_gradient;
        ratio = drift_v_old - drift_v_new;

        if (E.get_hit_number() < 4 | iteration_number >= 30)
        {
            break;
        }

        else if (std::fabs(ratio) < 0.00001)
        {
            E.good_fit = true;
            break;
        }

        else
        {
            ++iteration_number;
        }

    } 
    return E;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now(); 
    int event_start = 0;
    int total_event_no = 1000000;
    int buffer_size = 10000;
    std::ofstream myfile;
    myfile.open ("test.txt");
    myfile << "drift_velocity" << "," << "fit_gradient" << '\n';
    myfile.close();

    
    for (int k=event_start; k<event_start+total_event_no/buffer_size; ++k){
        std::vector<event> event_buffer = reading(k,buffer_size);
        for (event E:event_buffer)
        {
            myfile.open("test.txt", std::ios_base::app);
            E = calculation(E);
            if (E.good_fit)
                myfile << E.drift_velocity << "," << std::atan(E.fit_gradient) << '\n';
                myfile.close();
            
        }
        
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}


