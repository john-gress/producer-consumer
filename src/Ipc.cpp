#include "Ipc.h"
#include <iostream>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <cerrno>
#include <cstring>

Ipc::Ipc(int numBufs, std::string bufSemName, std::string finishedSemName, bool create) :
   mNumBufs(numBufs),
   mBufferSemName(bufSemName),
   mFinishedSemName(finishedSemName),
   mBufferSemPtr(nullptr),
   mFinishedSemPtr(nullptr)
{
   SetupIpc(create);
}

void Ipc::SetupIpc(bool create) {
   int oflag = O_EXCL;
   if (create) {
      oflag = O_CREAT | O_EXCL;
   }

   mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP;

   mBufferSemPtr = sem_open(mBufferSemName.c_str(), oflag, mode, 0);
   if (mBufferSemPtr == nullptr) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Failed to open semaphore for: " << mBufferSemName << ", " << errnoMsg << std::endl;
      exit(1);
   }

   mFinishedSemPtr = sem_open(mFinishedSemName.c_str(), oflag, mode, 0);
   if (mFinishedSemPtr == nullptr) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Failed to open semaphore for: " << mFinishedSemName << ", " << errnoMsg << std::endl;
      exit(1);
   }
}

Ipc::~Ipc() {
   if (sem_close(mBufferSemPtr) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Failed to close buffer semaphore: " << errnoMsg << std::endl;
   }
   
   if (sem_unlink(mBufferSemName.c_str()) != 0 && errno != ENOENT) {
      // ENOENT = No such file or directory. This is normal, as there can be a race condition for which process
      // deletes the file.
      std::string errnoMsg(strerror(errno));
      std::cerr << "Failed to unlink: " <<  mBufferSemName << ", " << errnoMsg << std::endl;
   }

   if (sem_close(mFinishedSemPtr) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Failed to close buffer semaphore: " << errnoMsg << std::endl;
   }
   
   if (sem_unlink(mFinishedSemName.c_str()) != 0 && errno != ENOENT) {
      // ENOENT = No such file or directory. This is normal, as there can be a race condition for which process
      // deletes the file.
      std::string errnoMsg(strerror(errno));
      std::cerr << "Failed to unlink: " <<  mFinishedSemName << ", " << errnoMsg << std::endl;
   }
}

int Ipc::GetBufferValue() {
   int bufValue(-1);
   if (sem_getvalue(mBufferSemPtr, &bufValue) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Buffer sem_getvalue failed: " << mBufferSemName << ", " << errnoMsg << std::endl;
   }
   return bufValue;
}

int Ipc::GetFinishedValue() {
   int finishedValue(-1);
   if (sem_getvalue(mFinishedSemPtr, &finishedValue) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Finished sem_getvalue failed: " << mFinishedSemName << ", " << errnoMsg << std::endl;
   }
   return finishedValue;
}

void Ipc::IncrementBuffer() {
   if (sem_post(mBufferSemPtr) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Buffer sem_post failed: " << mBufferSemName << ", " << errnoMsg << std::endl;
   }
}

void Ipc::DecrementBuffer() {
   if (sem_trywait(mBufferSemPtr) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Buffer sem_trywait failed: " << mBufferSemName << ", " << errnoMsg << std::endl;
   }
}

bool Ipc::QueueFull() {
   return GetBufferValue() >= mNumBufs;
}

bool Ipc::BufsAvailable() {
   return GetBufferValue() > 0;
}

void Ipc::SignalFinished() {
   if (sem_post(mFinishedSemPtr) != 0) {
      std::string errnoMsg(strerror(errno));
      std::cerr << "Finished sem_post failed: " << mFinishedSemName << ", " << errnoMsg << std::endl;
   }
}

bool Ipc::IsProducerFinished() {
   return GetFinishedValue() >= 1;
}

bool Ipc::IsConsumerFinished() {
   return GetFinishedValue() >= 2;
}
