#pragma once

#include <string>
#include <fstream>

class FileInput {
   public:
      FileInput(std::string inputFile, int bufSize);
      ~FileInput();

      bool EndOfFile();
      void GetNextSentence(std::string& sBuf);

   private:
      void ReadToSentenceEnd();
      void ReadWhiteSpace();

      std::ifstream mFile;
      int mMaxS; // Max sentence length. Sentences read from mFile longer than mMaxS are dropped.
};
