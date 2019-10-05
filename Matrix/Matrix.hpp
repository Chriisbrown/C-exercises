// Matrix.hpp - header file for Matrix class.
//
// Needed so we know Vector class exists somewhere.
class Vector;
class Matrix {

    double m[4][4];

    public:
        Matrix(); // Initialise to unit matrix.
        Matrix(const double* a); // Initialise by array.

        Matrix operator*(const Matrix& b);
        Vector operator*(const Vector& v);
        // rotations about x,y,z axes.
        void rotX(double degrees);
        void rotY(double degrees);
        void rotZ(double degrees);
        // translate
        void trans(double tx, double ty, double tz);
        void trans(const double* t);
        // scale
        void scale(double sx, double sy, double sz);
        void scale(const double* s);
        void print();

        };

inline void Matrix::trans(const double* t)
{   
    double tx, ty, tz;
    tx = t[0];
    ty = t[1];
    tz = t[2];
    trans(tx, ty, tz);
        }

inline void Matrix::scale(const double* s)
{ 
    double sx, sy, sz;
    sx = s[0];
    sy = s[1];
    sz = s[2];
    scale(sx, sy, sz);
        }