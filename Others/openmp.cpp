  #include <cmath>
  #include <iostream>
  int main()
  {
    const int size = 256;
    double sinTable[size];
    
#pragma omp parallel for
 for(int n=0; n<25; ++n)
 {
   printf(" %d", n);
 }
 printf(".\n");
  }

 //g++ -o opmp.exe openmp.cpp -fopenmp
 