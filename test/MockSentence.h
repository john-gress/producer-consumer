#include "Sentence.h"

class MockSentence : public Sentence {
   public:
      MockSentence(std::string inputFile, int bufSize) :
         Sentence(inputFile, bufSize)
      {
      }

      ~MockSentence() {
      }

      void SetBufPtr(char* buf) override {
         Sentence::SetBufPtr(buf);
      }

      void AdvanceOffset(int advance) override {
         Sentence::AdvanceOffset(advance);
      }

      bool GetLength(unsigned long& len) override {
         return Sentence::GetLength(len);
      }

      bool GoodPunctuation(unsigned long len) override {
         return Sentence::GoodPunctuation(len);
      }

      bool ValidChars(unsigned long len) override {
         return Sentence::ValidChars(len);
      }

      bool StringMatch(unsigned long len) override {
         return Sentence::StringMatch(len);
      }

};
