#include <iostream>
#include <fstream>
#include <exception>
#include <vector>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]){
    string iname = argv[1];
    ifstream ist {iname};
    if (!ist) cerr << "can't open input file" << iname << '\n';

    vector<double> reading;
    double buff;
    while (ist >> buff){
        reading.push_back(buff);
    } 

    sort(reading.begin(),reading.end());

    for (int i=0; i<reading.size(); ++i){
        if (i%5 == 0) cout << '\n'; 
        cout << reading[i] << '\t'; 
    }
    cout << '\n';

    return 0;
}