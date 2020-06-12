#include "Consumer.h"
#include <iostream>
#include <thread>
#include <chrono>

Consumer::Consumer(std::string confFile, int numBufs, std::string& match) :
   mConfig(confFile),
   mSharedBufferReader(numBufs, mConfig.GetSizeSharedBuf(), mConfig.GetSharedBufName(), false),
   mSentence(match, mConfig.GetSizeSharedBuf()), 
   mIpc(numBufs, mConfig.GetBufferSemaName(), mConfig.GetFinishedSemaName(), false),
   mBufSize(mConfig.GetSizeSharedBuf()),
   mBufsReceived(0),
   mSentenceMatch(0)
{
}

Consumer::~Consumer() {
}

void Consumer::Run() {
   char buf[mBufSize];
   while (mIpc.BufsAvailable() || ! mIpc.IsProducerFinished()) {
      if (mIpc.BufsAvailable()) {
         // Process buffer
         mBufsReceived++;
         mSharedBufferReader.GetNextBuffer(buf);
         mIpc.DecrementBuffer();
         mSentenceMatch += mSentence.ProcessBuf(buf);
      } else {
         // No buffers to process, but producer is not finished.
         std::this_thread::sleep_for(std::chrono::microseconds(1));
      }
   }
   mIpc.SignalFinished();

   std::this_thread::sleep_for(std::chrono::seconds(2));
}

