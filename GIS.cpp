#include <string>
#include <iostream>
#include <fstream>
#include "gis_record.hpp"
#include "logger.hpp"
using namespace std;

enum Command { };
struct DMS { };

class NameIndex {

};

class CoordinateIndex {

};

class BufferPool {

};

class SystemManager {

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

public:

    CommandProcesor(string db, string script, string log) {
        databaseFile = std::move(db);
        scriptFile = std::move(script);
        logger = Logger(log);
        commandNumber = 0;
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

    }
};

int main(int argc, char *argv[]) {
    const char * database_filename = argv[1];
    const char * script_filename = argv[2];
    const char * logfile_filename = argv[3];

    CommandProcesor commandProcessor(database_filename, script_filename, logfile_filename);
    commandProcessor.run();

    return 0;
}
