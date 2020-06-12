#include "SharedBufferWriter.h"

class MockSharedBufferWriter : public SharedBufferWriter {
   public:
      MockSharedBufferWriter(int numBufs, int sizeBuf, std::string bufName, bool create) :
         SharedBufferWriter(numBufs, sizeBuf, bufName, create)
      {
      }

      ~MockSharedBufferWriter() {
      }

      bool WriteBuf(char*& shrBufPtr) override {
         return SharedBufferWriter::WriteBuf(shrBufPtr);
      }

      bool SentenceFits(int len) override {
         return SharedBufferWriter::SentenceFits(len);
      }

      char* CopyToBuf(std::string& sentence) override {
         return SharedBufferWriter::CopyToBuf(sentence);
      }
};
