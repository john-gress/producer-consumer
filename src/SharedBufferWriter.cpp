#include "SharedBufferWriter.h"
#include <iostream>
#include <cstring>

SharedBufferWriter::SharedBufferWriter(int numBufs, int sizeBuf, std::string bufName, bool create) :
   SharedBuffer(numBufs, sizeBuf, bufName, create),
   mOffset(0),
   mWorkingBuf(nullptr)
{
   mWorkingBuf = new (std::nothrow) char[mBufSize];
   if (mWorkingBuf == nullptr) {
      std::cerr << "SharedBufferWriter: unable to allocate buffer: " << mBufSize << std::endl;
      exit(1);
   }
}

SharedBufferWriter::~SharedBufferWriter() {
   delete[] mWorkingBuf;
}

bool SharedBufferWriter::AddSentenceToBuffer(std::string& sentence) {
   bool BufferWrittenToSharedMem(false);

   if (SentenceFits(sentence.length())) {
      CopyToBuf(sentence);
   } else {
      char* shrBufPtr;
      WriteBuf(shrBufPtr);
      BufferWrittenToSharedMem = true;
      CopyToBuf(sentence);
   }
   return BufferWrittenToSharedMem;
}

bool SharedBufferWriter::SentenceFits(int len) {
   return (mBufSize - mOffset) >= (kLenSize + len);
}

char* SharedBufferWriter::CopyToBuf(std::string& sentence) {
   char * bufStartPtr = mWorkingBuf + mOffset;

   unsigned long size = static_cast<unsigned long>(sentence.length());
   *(reinterpret_cast<unsigned long *>(mWorkingBuf + mOffset)) = size;
   mOffset += kLenSize;

   sentence.copy((mWorkingBuf + mOffset), sentence.length(), 0);
   mOffset += sentence.length();
   return bufStartPtr;
}

void SharedBufferWriter::ZeroTrailingLen() {
   for (int i = 0; i < kLenSize && mOffset < mBufSize; i++) {
      *(mWorkingBuf + mOffset++) = 0;
   }
}

bool SharedBufferWriter::WriteBuf(char*& shrBufPtr) {
   bool bufWritten(false);
   shrBufPtr = GetSharedMemBufPtr();
   //printf("WriteBuf: %p\n", shrBufPtr);
   if (mOffset > 0) {
      ZeroTrailingLen();
      memcpy(shrBufPtr, mWorkingBuf, mOffset);
      mBufCount++;
      mOffset = 0;
      bufWritten = true;
   }

   return bufWritten;
}
