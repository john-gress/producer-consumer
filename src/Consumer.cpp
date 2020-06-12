#include "Consumer.h"
#include <iostream>
#include <thread>
#include <chrono>

Consumer::Consumer(std::string confFile, int numBufs, std::string& match) :
   mConfig(confFile),
   mSharedBufferReader(numBufs, mConfig.GetSizeSharedBuf(), mConfig.GetSharedBufName(), false),
   mSentence(match, mConfig.GetSizeSharedBuf()), 
   mIpc(numBufs, mConfig.GetBufferSemaName(), mConfig.GetFinishedSemaName(), false),
   mBufSize(mConfig.GetSizeSharedBuf())
{
}

Consumer::~Consumer() {
}

void Consumer::Run() {
   char buf[mBufSize];
   while (mIpc.BufsAvailable() || ! mIpc.IsProducerFinished()) {
      if (mIpc.BufsAvailable()) {
         // Process buffer
         mSharedBufferReader.GetNextBuffer(buf);
         mIpc.DecrementBuffer();
         mSentence.ProcessBuf(buf);
      } else {
         // No buffers to process, but producer is not finished.
         std::this_thread::sleep_for(std::chrono::microseconds(1));
      }
   }
   mIpc.SignalFinished();

   std::this_thread::sleep_for(std::chrono::microseconds(100));
}

void Consumer::ReportStats() {
   std::cout << std::endl;
   std::cout << "========================================================================" << std::endl;
   std::cout << "Buffers received from producer: " << mSharedBufferReader.GetBufCount() << std::endl;
   std::cout << "Valid sentences: " << mSentence.GetSentenceCount() << std::endl;
   std::cout << "Corrupt sentences: " << mSentence.GetCorruptCount() << std::endl;
   std::cout << "Corrupt sizes: " << mSentence.GetBadSizeCount() << std::endl;
   std::cout << "Matching sentences: " << mSentence.GetMatchedCount() << std::endl;
   std::cout << "========================================================================" << std::endl;
}
