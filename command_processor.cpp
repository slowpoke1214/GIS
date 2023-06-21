#include "command_processor.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "command.hpp"
#include "gis_helpers.hpp"

void CommandProcessor::initLog() {
  std::stringstream msg;
  msg << "Course Project for COMP 8042" << std::endl
      << "Student Name: Tushya Iyer, Student Id: A01023434" << std::endl
      << "Student Name: Joey Kennedy, Student Id: A01030505" << std::endl
      << "Begin of GIS Program log:" << std::endl
      << "dbFile: " << databaseFile << std::endl
      << "script: " << scriptFile << std::endl
      << "log: " << logger.logFile << std::endl
      << "Start Time: " << getDatetime();

  logger.write(msg.str());
}

void CommandProcessor::closeLog() {
  std::stringstream msg;
  msg << "End time: " << getDatetime();
  logger.write(msg.str());
}

std::string CommandProcessor::getDatetime() {
  time_t now = time(0);
  tm* nowTm = localtime(&now);
  char buffer[30];

  strftime(buffer, 30, "%a %b %d %H:%M:%S %Z %Y", nowTm);
  return buffer;
}

CommandProcessor::CommandProcessor(std::string db, std::string script,
                                 std::string log) : database(Database(db)) {
  databaseFile = std::move(db);
  scriptFile = std::move(script);
  logger = Logger(log);
  commandNumber = 0;

  initLog();
}

void CommandProcessor::import(std::string filename) {
  std::ifstream file(filename);

  std::string line;
  std::getline(file, line);
  while (std::getline(file, line)) {
    database.insert(line);
  }
}

void CommandProcessor::run() {
  scriptStream.open(scriptFile, std::ios::in);
  std::string commandString;

  if (scriptStream) {
    while (std::getline(scriptStream, commandString)) {
      std::vector<std::string> commandTokens =
          helpers::splitString(commandString, scriptDelimeter);

      Command command = stringToCommand(commandTokens[0]);
      std::vector<std::string> args(commandTokens.begin() + 1,
                                    commandTokens.end());

      if (command == Command::comment) {
        logger.write(commandString);
        continue;
      } else {
        logger.write(commandString, commandNumber++);
        switch (command) {
          case Command::world:
            worldBorder = world(DMS(args[3]), DMS(args[2]), DMS(args[1]), DMS(args[0]));
            logger.write(worldBorder.repr());
            break;
          case Command::import:
          {
            import(args[0]);
            logger.writeSeparator();
            break;
          }
          case Command::debug:
          {
            if (args[0] == "world") {
              // TODO: Debug world
              break;
            } else if (args[0] == "pool") {
              bufferPool.str();
              break;
            } else if (args[0] == "quad") {
              // TODO: Debug Quad
              break;
            }
            // Unknown value
            break;
          }
          case Command::what_is:
          {
            std::vector<std::string> records = database.whatIs(args[0], args[1]);
            for (auto &&rec : records)
            {
              logger.write(rec);
            }
            logger.writeSeparator();
            break;
            }
            
          default: {
            logger.writeSeparator();
            continue;
            break;
          }
        }
      }
    }
  }
  closeLog();
}
