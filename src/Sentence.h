#pragma once

#include <string>

class Sentence {
   public:
      Sentence(std::string match, int bufSize);
      ~Sentence();

      int ProcessBuf(char* buf);

      unsigned int GetMatchedCount() {
         return mMatchedCount;
      }
      unsigned int GetSentenceCount() {
         return mSentenceCount;
      }
      unsigned int GetCorruptCount() {
         return mCorruptCount;
      }

      unsigned int GetBadSizeCount() {
         return mBadSizeCount;
      }

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
      unsigned int mMatchedCount;
      unsigned int mSentenceCount;
      unsigned int mCorruptCount;
      unsigned int mBadSizeCount;
};
