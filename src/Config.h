#pragma once

#include <map>
#include <string>

class Config {
   public:
      Config(std::string confFile);
      ~Config();

      int GetSizeSharedBuf();
      std::string GetSharedBufName();
      std::string GetBufferSemaName();
      std::string GetFinishedSemaName();

   private:
      void LoadConfig(std::string& confFile);

      std::map<std::string,std::string> mConfig;
};
