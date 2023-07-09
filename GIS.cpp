#include <string>
#include <iostream>

// #include "gis_record.hpp"
#include "system_manager.hpp"
#include "command_processor.hpp"
using namespace std;

int main(int argc, char *argv[]) {

    string databaseFilename, scriptFilename, logfileFilename;

    try
    {
        tie(databaseFilename, scriptFilename, logfileFilename) = SystemManager::verifyArguments(argc, argv);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }

    CommandProcessor commandProcessor(databaseFilename, scriptFilename, logfileFilename);
    commandProcessor.run();

    return 0;
}

