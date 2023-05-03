#include <fstream>
#include <string>

#include "logger.hpp"

class CommandProcesor {
 private:
  // Instantiate class attributes
  std::string databaseFile;
  std::string scriptFile;
  Logger logger;
  std::fstream scriptStream;
  int commandNumber;

  void initLog();

  void closeLog();

  static std::string getDatetime();

 public:
  CommandProcesor(std::string db, std::string script, std::string log);

  void run();
};
