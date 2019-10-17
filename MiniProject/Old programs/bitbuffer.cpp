#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>
#include <chrono>

const std::string s = "onetrack.raw";

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

        friend std::ostream& operator<<(std::ostream& os, const hit& h);
        

        ~hit(){};

    private:
        double id;
        double x_coordinate;
        double y_coordinate;
        double drift_time;

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



std::vector<double> reading(int start)
{
    
    std::ifstream is (s,std::ios::binary);
    char r[2] = {' ',' '};
    std::vector<double> return_values = {0.0,0.0,0.0,0.0};

    if (is){
            for (int j=0;j<2; ++j){

                is.seekg (start*2+j);
                char* buffer = new char;
                is.read(buffer,1);
                


                r[j] = *buffer;

                delete buffer;
            }

        
         
        std::bitset<16> y(r[1]);
        std::cout << x <<'\t' << y << '\n';
        
            
        x = x << 8;
        x = x ^ y;

        std::cout << x << '\n';

        //std::cout << x << '\n';

        std::bitset<3> x_pos;
        std::bitset<3> y_pos;
        std::bitset<10> drift_time;

        for (int k = 0; k < 3; ++k){
                    x_pos[2-k] = x[15-k];
                    y_pos[2-k] = x[12-k];
            }
        for (int k = 0; k<10; ++k){
                    drift_time[9-k] = x[9-k];
            }

        double Y_adjusted = y_pos.to_ulong();
        return_values[0] = start;
        return_values[1] = x_pos.to_ulong();

        if (x_pos.to_ulong() % 2 != 0) {
            Y_adjusted += 0.5;
        }
        return_values[2] = Y_adjusted;
        return_values[3] = drift_time.to_ulong();

        
    }
    is.close();
    return return_values;
}


class event{
    private:
        int event_id;
        std::vector<hit> event_hits;
    public:

        event() : event_id{0} {};
        event(int n) : event_id{n} {};

        int get_hit_number() {return event_hits.size();}

        ~event() {};

        hit &operator[] (int index){
            return event_hits[index];
        } 


        event& read_event();

        void remove_hit(int index){
            event_hits.erase(event_hits.begin() + index);
        }

        void print();
};


event& event::read_event(){
    for (int i=0; i<8; ++i){
        std::vector<double> reading_vals = reading(i+8*event_id);
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


void least_squares(event& e)
{
    int n = e.get_hit_number();
    double product = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double X_squared_mean = 0.0;
    double b = 0.0;

    for (int i=0; i<n; ++i){
        product += e[i][1]*e[i][2];
        X_mean += e[i][1];
        Y_mean += e[i][2];
        X_squared_mean += e[i][1]*e[i][1];
    }
    b = ((product/n) - (X_mean/n)*(Y_mean/n))/((X_squared_mean/n)-((X_mean*X_mean)/(n*n)));
    double a = Y_mean/n - b*X_mean/n;
    for (int i=0; i<n; ++i){
        double res = e[i][2] - ((b*e[i][1]) + a);
        if (res > 1){
            e.remove_hit(i);
        }
    }
    /*
    std::cout << "initial fit: " << b << '\n';
    std::cout << "removed: " << n-e.get_hit_number() << " hits\n";
    std::cout << "===================" << '\n';
    */
}

void weighted_least_squares(event& e)
{
    int n = e.get_hit_number();
    double numerator = 0.0;
    double X_mean = 0.0;
    double Y_mean = 0.0;
    double W_mean = 0.0;
    double denominator = 0.0;
    double b = 0.0;



    for (int i=0; i<n; ++i){
        X_mean += (1000/(400+e[i][3]*e[i][3]))*e[i][1];
        Y_mean += (1000/(400+e[i][3]*e[i][3]))*e[i][2];
        W_mean += (1000/(400+e[i][3]*e[i][3]));
    }
    X_mean = X_mean/W_mean;
    Y_mean = Y_mean/W_mean;

    for (int i=0; i<n; ++i){
        numerator += (1000/(400+e[i][3]*e[i][3]))*(e[i][1]-X_mean)*(e[i][2]-Y_mean);
        denominator += (1000/(400+e[i][3]*e[i][3]))*(e[i][1]-X_mean)*(e[i][1]-X_mean);
    }

    

    b = numerator/denominator;
    double a = Y_mean - b*X_mean;


    for (int i=0; i<n; ++i){
        double res = e[i][2] - ((b*e[i][1]) + a);
        if (res > 5){
            e.remove_hit(i);
        }
    }

    

    std::cout << "weighted fit: " << b << ',' << a << '\n';
    std::cout << "removed: " << n-e.get_hit_number() << " hits\n";
    std::cout << "===================" << '\n';
     
}




int main(){
    auto start = std::chrono::high_resolution_clock::now();  
    for (int k=0; k<1; ++k){
        event E(k);
        E.read_event();
        //E.print();
        least_squares(E);
        weighted_least_squares(E);
    }

    

    

        
        
        
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}


