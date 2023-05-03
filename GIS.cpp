#include <string>
#include "gis_record.hpp"
#include "system_manager.hpp"
#include "command_processor.hpp"
using namespace std;

enum Command { };
struct DMS { };

class NameIndex {

};

class CoordinateIndex {

};

class BufferPool {

};


int main(int argc, char *argv[]) {

    string databaseFilename, scriptFilename, logfileFilename;

    tie(databaseFilename, scriptFilename, logfileFilename) = SystemManager::verifyArguments(argc, argv);

    CommandProcesor commandProcessor(databaseFilename, scriptFilename, logfileFilename);
    commandProcessor.run();

    return 0;
}

