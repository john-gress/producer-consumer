#include <string>
#include <fstream>

class FileInput {
   public:
      FileInput(std::string inputFile, int bufSize);
      ~FileInput();

      unsigned long GetNextSentence(char* bufPtr);

   private:
      void ReadToSentenceEnd();
      void ReadWhiteSpace();

      std::ifstream mFile;
      int mMaxS; // Max sentence length. Sentences read from mFile longer than mMaxS are dropped.
};
