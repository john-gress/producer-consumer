#pragma once

#include <string>

class Sentence {
   public:
      Sentence(std::string match, int bufSize);
      ~Sentence();

      int ProcessBuf(char* buf);

   protected:
      virtual void SetBufPtr(char* buf);
      virtual void AdvanceOffset(int advance);
      virtual bool GetLength(unsigned long& len);
      virtual bool GoodPunctuation(unsigned long len);
      virtual bool ValidChars(unsigned long len);
      virtual bool StringMatch(unsigned long len);

   private:
      std::string mMatch;
      int mBufSize;
      int mOffset;
      char* mBufPtr;
      static const int kLenSize = sizeof(unsigned long);
};
