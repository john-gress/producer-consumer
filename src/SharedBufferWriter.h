#pragma once

#include "SharedBuffer.h"
#include <string>

class SharedBufferWriter : public SharedBuffer {
   public:
      SharedBufferWriter(int numBufs, int sizeBuf, std::string bufName, bool create);
      ~SharedBufferWriter();

      bool AddSentenceToBuffer(std::string& sentence);
      virtual bool WriteBuf(char*& shrBufPtr);
      
   protected:
      virtual bool SentenceFits(int len);
      virtual char* CopyToBuf(std::string& sentence);

   private:
      void ZeroTrailingLen();
      int mOffset;
      char* mWorkingBuf;
      static const int kLenSize = sizeof(unsigned long);
};
