#include "logger.hpp"

#include <fstream>
#include <sstream>

Logger::Logger() {}

Logger::Logger(std::string filename) {
    logFile = std::move(filename);
    std::ofstream logStream(logFile);
    logStream.close();
}

void Logger::write(std::string message) {
    std::fstream logStream(logFile, std::ios::app);

    logStream << message << std::endl;

    logStream.close();
}

void Logger::write(std::string command, int commandNumber) {
    if (commandNumber == 0) {
        write(command);
    } else {
        std::fstream logStream(logFile, std::ios::app);
        logStream << "Command " << commandNumber << ": " << command
                  << std::endl;
        logStream.close();
    }
}
