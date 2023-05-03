#include <sstream>
#include "command_processor.hpp"

void CommandProcesor::initLog() {
  std::stringstream msg;
  msg << "Course Project for COMP 8042" << std::endl
      << "Student Name: Tushya Iyer, Student Id: A01023434" << std::endl
      << "Student Name: Joey Kennedy, Student Id: <STUDENT ID>" << std::endl
      << "Begin of GIS Program log:" << std::endl
      << "dbFile: " << databaseFile << std::endl
      << "script: " << scriptFile << std::endl
      << "log: " << logger.logFile << std::endl
      << "Start Time: " << getDatetime();

  logger.write(msg.str());
}

void CommandProcesor::closeLog() {
  std::stringstream msg;
  msg << "End time: " << getDatetime();
  logger.write(msg.str());
}

std::string CommandProcesor::getDatetime() {
  time_t now = time(0);
  tm* nowTm = localtime(&now);
  char buffer[30];

  strftime(buffer, 30, "%a %b %d %H:%M:%S %Z %Y", nowTm);
  return buffer;
}

CommandProcesor::CommandProcesor(std::string db, std::string script,
                                 std::string log) {
  databaseFile = std::move(db);
  scriptFile = std::move(script);
  logger = Logger(log);
  commandNumber = 0;

  initLog();
}

void CommandProcesor::run() {
  // TODO:
  // Read Script file
  scriptStream.open(scriptFile, std::ios::in);
  std::string command;

  if (scriptStream) {
    while (std::getline(scriptStream, command)) {
      // cout << command;
      // cout << "\n";

      if (command[0] == ';') {
        logger.write(command);
      } else {
        logger.write(command, commandNumber++);

        // Command is not a comment, begin parsing for commands:

        // Check for quit
        if (command.substr(0, command.find(' ')) == "quit") {
          // Line is a quit command
        } else {
          // Line is an actual command
          if (command.substr(0, command.find(' ')) == "world") {
            // Line is a world command
            // TODO:
            // 1. Parse arguments
          }
        }
      }

      // TODO:
      // 1. Ignore comments (lines that start with ';'
      // 2. Determine Commands (world, import, debug, etc) and their arguments
      // 3.
    }
  }
  // Create database file
  // Create Log file, fill out start and end (start time, end time, etc)
  closeLog();
}
