#ifndef LOGGER
#define LOGGER

#include <sstream>

class Logger {
   private:

   public:
    std::string logFile;
    Logger();
    explicit Logger(std::string filename);
    void write(std::string message);
    void write(std::string command, int commandNumber);
};

#endif
