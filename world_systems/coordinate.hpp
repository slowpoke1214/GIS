# ifndef COORDINATE_HPP
# define COORDINATE_HPP

#include <string>

enum class Hemisphere { north, south, east, west, invalid_hemisphere };
Hemisphere stringToHemisphere(std::string str);
std::string hemisphereToString(Hemisphere hem);

class DMS {
 private:
  static const int latDigits = 7;
  static const int lonDigits = 8;
  int deg;
  int min;
  float sec;
  Hemisphere hem;
  bool isLat;

 public:
  DMS();
  DMS(int totalSeconds, Hemisphere hemisphere);
  DMS(int totalSeconds, bool isLatitude);
  DMS(int degrees, int minutes, float seconds, Hemisphere hemisphere);
  DMS half();
  DMS avg(DMS rhs);
  DMS(std::string dmsString);

  float dd();
  int totalSeconds();
  Hemisphere hemisphere();
  bool isLatitude();
  bool isLongitude();
  std::string repr();
};

class Coordinate {
 public:
  DMS lon;
  DMS lat;
  Coordinate();
  Coordinate(DMS longitude, DMS latitude);
  std::string repr();
  std::string repr(int halfHeight, int halfWidth);
};

# endif // COORDINATE_HPP
