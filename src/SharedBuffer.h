#pragma once

#include <string>

class SharedBuffer {
   public:
      SharedBuffer(int numBufs, int sizeBuf, std::string bufName, bool create);
      virtual ~SharedBuffer();

      unsigned long GetBufCount() {
         return mBufCount;
      }

   protected:
      char* GetSharedMemBufPtr();

      void SetupSharedBuffer(bool create);
      int mNumBufs;
      int mBufSize;
      unsigned long mBufCount;
      std::string mSharedMemName;
      int mShrMemFd;
      void* mShrMemPtr;

};
