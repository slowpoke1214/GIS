#include <sstream>

class Logger {
   private:
    std::string logFile;

   public:
    Logger();
    Logger(std::string filename);
    void write(std::string message);
    void write(std::string command, int commandNumber);
};
