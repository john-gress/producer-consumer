#include "ConfigTests.h"
#include "Config.h"

TEST_F(ConfigTests, LoadConfigTest)
{
   Config myConfig("../conf/pcConf");
   EXPECT_EQ(1024, myConfig.GetSizeSharedBuf());
   
   EXPECT_EQ("PcSharedMem", myConfig.GetSharedBufName());
   EXPECT_EQ("bufSem", myConfig.GetBufferSemaName());
   EXPECT_EQ("finishSem", myConfig.GetFinishedSemaName());
}
