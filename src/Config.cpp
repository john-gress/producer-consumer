#include "Config.h"
#include <fstream>
#include <iostream>

Config::Config(std::string confFile) {
   LoadConfig(confFile);
}

Config::~Config() {
}

void Config::LoadConfig(std::string& confFile) {
   std::ifstream fileInput(confFile);

   while (fileInput) {
      std::string key;
      std::string value;
      std::getline(fileInput, key, ':');
      std::getline(fileInput, value, '\n');
      if (key.size() > 0 && value.size() > 0) {
         std::cout << "Config key: \"" << key << "\", value: \"" << value << "\"" << std::endl;
         mConfig[key] = value;
      }
   }
   fileInput.close();
}

int Config::GetNumSharedBufs() {
   int numbufs(100);
   auto it = mConfig.find("NumSharedBufs");
   if (it != mConfig.end()) {
      numbufs = std::stoi(it->second);
      return numbufs;
   }

   std::cerr << "NumSharedBufs not found in config. Using default value: " << numbufs << std::endl;
   return numbufs;
}

int Config::GetSizeSharedBuf() {
   int bufsize(1024);
   auto it = mConfig.find("SizeSharedBuf");
   if (it != mConfig.end()) {
      bufsize = std::stoi(it->second);
      return bufsize;
   }

   std::cerr << "SizeSharedBuf not found in config. Using default value: " << bufsize << std::endl;
   return bufsize;
}

std::string Config::GetSharedBufName() {
   auto it = mConfig.find("SharedMemName");
   if (it != mConfig.end()) {
      return it->second;
   }

   std::cerr << "SharedMemName not found in config. Using default name: \"sharedMemSemaphore\"" << std::endl;
   return {"sharedMemSemaphore"};
}

std::string Config::GetBufferSemaName() {
   auto it = mConfig.find("BufferSemaName");
   if (it != mConfig.end()) {
      return it->second;
   }

   std::cerr << "BufferSemaName not found in config. Using default name: \"bufferSemaphore\"" << std::endl;
   return {"bufferSemaphore"};
}

std::string Config::GetFinishedSemaName() {
   auto it = mConfig.find("FinishedSemaName");
   if (it != mConfig.end()) {
      return it->second;
   }

   std::cerr << "FinishedSemaName not found in config. Using default name: \"finishSemaphore\"" << std::endl;
   return {"finishSemaphore"};
}

std::string Config::GetSentenceFilename() {
   auto it = mConfig.find("SentenceFilename");
   if (it != mConfig.end()) {
      return it->second;
   }

   std::cerr << "SentenceFilename not found in config. Using default name: \"./sentences\"" << std::endl;
   return {"./sentences"};
}