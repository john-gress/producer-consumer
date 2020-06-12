#include "Sentence.h"
#include <iostream>

Sentence::Sentence(std::string match, int bufSize) :
   mMatch(match),
   mBufSize(bufSize),
   mOffset(0),
   mBufPtr(nullptr),
   mMatchedCount(0),
   mSentenceCount(0),
   mCorruptCount(0),
   mBadSizeCount(0)
{
}

Sentence::~Sentence() {
}

int Sentence::ProcessBuf(char* buf) {
   int numMatches(0);
   mOffset = 0;
   SetBufPtr(buf);
   unsigned long len(0);

   do {
      if (GetLength(len)) {
         AdvanceOffset(kLenSize); // Advance past the unsigned long
         if (GoodPunctuation(len) && ValidChars(len)) {
            mSentenceCount++;
            if (StringMatch(len)) {
               numMatches++;
               mMatchedCount++;
            }
            AdvanceOffset(len); // Advance past the sentence
         } else {
            // Stop processing buffer. Corruption discovered.
            //std::string s((mBufPtr + mOffset), len);
            //std::cout << std::endl << std::endl << "Corrupt s: " << s << std::endl << std::endl;
            mCorruptCount++;
            len = 0;
         }
      }
   } while (len > 0);

   return numMatches;
}

void Sentence::SetBufPtr(char* buf) {
   mBufPtr = buf;
}

void Sentence::AdvanceOffset(int advance) {
   mOffset += advance;
}

bool Sentence::GetLength(unsigned long& len) {
   len = 0;
   unsigned long bufSpace = mBufSize - mOffset;
   if (bufSpace > kLenSize) {
      len = *(reinterpret_cast<unsigned long *>(mBufPtr + mOffset));
      bufSpace -= kLenSize;
      if (len > bufSpace) {
         len = 0;
         mBadSizeCount++;
      }
   }
   return len > 0;
}

bool Sentence::GoodPunctuation(unsigned long len) {
   switch (*(mBufPtr + mOffset + len - 1)) {
      case '.':
      case '!':
      case '?':
         return true;
         break;
      default:
         return false;
         break;
   }
}

bool Sentence::ValidChars(unsigned long len) {
   for (int i = 0; i < len; i++) {
      // Valid characters in ascii are between a space char and tilda char
      if (*(mBufPtr + mOffset + i) < ' ' || *(mBufPtr + mOffset + i) > '~') {
         return false;
      }
   }
   return true;
}

bool Sentence::StringMatch(unsigned long len) {
   std::string s((mBufPtr + mOffset), len);
   if (s.find(mMatch) != std::string::npos) {
      std::cout << "Match: " << s << std::endl;
      return true;
   }
   return false;
}
