#include <string>
// #include "gis_record.hpp"
#include "system_manager.hpp"
#include "command_processor.hpp"
using namespace std;

int main(int argc, char *argv[]) {

    string databaseFilename, scriptFilename, logfileFilename;

    tie(databaseFilename, scriptFilename, logfileFilename) = SystemManager::verifyArguments(argc, argv);

    CommandProcessor commandProcessor(databaseFilename, scriptFilename, logfileFilename);
    commandProcessor.run();

    return 0;
}

