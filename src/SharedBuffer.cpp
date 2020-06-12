#include "SharedBuffer.h"
#include <iostream>
#include <unistd.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/mman.h>
#include <cerrno>
#include <cstring>

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
      std::string errnoMsg(strerror(errno));
      std::cerr << "Unable to truncate shared memory, " << mSharedMemName << ", to size:"
         << mNumBufs * mBufSize << ", " << errnoMsg << std::endl;
      exit(1);
   }

   mShrMemPtr = mmap(NULL, // let system pick where to put segment
         mNumBufs * mBufSize, // Number of bytes to allocate
         PROT_READ | PROT_WRITE, // access protections
         MAP_SHARED, // mapping visible to other processes
         mShrMemFd,
         0); // offset: start at 1st byte
   if ((void*) -1 == mShrMemPtr) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Unable to access shared memory: " << mSharedMemName << ", " << errnoMsg << std::endl;
      exit(1);
   }
}

SharedBuffer::~SharedBuffer() {
   if (munmap(mShrMemPtr, mNumBufs * mBufSize) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Unable to munmap memory: " << mSharedMemName << ", " << errnoMsg << std::endl;
   }
   if (shm_unlink(mSharedMemName.c_str()) != 0 && errno != ENOENT) {
      // ENOENT = No such file or directory. This is normal, as there is a race condition for which process deletes 
      // the file.
      std::string errnoMsg(strerror(errno));
      std::cerr << "Unable to unlink file: " << mSharedMemName << ", " << errnoMsg << std::endl;
   }
}

char* SharedBuffer::GetSharedMemBufPtr() {
   char* bufPtr = reinterpret_cast<char *>(mShrMemPtr);
   return (bufPtr + ((mBufCount % mNumBufs) * mBufSize));
}

