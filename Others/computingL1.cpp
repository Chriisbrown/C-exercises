#include <iostream>
#include <string>

std::ostream& operator<<(std::ostream& os, std::string s)
{
    os << s << '\n';
    return os;
}

int main(){
  std::cout << "hello world";
  std::string q = " ";
  std::cout << "enter something to be printed";

  while (std::cin >> q){
    std::cout << q << '\n';
  }
  return 0;
}

