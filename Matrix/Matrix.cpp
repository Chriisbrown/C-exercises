// Matrix.cpp - implementation file
//
#include "Matrix.hpp"
#include "Vector.hpp"
#include <iostream>
#include <cmath>
//convert degrees to radians.
static const double degrad = std::acos(-1.0)/180.;

Matrix::Matrix()
{   
    for (int i=0 ; i<4 ; i++ )
    {   
        for (int j=0 ; j<4 ; j++ )
            m[i][j] = (i==j) ? 1.0 : 0.0 ; 
    }
}

Matrix::Matrix(const double* a)
{
    double *p = &m[0][0];
    const double *q = a;
    while (p <= &m[3][3]) *p++ = *q++ ;
}

Matrix Matrix::operator*(const Matrix& b)
{ 
    Matrix p;
    for (int i=0 ; i<4 ; i++ )
    { 
        for (int j=0 ; j<4 ; j++ )
        {
            double sum=0.0;
            for (int k=0 ; k<4 ; k++)
            {
                sum += m[i][k]*b.m[k][j]; }
                p.m[i][j] = sum;
        }
    }
    return p;
}

void Matrix::rotX(double degrees)
{ double ang = degrad*degrees;
double c = std::cos(ang);
double s = std::sin(ang);
Matrix r;
r.m[1][1] = c;
r.m[1][2] = -s;
r.m[2][1] = s;
r.m[2][2] = c;
*this = r*(*this);
}