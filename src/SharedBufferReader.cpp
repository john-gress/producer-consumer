#include "SharedBufferReader.h"
#include <cstring>

SharedBufferReader::SharedBufferReader(int numBufs, int sizeBuf, std::string bufName, bool create) :
   SharedBuffer(numBufs, sizeBuf, bufName, create)
{
}

SharedBufferReader::~SharedBufferReader() {
}

void SharedBufferReader::GetNextBuffer(char* bufPtr) {
   char* shrBufPtr = GetSharedMemBufPtr();
   memcpy(bufPtr, shrBufPtr, mBufSize);
   mBufCount++;
}
