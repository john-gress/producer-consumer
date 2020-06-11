#pragma once

#include "SharedBuffer.h"
#include <string>

class SharedBufferReader : public SharedBuffer {
   public:
      SharedBufferReader(int numBufs, int sizeBuf, std::string bufName, bool create);
      ~SharedBufferReader();

      void GetNextBuffer(char* bufPtr);
};
