#include "gis_helpers.hpp"

#include <fstream>
#include <sstream>

namespace helpers {

std::vector<std::string> splitString(const std::string& str,
                                     const char delimeter) {
  std::stringstream data(str);
  std::vector<std::string> attributes;

  std::string attr;
  while (std::getline(data, attr, delimeter)) {
    attributes.push_back(attr);
  }
  return attributes;
}
}  // namespace helpers
