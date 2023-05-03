#include <iostream>
#include <sstream>

#include "coordinate.hpp"

Hemisphere stringToHemisphere(std::string str);
std::string hemisphereToString(Hemisphere hem);

DMS::DMS() {
  deg = 0;
  min = 0;
  sec = 0;
}

DMS::DMS(int degrees, int minutes, float seconds, Hemisphere hemisphere) {
  deg = degrees;
  min = minutes;
  sec = seconds;
  hem = hemisphere;
}

DMS::DMS(std::string dmsString) {
  // std::cout << "dms constructor;\n" << dmsString << std::endl;

  Hemisphere hemisphere = stringToHemisphere(std::string(1, dmsString.back()));

  dmsString.pop_back();
  int seconds = std::stoi(dmsString.substr(dmsString.size() - 2));
  dmsString.pop_back();
  dmsString.pop_back();
  int minutes = std::stoi(dmsString.substr(dmsString.size() - 2));
  dmsString.pop_back();
  dmsString.pop_back();
  int degrees = std::stoi(dmsString);

  deg = degrees;
  min = minutes;
  sec = seconds;
  hem = hemisphere;

  // std::cout << deg << "d " << min << "m " << sec << "s "
  //           << hemisphereToString(hem) << std::endl;
}

DD DMS::dd() { return DD(deg + min / 60 + sec / 3600); }

float DMS::toFloat() { return dd().toFloat(); }

std::string DMS::repr() {
  std::stringstream r;
  r << deg << "d " << min << "m " << sec << "s " << hemisphereToString(hem);
  return r.str();
}

Hemisphere stringToHemisphere(std::string str) {
  Hemisphere hem = Hemisphere::invalid_hemisphere;
  if (str == "N") {
    hem = Hemisphere::north;
  } else if (str == "S") {
    hem = Hemisphere::south;
  } else if (str == "E") {
    hem = Hemisphere::east;
  } else if (str == "W") {
    hem = Hemisphere::west;
  }
  return hem;
}

std::string hemisphereToString(Hemisphere hem) {
  std::string str = "invalid_hemisphere";
  if (hem == Hemisphere::north) {
    str = "N";
  } else if (hem == Hemisphere::south) {
    str = "S";
  } else if (hem == Hemisphere::east) {
    str = "E";
  } else if (hem == Hemisphere::west) {
    str = "W";
  }
  return str;
}