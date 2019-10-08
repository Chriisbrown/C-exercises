#include <iostream>
#include <fstream>
#include <iterator>
#include <exception>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;


template<typename I> 
void my_sort(I first, I last)
{
    I original = first;
    while (first!=(last-1)){
        if (*first >= *(first+1))
            ++first;
        else {
            auto temp = *(first+1);
            *(first+1)  = *first;
            *first = temp;
            first = original; 
        }
    }
    return;
}




int main(int argc, char *argv[]){
    string iname = argv[1];
    ifstream ist {iname};
    if (!ist) cerr << "can't open input file" << iname << '\n';

    vector<double> reading;
    double buff;
    while (ist >> buff){
        reading.push_back(buff);
    } 

    list<int> my_list = {04,1,21,3,43,5,7,56,8,19};

    my_sort(reading.begin(),reading.end());

    my_list.sort();

    copy(my_list.begin(),my_list.end(),ostream_iterator<int>(cout, " "));


    for (int i=0; i<reading.size(); ++i){
        if (i%5 == 0) cout << '\n'; 
        cout << reading[i] << '\t'; 
    }
    cout << '\n';

    return 0;
}