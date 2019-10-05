#include "Box.hpp"
#include "Tube.hpp"


int main(){
    Box box1(2.0,2.0,2.0);
    point p1;
    p1.x = 1,0, p1.y = 1.0, p1.z = 1.0;
    Box box2(p1);
    Box box3(3.0,3.0,3.0);
    Tube tube1(1.0,2.0,4.0);


    point p2(0.0,0.0,0.0);
    point p3(-0.5,-0.5,-0.5);
    point p4(2.0,2.0,2.0);

    volume *vols[6]; // for example
    vols[0] = &box1;
    vols[1] = &box2;
    vols[2] = &box3;
    vols[3] = &tube1;
    vols[4] = new Box;
    vols[5] = new Tube;




    for (int i=0; i<6; i++)
    {
      bool isin = vols[i]->contains(0.5, 0.5, 0.5);
      std::cout << isin << '\n';
    }

    return 0;
}