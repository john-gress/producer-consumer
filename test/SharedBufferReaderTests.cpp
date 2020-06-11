#include "SharedBufferReader.h"
#include "SharedBufferReaderTests.h"

TEST_F(SharedBufferReaderTests, SharedBufferReaderConstructorTest) {
   SharedBufferReader mySharedBufferReader(mNumBufs, mBufSize, mSharedMemName.c_str(), false);
}

TEST_F(SharedBufferReaderTests, GetNextBufferTest) {
   SharedBufferReader mySharedBufferReader(mNumBufs, mBufSize, mSharedMemName.c_str(), false);
   std::string sBuf{"This is the sentence in the buffer for buffer # "};

   for (int bufCount = 0; bufCount < 100; bufCount++) {
      // Create a unique sentence in each buffer.
      std::string sBufContent{sBuf + std::to_string(bufCount)};
      AddToSharedBuffer(bufCount, sBufContent);

      char buf[mBufSize];
      mySharedBufferReader.GetNextBuffer(buf);
      std::string bufContent(buf);
      EXPECT_EQ(sBufContent, bufContent);
   }
}
