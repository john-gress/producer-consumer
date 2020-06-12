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

   std::string sBuf(BufSize, '\0');
   myFileInput.GetNextSentence(sBuf);

   EXPECT_EQ(testFileContent.length(), sBuf.length());
   EXPECT_EQ(testFileContent, sBuf);

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

   std::string sBuf(BufSize, '\0');

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(sentence1.length(), sBuf.length());
   EXPECT_EQ(sentence1, sBuf);

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(sentence2.length(), sBuf.length());
   EXPECT_EQ(sentence2, sBuf);

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(sentence3.length(), sBuf.length());
   EXPECT_EQ(sentence3, sBuf);

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(0, sBuf.length());

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

   std::string sBuf(BufSize, '\0');

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(sentence1.length(), sBuf.length());
   EXPECT_EQ(sentence1, sBuf);

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(0, sBuf.length());

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

   std::string sBuf(BufSize, '\0');

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(sentence1.length(), sBuf.length());
   EXPECT_EQ(sentence1, sBuf);

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(sentence2.length(), sBuf.length());
   EXPECT_EQ(sentence2, sBuf);

   // sentence # 3 is dropped

   myFileInput.GetNextSentence(sBuf);
   EXPECT_EQ(0, sBuf.length());

   RemoveTestFile(testFile);
}
