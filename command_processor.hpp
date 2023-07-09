#include <fstream>
#include <string>

#include "logger.hpp"
// #include "world_systems/region.hpp"
// #include "world.hpp"
#include "database/database.hpp"

using world = Region;

class CommandProcessor {
 private:
  // Instantiate class attributes
  Database database;
  std::string databaseFile;
  std::string scriptFile;
  Logger logger;
  world worldBorder;
  std::fstream scriptStream;
  int commandNumber;
  char scriptDelimeter = '\t';

  void initLog();

  void closeLog();

  static std::string getDatetime();

  void import(std::string filename);

 public:
  CommandProcessor(std::string db, std::string script, std::string log);

  void run();
};
