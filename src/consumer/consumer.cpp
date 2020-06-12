#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "Consumer.h"

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

   Consumer consumer("../conf/pcConf", N, match);
   consumer.Run();
}
