#pragma once
#include "Config.h"
#include "SharedBufferReader.h"
#include "Sentence.h"
#include "Ipc.h"


class Consumer {
   public:
      Consumer(std::string confFile, int numBufs, std::string& match);
      ~Consumer();
      void Run();
      void ReportStats();

   protected:

   private:
      Config mConfig;
      SharedBufferReader mSharedBufferReader;
      Sentence mSentence;
      Ipc mIpc;
      int mBufSize;
};
