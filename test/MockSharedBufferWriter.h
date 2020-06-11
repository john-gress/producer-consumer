#include "SharedBufferWriter.h"

class MockSharedBufferWriter : public SharedBufferWriter {
   public:
      MockSharedBufferWriter(int numBufs, int sizeBuf, std::string bufName, bool create) :
         SharedBufferWriter(numBufs, sizeBuf, bufName, create)
      {
      }

      ~MockSharedBufferWriter() {
      }

      char* WriteBuf() override {
         return SharedBufferWriter::WriteBuf();
      }

      bool SentenceFits(int len) override {
         return SharedBufferWriter::SentenceFits(len);
      }

      char* CopyToBuf(std::string& sentence) override {
         return SharedBufferWriter::CopyToBuf(sentence);
      }
};
