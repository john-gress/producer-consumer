#include "pcTests.h"
#include "pc.h"

TEST_F(pcTests, simple_test)
{
   pc myPc;
   EXPECT_TRUE(myPc.pcTest());
}
