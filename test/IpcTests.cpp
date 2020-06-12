#include "MockIpc.h"
#include "IpcTests.h"

TEST_F(IpcTests, IpcConstructorTest) {
   Ipc myIpc(10, "bufferSem", "finishedSem", true);
}

TEST_F(IpcTests, IncrementDecrementBufferTest) {
   MockIpc myIpc(10, "bufferSem", "finishedSem", true);

   for (int expectedBufferValue = 1; expectedBufferValue <= 10; expectedBufferValue++) {
      myIpc.IncrementBuffer();
      int bufValue = myIpc.GetBufferValue();
      EXPECT_EQ(expectedBufferValue, bufValue);
   }

   for (int expectedBufferValue = 10; expectedBufferValue > 0; expectedBufferValue--) {
      int bufValue = myIpc.GetBufferValue();
      EXPECT_EQ(expectedBufferValue, bufValue);
      myIpc.DecrementBuffer();
   }

   int bufValue = myIpc.GetBufferValue();
   EXPECT_EQ(0, bufValue);
}

TEST_F(IpcTests, QueueFullBufsAvailableTest) {
   const int kMaxBuf(10);
   MockIpc myIpc(kMaxBuf, "bufferSem", "finishedSem", true);

   for (int bufCount = 1; bufCount <= kMaxBuf; bufCount++) {
      myIpc.IncrementBuffer();
      if (bufCount < kMaxBuf) {
         EXPECT_FALSE(myIpc.QueueFull()) << "bufCount: " << bufCount;
      } else {
         EXPECT_TRUE(myIpc.QueueFull()) << "bufCount: " << bufCount;
      }
   }
   EXPECT_TRUE(myIpc.QueueFull());

   for (int bufCount = kMaxBuf; bufCount > 0; bufCount--) {
      EXPECT_TRUE(myIpc.BufsAvailable()) << "bufCount: " << bufCount;
      myIpc.DecrementBuffer();
   }
   EXPECT_FALSE(myIpc.BufsAvailable());
}

TEST_F(IpcTests, FinishedTest) {
   MockIpc myIpc(10, "bufferSem", "finishedSem", true);

   EXPECT_FALSE(myIpc.IsProducerFinished());
   EXPECT_FALSE(myIpc.IsConsumerFinished());

   myIpc.SignalFinished();  // Producer signals first
   EXPECT_TRUE(myIpc.IsProducerFinished());
   EXPECT_FALSE(myIpc.IsConsumerFinished());

   myIpc.SignalFinished();  // Consumer signals second
   EXPECT_TRUE(myIpc.IsProducerFinished());
   EXPECT_TRUE(myIpc.IsConsumerFinished());
}
