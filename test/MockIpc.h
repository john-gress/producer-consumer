#include "Ipc.h"

class MockIpc : public Ipc {
   public:
      MockIpc(int numBufs, std::string bufSemName, std::string finishedSemName, bool create) :
         Ipc(numBufs, bufSemName, finishedSemName, create)
      {
      }

      ~MockIpc() {
      }

      int GetBufferValue() override {
         return Ipc::GetBufferValue();
      }

      int GetFinishedValue() override {
         return Ipc::GetFinishedValue();
      }
};
