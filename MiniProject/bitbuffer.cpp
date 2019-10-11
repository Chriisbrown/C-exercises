#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>
#include <chrono>

class hit
{
    public:
        hit() : id{0}, x_coordinate{0} , y_coordinate{0} , drift_time{0} {}; //default constructor

        hit(int number, int x, int y, int t) : id{number}, x_coordinate{x} , y_coordinate{y} , drift_time{t} {}; //constructor

        hit& operator=(const hit &); //copy assignment

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
        int id;
        int x_coordinate;
        int y_coordinate;
        int drift_time;

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
            os << "ID: " << h.id << " X:" << h.x_coordinate << " Y: " << h.y_coordinate << " Drift Time: " << h.drift_time << '\n';
            return os;
        }



std::vector<int> reading(int start)
{
    
    std::ifstream is ("onetrack.raw",std::ios::binary);
    char r[2] = {' ',' '};
    std::vector<int> return_values = {0,0,0,0};

    if (is){
            for (int j=0;j<2; ++j){

                is.seekg (start*2+j);
                char* buffer = new char;
                is.read(buffer,1);

                r[j] = *buffer;

                delete buffer;
            }
        
        std::bitset<16> x(r[0]);
        std::bitset<16> y(r[1]);
            
        x = x << 8;
        x = x ^ y;

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

        return_values[0] = start;
        return_values[1] = x_pos.to_ulong();
        return_values[2] = y_pos.to_ulong();
        return_values[3] = drift_time.to_ulong();

        
    }
    is.close();
    return return_values;
}


int main(){
    auto start = std::chrono::high_resolution_clock::now();  
    for (int i=0; i<8; ++i){

        std::vector<int> reading_vals = reading(i);
        hit h1(reading_vals[0],reading_vals[1],reading_vals[2],reading_vals[3]);
        std::cout << h1;

    }

        
        
        
    
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    return 0;
}


