#include "Producer.h"
#include <iostream>
#include <thread>
#include <chrono>

Producer::Producer(std::string confFile, int numBufs, std::string& inputFile) :
   mConfig(confFile),
   mSharedBufferWriter(numBufs, mConfig.GetSizeSharedBuf(), mConfig.GetSharedBufName(), true),
   mFileInput(inputFile, mConfig.GetSizeSharedBuf()),
   mIpc(numBufs, mConfig.GetBufferSemaName(), mConfig.GetFinishedSemaName(), true),
   mBufSize(mConfig.GetSizeSharedBuf())
{
}

Producer::~Producer() {
}

void Producer::Run() {
   std::string sBuf;
   sBuf.reserve(mBufSize);
   while (! mFileInput.EndOfFile()) {
      WaitForAvailableBufs();
      mFileInput.GetNextSentence(sBuf);
      if (sBuf.length() > 0) {
         bool bufWritten = mSharedBufferWriter.AddSentenceToBuffer(sBuf);
         if (bufWritten) {
            mIpc.IncrementBuffer();
         }
      }
   }
   // Send a partial buffer (the last one) to the consumer
   char* shrBufPtr;
      
   bool bufWritten = mSharedBufferWriter.WriteBuf(shrBufPtr);
   if (bufWritten) {
      mIpc.IncrementBuffer();
   }
   mIpc.SignalFinished();
}

void Producer::WaitForAvailableBufs() {
   while (mIpc.QueueFull()) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
   }
}

void Producer::Shutdown() {
   while (! mIpc.IsConsumerFinished()) {
      std::this_thread::sleep_for(std::chrono::microseconds(1));
   }
}

void Producer::ReportStats() {
   std::cout << std::endl;
   std::cout << "========================================================================" << std::endl;
   std::cout << "Buffers sent to consumer: " << mSharedBufferWriter.GetBufCount() << std::endl;
   std::cout << "Sentences process from input file: " << mFileInput.GetSentenceCount() << std::endl;
   std::cout << "Oversized sentences in input file: " << mFileInput.GetOversizeCount() << std::endl;
   std::cout << "========================================================================" << std::endl;
}
