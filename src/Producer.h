#pragma once
#include "Config.h"
#include "SharedBufferWriter.h"
#include "FileInput.h"
#include "Ipc.h"


class Producer {
   public:
      Producer(std::string confFile, int numBufs, std::string& inputFile);
      ~Producer();
      void Run();
      void Shutdown();

   protected:
      void WaitForAvailableBufs();
   private:
      Config mConfig;
      SharedBufferWriter mSharedBufferWriter;
      FileInput mFileInput;
      Ipc mIpc;
      int mBufSize;
      unsigned long mBufsSent;
      unsigned long mSentenceCount;
};
