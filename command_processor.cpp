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
  database.resetImportStats();
  std::string line;
  std::getline(file, line);
  while (std::getline(file, line)) {
    database.insert(line, worldBorder);
  }
  std::stringstream out;
  out <<
  "Imported Features by name: " << std::to_string(database.numImported()) << std::endl <<
  "Longest probe sequence:    " << std::to_string(database.longestProbe()) << std::endl <<
  "Imported Locations:        " << std::to_string(database.numImported()) << std::endl <<
  "Average name length:       " << std::to_string(database.avgNameLength());
  logger.write(out.str());
}

void CommandProcessor::run() {
  scriptStream.open(scriptFile, std::ios::in);
  std::string commandString;

  if (scriptStream) {
    bool hasQuit = false;
    while (std::getline(scriptStream, commandString) and !hasQuit) {
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
              logger.write(database.debugWorld());
            } else if (args[0] == "pool") {
              logger.write(database.debugBufferPool());
            } else if (args[0] == "hash") {
              logger.write(database.debugNameIndex());
            } else if (args[0] == "quad") {
              logger.write(database.debugCoordinateIndex());
            }
            // Unknown value
            logger.writeSeparator();
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
          case Command::what_is_at:
          {
            Coordinate coord = Coordinate(args[1], args[0]);
            std::vector<std::string> records = database.whatIsAt(coord);

            for (auto &&rec : records)
            {
              logger.write("\t" + rec);
            }
            logger.writeSeparator();
            break;
          }
          case Command::what_is_in: {
            std::vector<std::string> records;
            if (args[0] == "-filter") {
              // Filter
              Coordinate coord = Coordinate(args[3], args[2]);
              std::string filterType = args[1];
              int halfHeight = std::stoi(args[4]);
              int halfWidth = std::stoi(args[5]);
              records = database.what_is_in(coord, filterType, halfHeight, halfWidth);
            } else if (args[0] == "-long") {
              // Long Listing
              Coordinate coord = Coordinate(args[2], args[1]);
              int halfHeight = std::stoi(args[3]);
              int halfWidth = std::stoi(args[4]);
              records = database.what_is_in(coord, true, halfHeight, halfWidth);
            } else {
              // Defualt usage
              Coordinate coord = Coordinate(args[1], args[0]);
              int halfHeight = std::stoi(args[2]);
              int halfWidth = std::stoi(args[3]);
              records = database.what_is_in(coord, halfHeight, halfWidth);
            }
            // TODO: Find a way to pass the additional parameters dynamically:
            // TODO: Case 1: what_is_in -filter [pop|water|strcuture] <lat> <long> <half-height> <half-width>
            // TODO: Case 2: what_is_in -long <lat> <long> <half-height> <half-width>
            // TODO: Case 3: what_is_in <lat> <long> <half-height> <half-width>
            // The center of the rectangle search area
//            Coordinate coord = Coordinate(args[1], args[0]);  // TODO: Cant used args[1]/args[2] because of dynamic input // TODO: Pass optional parameters?
            for (auto &&rec: records) {
              logger.write("\t" + rec);
            }
            logger.writeSeparator();
            break;
          }
          case Command::quit:
          {
            hasQuit = true;
            logger.write("Terminating execution of commands.");
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
