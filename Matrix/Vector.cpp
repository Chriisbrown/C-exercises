// Vector.cpp - implementation file
//for Vector class.
//
#include "Vector.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std; // lazy!

void Vector::print(int w)
{   
// Print 1st 3 elements of vector.
// 4th element is always 1.0
//
    if (w > 0)
    { 
        for (int j=0 ; j<3 ; j++ )
        cout << " " << setw(w) << v[j];
    }
    else
    { 
        for (int j=0 ; j<3 ; j++ )
        cout << " " << v[j];
    }

    cout << '\n';
}