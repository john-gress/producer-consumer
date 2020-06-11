#include "SharedBufferTests.h"
#include "SharedBuffer.h"

TEST_F(SharedBufferTests, SharedBufferConstructorTest)
{
   SharedBuffer mySharedBuffer(10, 1024, "mySharedMem", true);
}
