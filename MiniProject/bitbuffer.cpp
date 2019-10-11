#include <iostream>
#include <fstream>
#include <bitset>
#include <cmath>
#include <vector>
#include <chrono>


class bits
{
    private:
        char* mem_buff;
    public:

        int n_bits;

        bits() {};

        bits(int n) : n_bits{n} , mem_buff{new char[n]}
        {
            for (int i=0; i<n; ++ i) mem_buff[i]=0;
        }

        ~bits()
        {
            delete mem_buff;
        }

        int decode();

        void print();

        bits reading(bits&);

};

int bits::decode(){
    return int(*mem_buff); 
     
};

void bits::print(){
    std::cout<<*mem_buff << '\n';
};

/*

class hit
{
    public:
        hit() {
            x_coordinate = bits(3);
            y_coordinate = bits(3);
            drift_time = bits(10);
        };
        ~hit(){};

        void fill_hit();

    private:
        bits x_coordinate;
        bits y_coordinate;
        bits drift_time;

};

void hit::fill_hit()
{
    x_coordinate.reading();
    y_coordinate.reading();
    drift_time.reading();

}




*/
char reading(int start)
{

    std::ifstream is ("manytracks.raw",std::ios::binary);
    char r = {' '};

    if (is){
        is.seekg (start);
        char* buffer = new char;
        is.read(buffer,1);

        r = *buffer;

        delete buffer;
        }
    
    is.close();
    return r;

}


int main(){
    for (int k=0; k<1000000; ++k){
        //std::cout << k << '\n';

        for (int i=0; i<8; ++i){
            char r = reading(k+2*i);
            char r2 = reading(k+2*i+1);
            
            std::bitset<16> x(r);
            std::bitset<16> y(r2);
            
            x = x << 8;
            x = x ^ y;

            std::bitset<3> x_pos;
            std::bitset<3> y_pos;
            std::bitset<10> drift_time;

            for (int j = 0; j<3; ++j){
                    x_pos[2-j] = x[15-j];
                    y_pos[2-j] = x[12-j];
            }
            for (int j = 0; j<10; ++j){
                    drift_time[9-j] = x[9-j];
            }


/*
            std::cout << "X: " << x_pos.to_ulong() << 
            " Y: " << y_pos.to_ulong() << " Time: " 
            << drift_time.to_ullong()  << '\n';

  */      
        
        }
    }
    return 0;
}


