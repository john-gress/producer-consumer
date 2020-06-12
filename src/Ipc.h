#pragma once

#include <string>
#include <semaphore.h>

class Ipc {
   public:
      Ipc(int numBufs, std::string bufSemName, std::string finishedSemName, bool create);
      ~Ipc();

      void IncrementBuffer();
      void DecrementBuffer();
      bool QueueFull();
      bool BufsAvailable();
      void SignalFinished();
      bool IsProducerFinished();
      bool IsConsumerFinished();

   protected:
      void SetupIpc(bool create);

      virtual int GetBufferValue();
      virtual int GetFinishedValue();

   private:
      int mNumBufs;
      std::string mBufferSemName;
      std::string mFinishedSemName;
      sem_t* mBufferSemPtr;
      sem_t* mFinishedSemPtr;
};
