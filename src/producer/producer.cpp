#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "Producer.h"

int ProcessArgs(int argc, char* argv[], int& N, std::string& inputFile, std::string& confFile) {
   int opt;
   const int kMinNumBufs(10);
   const int kExpectedArgCount(7);

   if (argc != kExpectedArgCount) {
      return 1;
   }

   while ((opt = getopt(argc, argv, "n:f:c:")) != -1) {
      switch (opt) {
         case 'n':
            N = atoi(optarg);
            break;
         case 'f':
            inputFile = optarg;
            break;
         case 'c':
            confFile = optarg;
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
   std::string inputFile;
   std::string confFile;

   int res = ProcessArgs(argc, argv, N, inputFile, confFile);
   if (res != 0) {
      std::cerr << "Usage: " << argv[0] << " -n [NUM_BUFS] -f [FILE] -c [CONF_FILE]" << std::endl;
      exit(res);
   }

   std::cout << "Num Bufs: " << N << ", inputFile: " << inputFile << ", confFile: " << confFile << std::endl;

   Producer producer(confFile, N, inputFile);
   producer.Run();
   producer.ReportStats();
   producer.Shutdown();
}
