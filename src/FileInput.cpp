#include "FileInput.h"
#include <iostream>

// NOTE: sizeof(unsigned long) is subtracted from bufSize to get the max sentence size. This is because the first bytes
// in a buffer are reserved for the size of a sentence sent as an unsigned long value.
FileInput::FileInput(std::string inputFile, int bufSize) :
   mFile(inputFile),
   mMaxS(bufSize - sizeof(unsigned long))
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

// bufPtr points to a buffer of size mMaxS or bigger.
unsigned long FileInput::GetNextSentence(char* bufPtr) {
   unsigned long len(0);
   bool sentenceEnd(false);
   while (mFile.is_open() && ! mFile.eof() && ! sentenceEnd) {
      char c = mFile.get();
      if (c == '\n') {
         bufPtr[len] = ' '; // replace CR with space
      } else {
         bufPtr[len] = c; // char is part of the sentence
      }
      len++;
      if (c == EOF || c == '.' || c == '?' || c == '!') {
         sentenceEnd = true;
      }
      if (len > mMaxS) {
         // Drop sentences greater than mMaxS in length. Move on to next sentence.
         len = 0;
         ReadToSentenceEnd();
      }
   }
   if (sentenceEnd) {
      ReadWhiteSpace();
   }
   bufPtr[len] = '\0'; // null terminate string, but don't count null termination character
   if (len > 0) {
      std::cout << std::endl << "\"" << bufPtr << "\"" << std::endl;
   }
   return len;
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
