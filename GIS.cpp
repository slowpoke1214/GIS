#include <string>
#include <iostream>
#include <fstream>
#include <ctime>
#include "gis_record.hpp"
#include "logger.hpp"
#include "system_manager.hpp"
using namespace std;

enum Command { };
struct DMS { };

class NameIndex {

};

class CoordinateIndex {

};

class BufferPool {

};

class CommandProcesor {
    // TODO:
    //  Read script file
private:
    // Instantiate class attributes
    string databaseFile;
    string scriptFile;
    Logger logger;
    fstream scriptStream;
    int commandNumber;

void initLog() {
    stringstream msg;
    msg << "Course Project for COMP 8042" << endl
        << "Student Name: Tushya Iyer, Student Id: A01023434" << endl
        << "Student Name: Joey Kennedy, Student Id: <STUDENT ID>" << endl
        << "Begin of GIS Program log:" << endl
        << "dbFile: " << databaseFile << endl
        << "script: " << scriptFile << endl
        << "log: " << logger.logFile << endl
        << "Start Time: " << getDatetime();

    logger.write(msg.str());
}

void closeLog() {
    stringstream msg;
    msg << "End time: " << getDatetime();
    logger.write(msg.str());
}

static string getDatetime() {
    time_t now = time(0);
    tm* nowTm = localtime(&now);
    char buffer[30];

    strftime(buffer, 30, "%a %b %d %H:%M:%S %Z %Y", nowTm);
    return buffer;
}

public:

    CommandProcesor(string db, string script, string log) {
        databaseFile = std::move(db);
        scriptFile = std::move(script);
        logger = Logger(log);
        commandNumber = 0;

        initLog();
    }

    void run() {
        // TODO:
        // Read Script file
        scriptStream.open(scriptFile, ios::in);
        string command;

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
};

int main(int argc, char *argv[]) {

    string databaseFilename, scriptFilename, logfileFilename;

    tie(databaseFilename, scriptFilename, logfileFilename) = SystemManager::verifyArguments(argc, argv);

    CommandProcesor commandProcessor(databaseFilename, scriptFilename, logfileFilename);
    commandProcessor.run();

    return 0;
}

