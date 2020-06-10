#pragma once

#include "gtest/gtest.h"

class pcTests : public ::testing::Test
{
   public:
      pcTests(){};

   protected:
      virtual void SetUp() {}
      virtual void TearDown() {}
};
