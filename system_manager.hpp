#include <string>
#include <tuple>

class SystemManager {
   private:
    const static int expectedArgs = 3;
    static bool fileExists(const std::string& filename);

   public:
    static std::tuple<std::string, std::string, std::string> verifyArguments(
        int argc, char* argv[]);
};
