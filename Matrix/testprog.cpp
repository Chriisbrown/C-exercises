// prog.cpp - test program for Matrix class.
//
#include "Matrix.hpp"
#include "Vector.hpp"
#include <iostream>
using namespace std; // This is very bad!

int main()
{
    Vector p[8];
// Initialise coordinates of corners of a cube.
//
    { 
        int n=0;
        for (int k=-1; k<=1; k+=2)
        { 
            for (int j=-1 ; j<=1; j+=2)
            {
                for (int i=-1 ; i<=1; i+=2)
                {  
                    Vector x(i,j,k);
                    p[n] = x;
                    n++;
                }
            }
        }
    }
    cout << "Initial values of Vector p:" << endl;
    int i;
    for (i=0; i<8; i++) p[i].print(4);
// Define various transformations to apply to p.
//
    Matrix t;
    t.scale(3, 9, 1);
    t.rotY(20.);
    t.rotX(10.);
 // apply the transformation to p.
    for (i=0; i<8; i++) p[i] = t*p[i];

    cout << "Transformed values of Vector p:" << '\n';
    for (i=0; i<8; i++) p[i].print(10);

    return 0;
    
}