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

   protected:

   private:
      Config mConfig;
      SharedBufferReader mSharedBufferReader;
      Sentence mSentence;
      Ipc mIpc;
      int mBufSize;
      unsigned long mBufsReceived;
      unsigned long mSentenceMatch;
};
