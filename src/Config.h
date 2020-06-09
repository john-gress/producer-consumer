#include <map>
#include <string>

class Config {
   public:
      Config(std::string confFile);
      ~Config();

      int GetNumSharedBufs();
      int GetSizeSharedBuf();
      std::string GetSharedBufName();
      std::string GetBufferSemaName();
      std::string GetFinishedSemaName();
      std::string GetSentenceFilename();

   private:
      void LoadConfig(std::string& confFile);

      std::map<std::string,std::string> mConfig;
};
