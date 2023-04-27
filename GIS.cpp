#include <string>
#include <iostream>
#include <fstream>
using namespace std;

enum Command { };
struct DMS { };

class GISRecord {

};

class NameIndex {

};

class CoordinateIndex {

};

class BufferPool {

};

class Logger {
    // TODO:
    // Initialize the logger file (Create it)
public:
    static void write() {
        // Write to logger file
    }

};

class SystemManager {

};

class CommandProcesor {
    // TODO:
    //  Read script file
public:
    // Instantiate class attributes
    string databaseFile;
    string scriptFile;
    string logfile;
    fstream scriptStream;

    CommandProcesor(string db, string script, string log) {
        databaseFile = std::move(db);
        scriptFile = std::move(script);
        logfile = std::move(log);
    }

    void run() {
        // TODO:
        // Read Script file
        scriptStream.open(scriptFile, ios::in);
        string command;

        if (scriptStream) {
            while (std::getline(scriptStream, command)) {
                cout << command;
                cout << "\n";

                if (command[0] == ';') {
                    // Line is comment, log it using Logger class
                } else {
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
