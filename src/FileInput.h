#pragma once

#include <string>
#include <fstream>

class FileInput {
   public:
      FileInput(std::string inputFile, int bufSize);
      ~FileInput();

      bool EndOfFile();
      void GetNextSentence(std::string& sBuf);

      unsigned long GetSentenceCount() {
         return mSentenceCount;
      }

      unsigned long GetOversizeCount() {
         return mOversizeCount;
      }

   private:
      void ReadToSentenceEnd();
      void ReadWhiteSpace();

      std::ifstream mFile;
      int mMaxS; // Max sentence length. Sentences read from mFile longer than mMaxS are dropped.

      unsigned long mSentenceCount;
      unsigned long mOversizeCount;
};
