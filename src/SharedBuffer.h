#pragma once

#include <string>

class SharedBuffer {
   public:
      SharedBuffer(int numBufs, int sizeBuf, std::string bufName, bool create);
      virtual ~SharedBuffer();

   protected:
      char* GetSharedMemBufPtr();

      void SetupSharedBuffer(bool create);
      int mNumBufs;
      int mBufSize;
      int mBufCount;
      std::string mSharedMemName;
      int mShrMemFd;
      void* mShrMemPtr;

};
