// Vector.hpp - Header file for Vector class.
//
class Matrix; // Needed so we know Matrix class exists somewhere.
class Vector {
double v[4]; // Last element is always 1.0 for matrix mult.
public:
// Provide simple constructors as inline functions.
//
Vector(double x=1.0, double y=0.0, double z=0.0)
{ v[0]=x; v[1]=y; v[2]=z; v[3]=1.0; }
Vector(const double* x)
{ for (int i=0; i<3; i++) v[i]=x[i];
v[3]=1.0; }
friend Vector Matrix::operator*(const Vector& v);
void print(int w=0);
} ;