#include <iostream>
#include "pc.h"
#include "Config.h"

int main(int argc, char* argv[]) {
   pc myPc;
   Config myConfig("../conf/pcConf");

   if (myPc.pcTest()) {
      std::cout << "Consumer: Hello pcTest" << std::endl;
   } else {
      std::cout << "Consumer: No Hello pcTest" << std::endl;
   }
}
