#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main () {
  vector<int> size{3,3,10};
  char *memblock;

  ifstream file ("onetrack.raw", ios::in|ios::binary);
  if (file.is_open())
  {
      for (int i : size){
        cout << i << '\n';
        memblock = new char[i];
        //file.seekg (0, ios::beg);
        file.read (memblock, i);
        cout << int(*memblock) << '\n';

        delete[] memblock;
      }
      file.close();
  }

  else cout << "Unable to open file";
  return 0;
}



