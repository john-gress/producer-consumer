#include "FileInputTests.h"
#include "FileInput.h"
#include <cstring>

TEST_F(FileInputTests, ConstructorDestructorTest) {
   FileInput myFileInput("/usr/share/doc/cmake-2.8.12.2/Copyright.txt", 1024);
}

TEST_F(FileInputTests, GetOneSentenceTest) {
   // One simple sentence
   std::string testFile{"/tmp/testfile"};
   std::string testFileContent{"This is a simple sentence."};
   const int BufSize(1024);

   CreateTestFile(testFile, testFileContent);

   FileInput myFileInput(testFile, BufSize);

   unsigned long len;
   char buf[BufSize];
   std::memset(buf, 0, BufSize);
   len = myFileInput.GetNextSentence(buf);

   EXPECT_EQ(testFileContent.size(), len);
   EXPECT_EQ(testFileContent, buf);

   RemoveTestFile(testFile);
}

TEST_F(FileInputTests, SentencePunctuationTest) {
   // Sentences with different punctuation marks
   std::string testFile{"/tmp/testfile"};
   std::string sentence1{"This is sentence number one."};
   std::string sentence2{"Is this sentence # two?"};
   std::string sentence3{"Fantastic, this is sentence number three!"};
   std::string whitespace{"  "};
   const int BufSize(1024);

   CreateTestFile(testFile, {sentence1 + whitespace + sentence2 + whitespace + sentence3 + whitespace});

   FileInput myFileInput(testFile, BufSize);

   unsigned long len;
   char buf[BufSize];
   std::memset(buf, 0, BufSize);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(sentence1.size(), len);
   EXPECT_EQ(sentence1, buf);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(sentence2.size(), len);
   EXPECT_EQ(sentence2, buf);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(sentence3.size(), len);
   EXPECT_EQ(sentence3, buf);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(0, len);
   EXPECT_EQ('\0', buf[0]);

   RemoveTestFile(testFile);
}

TEST_F(FileInputTests, BreakSentenceAcrossCRTest) {
   // Sentences with different punctuation marks
   std::string testFile{"/tmp/testfile"};
   std::string sentence1{"This is a sentence split across lines."};

   std::string sentenceP1{"This is a sentence"};
   std::string sentenceP2{"split across lines."};
   std::string newline{"\n"};
   const int BufSize(1024);

   CreateTestFile(testFile, {sentenceP1 + newline + sentenceP2 + newline + newline + newline});

   FileInput myFileInput(testFile, BufSize);

   unsigned long len;
   char buf[BufSize];
   std::memset(buf, 0, BufSize);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(sentence1.size(), len);
   EXPECT_EQ(sentence1, buf);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(0, len);
   EXPECT_EQ('\0', buf[0]);

   RemoveTestFile(testFile);
}

TEST_F(FileInputTests, SentenceOverMaxTest) {

   std::string testFile{"/tmp/testfile"};
   std::string sentence1{"This sentence is under the maximum."};
   std::string sentence2{"This sentence is at the maximum of 38."};
   std::string sentence3{"This sentence is 1 over the maximum 38."};
   std::string whitespace{"  "};
   const int BufSize(38+sizeof(unsigned long));

   CreateTestFile(testFile, {sentence1 + whitespace + sentence2 + whitespace + sentence3 + whitespace});

   FileInput myFileInput(testFile, BufSize);

   unsigned long len;
   char buf[BufSize];
   std::memset(buf, 0, BufSize);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(sentence1.size(), len);
   EXPECT_EQ(sentence1, buf);

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(sentence2.size(), len);
   EXPECT_EQ(sentence2, buf);

   // sentence # 3 is dropped

   len = myFileInput.GetNextSentence(buf);
   EXPECT_EQ(0, len);
   EXPECT_EQ('\0', buf[0]);

   RemoveTestFile(testFile);
}
