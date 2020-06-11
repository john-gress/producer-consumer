#include "SharedBufferTests.h"
#include "SharedBuffer.h"

TEST_F(SharedBufferTests, LoadSharedBufferTest)
{
   SharedBuffer mySharedBuffer(10, 1024, "mySharedMem", true);
}
