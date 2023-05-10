#include "coordinate.hpp"

#include <string>
#include <iostream>
#include <sstream>

Coordinate::Coordinate() {
    lon = DMS();
    lat = DMS();
}

Coordinate::Coordinate(DMS longitude, DMS latitude) {
    lon = longitude;
    lat = latitude;
}
std::string Coordinate::repr() {
    std::stringstream r;
    r << "lat: " << lat.repr() << " lon: " << lon.repr() << std::endl;
    return r.str();
}


Hemisphere stringToHemisphere(std::string str);
std::string hemisphereToString(Hemisphere hem);

DMS::DMS() {
  deg = 0;
  min = 0;
  sec = 0;
  hem = Hemisphere::invalid_hemisphere;
  isLat = false;
}

DMS::DMS(int totalSeconds, Hemisphere hemisphere) { 
  
  int totalAbsSeconds = abs(totalSeconds);

  deg = int(totalAbsSeconds / 3600);
  int totalMinutes = totalAbsSeconds - deg * 3600;
  min = int(totalMinutes / 60);
  int _totalSeconds = totalMinutes - min * 60;
  sec = int(_totalSeconds);
  hem = hemisphere;
  isLat = (hem == Hemisphere::north or hem == Hemisphere::south);
  
}

DMS::DMS(int degrees, int minutes, float seconds, Hemisphere hemisphere) {
  deg = degrees;
  min = minutes;
  sec = seconds;
  hem = hemisphere;
  isLat = (hem == Hemisphere::north or hem == Hemisphere::south);
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

  isLat = (hem == Hemisphere::north or hem == Hemisphere::south);
  // std::cout << deg << "d " << min << "m " << sec << "s "
  //           << hemisphereToString(hem) << std::endl;
}

float DMS::dd() { return deg + min / 60 + sec / 3600 * (hem == Hemisphere::south or hem == Hemisphere::west ? -1 : 1); }

int DMS::totalSeconds() {
  return (deg * 3600 + min * 60 + sec) * (hem == Hemisphere::south or hem == Hemisphere::west ? -1 : 1);
}

Hemisphere DMS::hemisphere() {
  return hem;
}

bool DMS::isLatitude() {
  return isLat;
}
bool DMS::isLongitude() {
  return !isLat;
}

std::string DMS::repr() {
  std::stringstream r;
  r << (isLat ^ (abs(deg) > 100) ? "" : "0") << deg << "d " << min << "m " << sec << "s " << hemisphereToString(hem);
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
