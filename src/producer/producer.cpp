#include <iostream>
#include "pc.h"
#include "Config.h"

int main(int argc, char* argv[]) {
   pc myPc;
   Config myConf("../conf/pcConf");

   if (myPc.pcTest()) {
      std::cout << "Producer: Hello pcTest" << std::endl;
   } else {
      std::cout << "Producer: No Hello pcTest" << std::endl;
   }
}
