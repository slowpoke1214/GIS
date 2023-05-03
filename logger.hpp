#include <sstream>

class Logger {
   private:
    std::string commandLogSeperator = std::string(90, '-');

   public:
    std::string logFile;
    Logger();
    Logger(std::string filename);
    void write(std::string message);
    void write(std::string command, int commandNumber);
};
