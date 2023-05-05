#include <string>

enum class Hemisphere { north, south, east, west, invalid_hemisphere };
Hemisphere stringToHemisphere(std::string str);
std::string hemisphereToString(Hemisphere hem);

class DMS {
 private:
  int deg;
  int min;
  float sec;
  Hemisphere hem;

 public:
  DMS();
  DMS(int degrees, int minutes, float seconds, Hemisphere hemisphere);
  DMS(std::string dmsString);

  float dd();
  int totalSeconds();
  std::string repr();
};

class Coordinate {
 public:
  DMS lon;
  DMS lat;
  Coordinate();
  Coordinate(DMS longitude, DMS latitude);
  std::string repr();
};
