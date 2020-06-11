#include "SharedBuffer.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/mman.h>

SharedBuffer::SharedBuffer(int numBufs, int sizeBuf, std::string bufName, bool create) :
   mNumBufs(numBufs),
   mBufSize(sizeBuf),
   mBufCount(0),
   mSharedMemName(bufName),
   mShrMemFd(-1),
   mShrMemPtr(nullptr)
{
   SetupSharedBuffer(create);
}

void SharedBuffer::SetupSharedBuffer(bool create) {
   mode_t mode(0644);
   int oflag = O_RDWR;
   if (create) {
      oflag = O_RDWR | O_CREAT;
   }
   mShrMemFd = shm_open(mSharedMemName.c_str(), oflag, mode);
   if (mShrMemFd < 0) {
      std::cerr << "Unable to open file descriptor for shared memory: " << mSharedMemName << std::endl;
      exit(1);
   }

   if (ftruncate(mShrMemFd, mNumBufs * mBufSize) != 0) {
      std::cerr << "Unable to truncate shared memory, " << mSharedMemName << ", to size:"
         << mNumBufs * mBufSize << std::endl;
      exit(1);
   }

   mShrMemPtr = mmap(NULL, // let system pick where to put segment
         mNumBufs * mBufSize, // Number of bytes to allocate
         PROT_READ | PROT_WRITE, // access protections
         MAP_SHARED, // mapping visible to other processes
         mShrMemFd,
         0); // offset: start at 1st byte
   if ((void*) -1 == mShrMemPtr) {
      std::cerr << "Unable to access shared memory: " << mSharedMemName << std::endl;
      exit(1);
   }
}

SharedBuffer::~SharedBuffer() {
   munmap(mShrMemPtr, mNumBufs * mBufSize);
   shm_unlink(mSharedMemName.c_str());
}

char* SharedBuffer::GetSharedMemBufPtr() {
   char* bufPtr = reinterpret_cast<char *>(mShrMemPtr);
   /*
   int bufNum = mBufCount % mNumBufs;
   int bufOffset = bufNum * mBufSize;
   char* rBufPtr = bufPtr + bufOffset;
   return (rBufPtr);
   */
   return (bufPtr + ((mBufCount % mNumBufs) * mBufSize));
}

