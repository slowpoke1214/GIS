#include "coordinate.hpp"

#include <iostream>
#include <sstream>
#include <string>

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
  r << "(" << lat.repr() << ", " << lon.repr() << ")" << std::flush;
  return r.str();
}

std::string Coordinate::repr(int halfHeight, int halfWidth) {
  std::stringstream r;
  r << "(" << lat.repr() << " +/- " << std::to_string(halfHeight) << ", " << lon.repr() << " +/- " << std::to_string(halfWidth) << ")" << std::endl;
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

DMS::DMS(int totalSeconds, bool isLatitude) {
  Hemisphere h = Hemisphere::invalid_hemisphere;
  if (isLatitude){
      h = totalSeconds > 0 ? Hemisphere::north : Hemisphere::south;
  } else {
      h = totalSeconds > 0 ? Hemisphere::east : Hemisphere::west;
  }

  int totalAbsSeconds = abs(totalSeconds);

  deg = int(totalAbsSeconds / 3600);
  int totalMinutes = totalAbsSeconds - deg * 3600;
  min = int(totalMinutes / 60);
  int _totalSeconds = totalMinutes - min * 60;
  sec = int(_totalSeconds);
  hem = h;
  isLat = isLatitude;
}

DMS::DMS(int degrees, int minutes, float seconds, Hemisphere hemisphere) {
  deg = degrees;
  min = minutes;
  sec = seconds;
  hem = hemisphere;
  isLat = (hem == Hemisphere::north or hem == Hemisphere::south);
}

DMS DMS::half() {
  int tempDeg = deg / 2;
  int dR = deg % 2;

  min = min + dR * 60;
  int tempMin = min / 2;
  int mR = min % 2;

  sec = sec + mR * 60;
  int tempSec = sec / 2;

  return DMS(tempDeg,tempMin, tempSec, hem);
}

DMS DMS::avg(DMS rhs){
  if ((isLatitude() and rhs.isLatitude()) or (isLongitude() and rhs.isLongitude()))
  {
    int t = (totalSeconds() + rhs.totalSeconds()) / 2 ;
    Hemisphere h = abs(totalSeconds()) > abs(rhs.totalSeconds()) ? hemisphere() : rhs.hemisphere();
    return DMS(t, h);
  } else
  {
    throw std::invalid_argument("Cannot compare a longitude to a latitude");
  }
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

float DMS::dd() {
  return deg + min / 60 +
         sec / 3600 *
             (hem == Hemisphere::south or hem == Hemisphere::west ? -1 : 1);
}

int DMS::totalSeconds() {
  return (deg * 3600 + min * 60 + sec) *
         (hem == Hemisphere::south or hem == Hemisphere::west ? -1 : 1);
}

Hemisphere DMS::hemisphere() { return hem; }

bool DMS::isLatitude() { return isLat; }
bool DMS::isLongitude() { return !isLat; }

std::string DMS::repr() {
  std::stringstream r;
  r << (isLat ^ (abs(deg) > 100) ? "" : "") << deg << "d " << min << "m "
    << sec << "s " << hemisphereToString(hem);
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
    str = "North";
  } else if (hem == Hemisphere::south) {
    str = "South";
  } else if (hem == Hemisphere::east) {
    str = "East";
  } else if (hem == Hemisphere::west) {
    str = "West";
  }
  return str;
}
