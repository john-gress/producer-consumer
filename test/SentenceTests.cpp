#include "SentenceTests.h"
#include "MockSentence.h"

TEST_F(SentenceTests, ConstructorDestructorTest) {
   std::string match{"a fine day"};
   int bufSize(1024);
   Sentence mySentence(match, bufSize);
}

TEST_F(SentenceTests, ProcessBufUnsignedLongTest) {
   std::string match{"a fine day"};
   const int kBufSize(1024);
   MockSentence mySentence1(match, kBufSize);
   std::string bufSentence{"This is a fine day to have a party."}; // 35 chars long

   char buf[kBufSize];
   CreateBuf(buf, bufSentence);
   mySentence1.SetBufPtr(buf);
   unsigned long len;
   EXPECT_TRUE(mySentence1.GetLength(len));
   EXPECT_EQ(bufSentence.size(), len);

   MockSentence mySentence2(match, 43); // Sentence and unsigned long fit exactly
   mySentence2.SetBufPtr(buf);
   EXPECT_TRUE(mySentence2.GetLength(len));
   EXPECT_EQ(bufSentence.size(), len);

   MockSentence mySentence3(match, 42); // Sentence and unsigned long are one char short
   mySentence3.SetBufPtr(buf);
   EXPECT_FALSE(mySentence3.GetLength(len));
   EXPECT_EQ(0, len);

}

TEST_F(SentenceTests, ProcessBufGoodPunctuationTest) {
   std::string match{"a fine day"};
   const int kBufSize(1024);
   MockSentence mySentence(match, kBufSize);
   std::string bufSentence{"This is a fine day to have a party."};

   char buf[kBufSize];
   CreateBuf(buf, bufSentence);
   mySentence.SetBufPtr(buf);
   unsigned long len;
   EXPECT_TRUE(mySentence.GetLength(len));

   mySentence.AdvanceOffset(sizeof(unsigned long));
   EXPECT_TRUE(mySentence.GoodPunctuation(len));
}

TEST_F(SentenceTests, ProcessBufValidCharsTest) {
   std::string match{"a fine day"};
   const int kBufSize(1024);
   MockSentence mySentence(match, kBufSize);
   std::string bufSentence{"This is a fine day to have a party."};

   char buf[kBufSize];
   CreateBuf(buf, bufSentence);
   mySentence.SetBufPtr(buf);
   unsigned long len;
   EXPECT_TRUE(mySentence.GetLength(len));

   mySentence.AdvanceOffset(sizeof(unsigned long));
   EXPECT_TRUE(mySentence.GoodPunctuation(len));

   EXPECT_TRUE(mySentence.ValidChars(len));

   buf[sizeof(unsigned long) + len - 1] = 127;
   EXPECT_FALSE(mySentence.ValidChars(len));

   buf[sizeof(unsigned long) + 10] = 31;
   EXPECT_FALSE(mySentence.ValidChars(len));

   buf[sizeof(unsigned long)] = 255;
   EXPECT_FALSE(mySentence.ValidChars(len));
}

TEST_F(SentenceTests, ProcessBufStringMatchTest) {
   std::string match{"a fine day"};
   const int kBufSize(1024);
   MockSentence mySentence(match, kBufSize);
   std::string bufSentence{"This is a fine day to have a party."};

   char buf[kBufSize];
   CreateBuf(buf, bufSentence);
   mySentence.SetBufPtr(buf);
   unsigned long len;
   EXPECT_TRUE(mySentence.GetLength(len));

   mySentence.AdvanceOffset(sizeof(unsigned long));
   EXPECT_TRUE(mySentence.GoodPunctuation(len));

   EXPECT_TRUE(mySentence.StringMatch(len));
}

TEST_F(SentenceTests, ProcessBufTest) {
   // One sentence, One match
   std::string match{"a fine day"};
   const int kBufSize(1024);
   Sentence mySentence(match, kBufSize);
   std::string bufSentence{"This is a fine day to have a party."};

   char buf[kBufSize];
   CorruptBuf(buf, 0, kBufSize); // Fill buf with random data
   CreateBuf(buf, bufSentence);

   EXPECT_EQ(1, mySentence.ProcessBuf(buf));

   // Two sentences, two matches
   CorruptBuf(buf, 0, kBufSize); // Fill buf with random data
   CreateBufMultiSentence(buf, {
         {"This is a fine day to remember."},
         {"Did anything happen on a fine day last week?"}
      });

   EXPECT_EQ(2, mySentence.ProcessBuf(buf));

   // Two sentences, one match
   CorruptBuf(buf, 0, kBufSize); // Fill buf with random data
   CreateBufMultiSentence(buf, {
         {"This is a fine day to remember."},
         {"Did anything happen on a day last week?"}
      });

   EXPECT_EQ(1, mySentence.ProcessBuf(buf));

   // Three sentences, match first one, corrupt second one, match third one (never checked).
   CorruptBuf(buf, 0, kBufSize); // Fill buf with random data
   CreateBufMultiSentence(buf, {
         {"This is a fine day for a picnic!"},
         {"Did anything happen last week?"},
         {"I think this is a fine day."}
      });
   CorruptBuf(buf, 43, 58);

   EXPECT_EQ(1, mySentence.ProcessBuf(buf));
}
