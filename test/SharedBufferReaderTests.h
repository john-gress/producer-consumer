#pragma once

#include "gtest/gtest.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <sys/mman.h>

class SharedBufferReaderTests : public ::testing::Test
{
   public:
      SharedBufferReaderTests() :
         mSharedMemName{"mySharedMem"},
         mNumBufs(10),
         mBufSize(1024),
         mShrMemFd(-1),
         mShrMemPtr(nullptr)
         {};

   protected:
      virtual void SetUp() {
         // Create the shared memory block so the Reader can open it for Read/Write access
         SetupSharedBuffer();
      }

      virtual void TearDown() {
         CleanupSharedBuffer();
      }

      void SetupSharedBuffer() {
         mode_t mode(0644);
         int oflag = O_RDWR | O_CREAT;
         mShrMemFd = shm_open(mSharedMemName.c_str(), oflag, mode);
         if (mShrMemFd < 0) {
            std::cerr << "Unable to open file descriptor for shared memory: " << mSharedMemName << std::endl;
         }

         if (ftruncate(mShrMemFd, mNumBufs * mBufSize) != 0) {
            std::cerr << "Unable to truncate shared memory, " << mSharedMemName << ", to size:"
               << mNumBufs * mBufSize << std::endl;
         }

         mShrMemPtr = mmap(NULL, // let system pick where to put segment
               mNumBufs * mBufSize, // Number of bytes to allocate
               PROT_READ | PROT_WRITE, // access protections
               MAP_SHARED, // mapping visible to other processes
               mShrMemFd,
               0); // offset: start at 1st byte
         if ((void*) -1 == mShrMemPtr) {
            std::cerr << "Unable to access shared memory: " << mSharedMemName << std::endl;
         }
      }

      void CleanupSharedBuffer() {
         munmap(mShrMemPtr, mNumBufs * mBufSize);
         shm_unlink(mSharedMemName.c_str());
      }

      void AddToSharedBuffer(int bufCount, std::string& bufContent) {
         // For these tests we don't care about size being encoded into the shared buffer. We only care that the correct
         // buffer is setup so we can test if the SharedBufferReader properly retrieves it.
         char* bufPtr = reinterpret_cast<char *>(mShrMemPtr);
         bufPtr = bufPtr + ((bufCount % mNumBufs) * mBufSize);
         memcpy(bufPtr, bufContent.c_str(), bufContent.length());
         memset(bufPtr + bufContent.length(), '\0', 1); // Add null termination
      }
   
      // Protected member data
      std::string mSharedMemName;
      int mNumBufs;
      int mBufSize;

   private:
      int mShrMemFd;
      void* mShrMemPtr;
};
