#include <iostream>
#include <unistd.h>
#include "SharedBuffer.h"
#include "Config.h"
#include "Ipc.h"
#include <cstdlib>
#include <ctime>

class ChaosBuffer : public SharedBuffer {
   public:
      ChaosBuffer(int numBufs, int sizeBuf, std::string bufName) :
         SharedBuffer(numBufs, sizeBuf, bufName, false)
      {
         srand(time(nullptr));
      }
      ~ChaosBuffer(){}

      void CorruptBuf() {
         char* shrBufPtr;
         shrBufPtr = GetSharedMemBufPtr();
         printf("Corrupting Buf starting at: %p\n", shrBufPtr);
         int offset(0);
         while (offset < mBufSize) {
            offset += rand() % 10;
            char c = static_cast<char>(rand() % 256);
            if (c < 127) {
               c += 126;
            }
            *(shrBufPtr+offset) = c;
         }

         mBufCount++;
      }
};

int ProcessArgs(int argc, char* argv[], int& N, std::string& confFile) {
   int opt;
   const int kMinNumBufs(10);
   const int kExpectedArgCount(5);

   if (argc != kExpectedArgCount) {
      return 1;
   }

   while ((opt = getopt(argc, argv, "n:c:")) != -1) {
      switch (opt) {
         case 'n':
            N = atoi(optarg);
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
   std::string confFile;

   int res = ProcessArgs(argc, argv, N, confFile);
   if (res != 0) {
      std::cerr << "Usage: " << argv[0] << " -n [NUM_BUFS] -f [FILE]" << std::endl;
      exit(res);
   }

   std::cout << "Num Bufs: " << N << ", confFile: " << confFile << std::endl;

   Config mConfig(confFile);
   ChaosBuffer mChaosBuffer(N, mConfig.GetSizeSharedBuf(), mConfig.GetSharedBufName());
   Ipc mIpc(N, mConfig.GetBufferSemaName(), mConfig.GetFinishedSemaName(), false);

   while (!mIpc.IsConsumerFinished()) {
      mChaosBuffer.CorruptBuf();
   }
}
