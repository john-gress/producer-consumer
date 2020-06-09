#include <iostream>
#include "pc.h"

int main(int argc, char* argv[]) {
   pc myPc;

   if (myPc.pcTest()) {
      std::cout << "Hello pcTest" << std::endl;
   } else {
      std::cout << "No Hello pcTest" << std::endl;
   }
}
