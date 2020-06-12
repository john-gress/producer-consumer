#include "FileInput.h"
#include <iostream>

// NOTE: sizeof(unsigned long) is subtracted from bufSize to get the max sentence size. This is because the first bytes
// in a buffer are reserved for the size of a sentence sent as an unsigned long value.
FileInput::FileInput(std::string inputFile, int bufSize) :
   mFile(inputFile),
   mMaxS(bufSize - sizeof(unsigned long)),
   mSentenceCount(0),
   mOversizeCount(0)
{
   if (mFile.is_open()) {
      std::cout << inputFile << " successfully opened for reading." << std::endl;
   } else {
      std::cerr << "Failed to open " << inputFile << " for reading." << std::endl;
   }
}

FileInput::~FileInput() {
   if (mFile.is_open()) {
      mFile.close();
   }
}

bool FileInput::EndOfFile() {
   return !mFile.is_open() || mFile.eof();
}

// sBuf points to a string buffer of size mMaxS or bigger.
void FileInput::GetNextSentence(std::string& sBuf) {
   sBuf.clear();
   bool sentenceEnd(false);
   char c;
   while (mFile.is_open() && ! mFile.eof() && ! sentenceEnd) {
      if (mFile.get(c)) {
         if (c == '\n') {
            sBuf.push_back(' '); // replace CR with space
         } else {
            sBuf.push_back(c); // char is part of the sentence
         }
         if (c == EOF || c == '.' || c == '?' || c == '!') {
            sentenceEnd = true;
         }
         if (sBuf.length() > mMaxS) {
            // Drop sentences greater than mMaxS in length. Move on to next sentence.
            mOversizeCount++;
            sBuf.clear();
            ReadToSentenceEnd();
         }
      }
   }
   if (sentenceEnd) {
      ReadWhiteSpace();
   }
   if (sBuf.length() > 0) {
      mSentenceCount++;
      std::cout << std::endl << "\"" << sBuf << "\"" << std::endl;
   }
}

void FileInput::ReadToSentenceEnd() {
   bool sentenceEnd(false);
   while (mFile.is_open() && ! mFile.eof() && ! sentenceEnd) {
      char c = mFile.get();
      if (c == EOF || c == '.' || c == '?' || c == '!') {
         sentenceEnd = true;
      }
   }
}

void FileInput::ReadWhiteSpace() {
   bool sentenceBegin(false);
   while (mFile.is_open() && ! mFile.eof() && ! sentenceBegin) {
      char c = mFile.get();
      if (c != EOF && ! isspace(c)) {
         sentenceBegin = true;
         mFile.putback(c);
      }
   }
}
