#include "lekser.hpp"

int main(void) {
  Token t;
  while((t = getToken()).tag != 0){
    t.print();
  }
  return 0;
}
