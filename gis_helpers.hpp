#include <string>
#include <vector>

namespace helpers {
std::vector<std::string> splitString(const std::string &str,
                                     const char delimeter, const int expectedLength = -1);
}  // namespace helpers
