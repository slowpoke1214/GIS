#include "system_manager.hpp"
#include <iostream>

#include <fstream>
#include <sstream>
#include <tuple>

std::tuple<std::string, std::string, std::string>
SystemManager::verifyArguments(int argc, char* argv[]) {
    const char* databaseFilename = argv[1];
    const char* scriptFilename = argv[2];
    const char* logfileFilename = argv[3];

    if (argc - 1 < expectedArgs) {
        throw std::invalid_argument("Not enough arguments provided");

    } else if (fileExists(scriptFilename)) {
        return std::make_tuple(databaseFilename, scriptFilename,
                               logfileFilename);
    } else {
        std::stringstream err;
        err << "Script file at the path provided does not exist. Please "
               "provide a valid path. ("
            << scriptFilename << ")" << std::flush;
        throw std::invalid_argument(err.str());
    }
}

bool SystemManager::fileExists(const std::string& filename) {
    std::ifstream file(filename);
    // file.open(fname);
    return file ? true : false;
}
