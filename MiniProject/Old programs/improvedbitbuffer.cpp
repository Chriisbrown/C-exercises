#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>
#include <chrono>

const std::string s = "manytracks.raw";

class hit
{
    public:
        hit() : id{0}, x_coordinate{0} , y_coordinate{0} , drift_time{0} {}; //default constructor

        hit(double number, double x, double y, double t) : id{number}, x_coordinate{x} , y_coordinate{y} , drift_time{t} {}; //constructor

        hit& operator=(const hit &); //copy assignment

        double &operator[] (int index){
            double* def = 0;
            switch (index){
                case 0: return id;
                case 1: return x_coordinate;
                case 2: return y_coordinate;
                case 3: return drift_time;
                case 4: return hit_position[0];
                case 5: return hit_position[1];

                default: return *def;
            }
        }

        hit(const hit& h) : id{h.id}, x_coordinate{h.x_coordinate} , y_coordinate{h.y_coordinate} , drift_time{h.drift_time} {}; //copy constructor

        void set_values(int n, int x, int y, int t)
        {
            id = n;
            x_coordinate = x;
            y_coordinate = y;
            drift_time = t;
        }

        void set_hit_pos(double x, double y){
                hit_position[0] = x;
                hit_position[1] = y;
        }
        

        friend std::ostream& operator<<(std::ostream& os, const hit& h);
        

        ~hit(){};

    private:
        double id;
        double x_coordinate;
        double y_coordinate;
        double drift_time;
        double hit_position[2];
        

};

hit& hit::operator=(const hit& h)
{
    id = h.id;
    x_coordinate = h.x_coordinate;
    y_coordinate = h.y_coordinate;
    drift_time = h.drift_time;
    return *this;
}

std::ostream& operator<<(std::ostream& os, const hit& h)
    {
            os << "Hit_id: " << h.id << " X:" << h.x_coordinate << " Y: " << h.y_coordinate << " Drift Time: " << h.drift_time << '\n';
            return os;
    }

double bitextractor(short buf, int length, int position) 
{ 
    return static_cast<double>(((1 << length) - 1) & (buf >> (position))); 
}


std::vector<double> reading(int start)
{
    
    std::ifstream is (s,std::ios::binary);
    std::vector<double> return_values = {0.0,0.0,0.0,0.0};

    if (is){
        short val;

        is.seekg (start*2);
        char* buffer = new char[2];
        is.read(buffer,2);

        val = buffer[1];
        val = ((val << 8) | buffer[0] & 0xff);
        delete buffer;

        double x_pos = (bitextractor(val,3,0));
            
        double Y_adjusted = bitextractor(val,3,3);
        return_values[0] = start;
        return_values[1] = x_pos;

        if (std::fmod(x_pos,2) != 0) {
            Y_adjusted += 0.5;
        }
        return_values[2] = Y_adjusted;
        return_values[3] = bitextractor(val,10,6);

    is.close();
    }
    return return_values;
}


class event{
    private:
        int event_id;
        std::vector<hit> event_hits;
    public:

        double fit_gradient;
        double fit_intercept; 
        double drift_velocity;

        event() : event_id{0} {};
        event(int n) : event_id{n} {};

        int get_hit_number() {return event_hits.size();}

        ~event() {};

        hit &operator[] (int index){
            return event_hits[index];
        } 

        void update_fit(double I, double G){
            fit_intercept = I;
            fit_gradient = G;
        }


        void update_velocity(double V){
            drift_velocity = V;
        }

        hit& return_hit(int index){
            return event_hits[index];
        }


        event& read_event();

        void remove_hit(int index){
            event_hits.erase(event_hits.begin() + index);
        }

        void print();
        void quick_print();
};


event& event::read_event(){
    for (int i=0; i<8; ++i){
        std::vector<double> reading_vals = reading(i+event_id*8);
        hit h1(reading_vals[0],reading_vals[1],reading_vals[2],reading_vals[3]);
        event_hits.push_back(h1);
    }

    return *this;
}

void event::print(){
    std::cout << "Event_id: " << event_id << '\n';
    for (auto h : event_hits){
        std::cout << h;
    }
}

void event::quick_print(){
    std::cout << "Event id: " << event_id << '\n';
    std::cout << "Hit number: " << get_hit_number() << '\n';
    std::cout << "Event fit: " << fit_gradient << '\n';
    std::cout << "Event drift velocity: " << drift_velocity << '\n';
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
        if (res > 1){
            e.remove_hit(i);
        }
    }

    e.update_fit(intercept,gradient);    
}

double inverse_weight(double r){
    return (1000.0/(r*r + 400.0));
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
    if (hit){ shift = 3;}
    
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
    
}

void drift_time_calculation(event& e)
{
    int n = e.get_hit_number();
    double speed = 0.0;

    for (int i=0; i<n; ++i)
    {
        speed += short_res(e.fit_intercept,e.fit_gradient,e[i][1],e[i][2])/e[i][3];
    }
    e.drift_velocity = speed/n;
}

void hit_finder(event& e)
{
    int n = e.get_hit_number();
    for (int i=0; i<n;++i)
    {
        double x = e[i][1];
        double y = e[i][2];
        double r = e[i][3] * e.drift_velocity;

        
        
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

event calculation(event& E){
    least_squares(E);
    weighted_least_squares(E,inverse_weight,false);
    drift_time_calculation(E);

    for (int i=0; i< 1; ++i){
        //std::cout << "Iteration no: " << i << '\n';
        hit_finder(E);
        weighted_least_squares(E,weight,true);
        drift_time_calculation(E);
        //E.quick_print();
        //std::cout << "====================" << '\n';
        }

    return E;
}

int main(){
    auto start = std::chrono::high_resolution_clock::now(); 
    for (int k=0; k<10; ++k){
        event E(k);
        E.read_event();
        E.print();
        E = calculation(E);
        std::cout << "Event: " << k <<" Velocity: " << E.drift_velocity << " Gradient: " << E.fit_gradient << '\n';
        
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}


