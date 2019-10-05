#include <iostream>

int main(int argc, char *argv[])
{
  for (int i =0; i<argc; ++i){
    std::cout << argv[i] << '\n';
  }
  return 0;
}

//c = a > b ? a : b;  exp0 ? exp1 : exp2 (exp0 if true then exp1, if false then exp2 )
// x[rows][columns] with columns iterated through fastest

union int_or_float
{
    int ival;
    float fval;
};

//friend Vector Matrix::operator*(const Vector& v); allowing private components to be accessed by friend classes e.g. matrix*vector


class Shape {
//...
public:
virtual void draw() =0; // pure virtual function.
// ...
};

// classes with puyrely virtual functions cannot be called, imhereted classes have to define their draw functions and then can be used