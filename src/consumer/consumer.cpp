#include <iostream>
#include "pc.h"
#include "Config.h"
#include <unistd.h>
#include <cstdlib>

int ProcessArgs(int argc, char* argv[], int& N, std::string& match) {
   int opt;
   const int kMinNumBufs(10);
   const int kExpectedArgCount(5);

   if (argc != kExpectedArgCount) {
      return 1;
   }

   while ((opt = getopt(argc, argv, "n:m:")) != -1) {
      switch (opt) {
         case 'n':
            N = atoi(optarg);
            break;
         case 'm':
            match = optarg;
            break;
         default:
            return 2;
      }
   }

   if (N < kMinNumBufs) {
      std::cerr << "Minumun value for -n argument is " << kMinNumBufs << std::endl;
      return 3;
   }

   return 0;
}

int main(int argc, char* argv[]) {
   int N;
   std::string match;

   int res = ProcessArgs(argc, argv, N, match);
   if (res != 0) {
      std::cerr << "Usage: " << argv[0] << " -n [NUM_BUFS] -m [MATCH_STRING]" << std::endl;
      exit(res);
   }

   std::cout << "Num Bufs: " << N << ", match: " << match << std::endl;

   pc myPc;
   Config myConfig("../conf/pcConf");

   if (myPc.pcTest()) {
      std::cout << "Consumer: Hello pcTest" << std::endl;
   } else {
      std::cout << "Consumer: No Hello pcTest" << std::endl;
   }
}
