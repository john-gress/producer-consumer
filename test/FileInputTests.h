#pragma once

#include "gtest/gtest.h"
#include <fstream>
#include <cstdio>

class FileInputTests : public ::testing::Test
{
   public:
      FileInputTests(){};

   protected:
      virtual void SetUp() {}
      virtual void TearDown() {}

      void CreateTestFile(std::string fileName, std::string fileContents) {
         std::ofstream testFile;
         testFile.open(fileName);
         testFile << fileContents;
         testFile.close();
      }

      void RemoveTestFile(std::string fileName) {
         remove(fileName.c_str());
      }
};
