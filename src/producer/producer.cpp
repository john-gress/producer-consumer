#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "Producer.h"

int ProcessArgs(int argc, char* argv[], int& N, std::string& filename) {
   int opt;
   const int kMinNumBufs(10);
   const int kExpectedArgCount(5);

   if (argc != kExpectedArgCount) {
      return 1;
   }

   while ((opt = getopt(argc, argv, "n:f:")) != -1) {
      switch (opt) {
         case 'n':
            N = atoi(optarg);
            break;
         case 'f':
            filename = optarg;
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
   std::string filename;

   int res = ProcessArgs(argc, argv, N, filename);
   if (res != 0) {
      std::cerr << "Usage: " << argv[0] << " -n [NUM_BUFS] -f [FILE]" << std::endl;
      exit(res);
   }

   std::cout << "Num Bufs: " << N << ", filename: " << filename << std::endl;

   Producer producer("../conf/pcConf", N, filename);
   producer.Run();
   producer.ReportStats();
   producer.Shutdown();
}
