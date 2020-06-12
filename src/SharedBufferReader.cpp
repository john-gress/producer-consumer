#include "SharedBufferReader.h"
#include <cstring>

SharedBufferReader::SharedBufferReader(int numBufs, int sizeBuf, std::string bufName) :
   SharedBuffer(numBufs, sizeBuf, bufName, false) // create = false; Reader does not create shared memory
{
}

SharedBufferReader::~SharedBufferReader() {
}

void SharedBufferReader::GetNextBuffer(char* bufPtr) {
   char* shrBufPtr = GetSharedMemBufPtr();
   memcpy(bufPtr, shrBufPtr, mBufSize);
   mBufCount++;
}
