#pragma once

#include "gtest/gtest.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

class SentenceTests : public ::testing::Test
{
   public:
      SentenceTests(){
         srand(time(nullptr));
      };

   protected:
      virtual void SetUp() {}
      virtual void TearDown() {}
   
      unsigned long CreateBuf(char* buf, std::string bufContent) {
         unsigned long size = static_cast<unsigned long>(bufContent.size());
         *(reinterpret_cast<unsigned long *>(buf)) = size;
         int startOffset = sizeof(unsigned long);
         int i(startOffset);
         for (int j = 0; j < size; j++) {
            buf[i++] = bufContent[j];
         }
         //std::string bufS(buf + startOffset, size);
         //std::cout << "CreateBuf: " << *(reinterpret_cast<unsigned long *>(buf)) << ", " << bufS << std::endl;
         return size;
      }

      void CreateBufMultiSentence(char* buf, std::vector<std::string> bufContent) {
         for (int i = 0; i < bufContent.size(); i++) {
            unsigned long size = CreateBuf(buf, bufContent[i]);
            buf += (size + sizeof(unsigned long));
         }
      }

      void CorruptBuf(char* buf, int startOffset, int endOffset) {
         for (int i = startOffset; i < endOffset; i++) {
            char c = static_cast<char>(rand() % 256);
            if (c < 127) {
               c += 126;
            }
            *(buf+i) = c;
         }
      }

};
