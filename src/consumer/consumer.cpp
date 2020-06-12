#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include "Consumer.h"

int ProcessArgs(int argc, char* argv[], int& N, std::string& match, std::string& confFile) {
   int opt;
   const int kMinNumBufs(10);
   const int kExpectedArgCount(7);

   if (argc != kExpectedArgCount) {
      return 1;
   }

   while ((opt = getopt(argc, argv, "n:m:c:")) != -1) {
      switch (opt) {
         case 'n':
            N = atoi(optarg);
            break;
         case 'm':
            match = optarg;
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
   std::string match;
   std::string confFile;

   int res = ProcessArgs(argc, argv, N, match, confFile);
   if (res != 0) {
      std::cerr << "Usage: " << argv[0] << " -n [NUM_BUFS] -m [MATCH_STRING] -c [CONFIG_FILE]" << std::endl;
      exit(res);
   }

   std::cout << "Num Bufs: " << N << ", match: " << match << ", confFile: " << confFile << std::endl;

   Consumer consumer(confFile, N, match);
   consumer.Run();
   consumer.ReportStats();
}
