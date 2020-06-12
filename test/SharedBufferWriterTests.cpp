#include "MockSharedBufferWriter.h"
#include "SharedBufferWriterTests.h"

TEST_F(SharedBufferWriterTests, SharedBufferWriterConstructorTest) {
   SharedBufferWriter mySharedBufferWriter(10, 1024, "mySharedMem");
}

TEST_F(SharedBufferWriterTests, SentenceFitsTest) {
   const int kBufSize(64);
   MockSharedBufferWriter mySharedBufferWriter(10, kBufSize, "mySharedMem");

   EXPECT_TRUE(mySharedBufferWriter.SentenceFits(kBufSize - sizeof(unsigned long)));
   EXPECT_FALSE(mySharedBufferWriter.SentenceFits((kBufSize - sizeof(unsigned long)) + 1));
}

TEST_F(SharedBufferWriterTests, CopyToBufTest) {
   const int kBufSize(1024);
   MockSharedBufferWriter mySharedBufferWriter(10, kBufSize, "mySharedMem");

   std::string sentence1{"This is a test."};
   
   char* buf1Ptr = mySharedBufferWriter.CopyToBuf(sentence1);

   unsigned long size1 = *(reinterpret_cast<unsigned long *>(buf1Ptr));
   EXPECT_EQ(sentence1.length(), size1);

   buf1Ptr += sizeof(unsigned long);
   std::string bufS1(buf1Ptr, size1);
   EXPECT_EQ(sentence1, bufS1);

   std::string sentence2{"This is a second test."};
   
   char* buf2Ptr = mySharedBufferWriter.CopyToBuf(sentence2);

   unsigned long size2 = *(reinterpret_cast<unsigned long *>(buf2Ptr));
   EXPECT_EQ(sentence2.length(), size2);

   buf2Ptr += sizeof(unsigned long);
   std::string bufS2(buf2Ptr, size2);
   EXPECT_EQ(sentence2, bufS2);

   EXPECT_EQ((buf1Ptr + sizeof(unsigned long) + sentence1.length()), buf2Ptr);
}

TEST_F(SharedBufferWriterTests, WriteBufTest) {
   const int kBufSize(1024);
   MockSharedBufferWriter mySharedBufferWriter(10, kBufSize, "mySharedMem");

   std::string sentence1{"This is a test."};
   
   char* buf1Ptr = mySharedBufferWriter.CopyToBuf(sentence1);

   char* shrMemBuf1Ptr;
   EXPECT_TRUE(mySharedBufferWriter.WriteBuf(shrMemBuf1Ptr));
   EXPECT_NE(buf1Ptr, shrMemBuf1Ptr);

   unsigned long size1 = *(reinterpret_cast<unsigned long *>(shrMemBuf1Ptr));
   EXPECT_EQ(sentence1.length(), size1);

   shrMemBuf1Ptr += sizeof(unsigned long);
   std::string bufS1(shrMemBuf1Ptr, size1);
   EXPECT_EQ(sentence1, bufS1);

   std::string sentence2{"This is a second test."};
   
   char* buf2Ptr = mySharedBufferWriter.CopyToBuf(sentence2);

   char* shrMemBuf2Ptr;
   EXPECT_TRUE(mySharedBufferWriter.WriteBuf(shrMemBuf2Ptr));
   EXPECT_NE(buf2Ptr, shrMemBuf2Ptr);

   unsigned long size2 = *(reinterpret_cast<unsigned long *>(shrMemBuf2Ptr));
   EXPECT_EQ(sentence2.length(), size2);

   shrMemBuf2Ptr += sizeof(unsigned long);
   std::string bufS2(shrMemBuf2Ptr, size2);
   EXPECT_EQ(sentence2, bufS2);

   EXPECT_NE(shrMemBuf1Ptr, shrMemBuf2Ptr);

   std::string sentence{"This is a test sentence."};
   
   for (int i = 0; i < 100; i++) {
      sentence.push_back('x'); // Increase sentence size by 1
      char* bufPtr = mySharedBufferWriter.CopyToBuf(sentence);

      char* shrMemBufPtr;
      EXPECT_TRUE(mySharedBufferWriter.WriteBuf(shrMemBufPtr));
      EXPECT_NE(bufPtr, shrMemBufPtr);

      unsigned long size1 = *(reinterpret_cast<unsigned long *>(shrMemBufPtr));
      EXPECT_EQ(sentence.length(), size1);

      shrMemBufPtr += sizeof(unsigned long);
      std::string bufS(shrMemBufPtr, size1);
      EXPECT_EQ(sentence, bufS);
   }

}

TEST_F(SharedBufferWriterTests, AddSentenceToBufTest) {
   // One sentence fits exactly in the buffer
   SharedBufferWriter mySharedBufferWriter1(10, 32, "mySharedMem");
   std::string sentence1{"This is a test sentence."}; // len = 24; add 8 for size to get 32 bytes in buffer.

   // First time buffer is not written to shared memory
   EXPECT_FALSE(mySharedBufferWriter1.AddSentenceToBuffer(sentence1));

   for (int i = 0; i < 100; i++) {
      // All subsequent adds force the buffer to be written to shared memory
      EXPECT_TRUE(mySharedBufferWriter1.AddSentenceToBuffer(sentence1));
   }

   // Two sentences fit, third sentence causes the buffer to be written to shared memory.
   SharedBufferWriter mySharedBufferWriter2(10, 128, "mySharedMem");

   // sentence len = 40; so 48 * 2 = 96, 48 * 3 = 144; 144 > 128
   std::string sentence2{"This is a test sentence to fill the buf."};
   EXPECT_FALSE(mySharedBufferWriter2.AddSentenceToBuffer(sentence2));
   EXPECT_FALSE(mySharedBufferWriter2.AddSentenceToBuffer(sentence2));
   EXPECT_TRUE(mySharedBufferWriter2.AddSentenceToBuffer(sentence2));
}

