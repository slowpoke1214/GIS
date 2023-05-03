#include <string>

class DD;
class DMS;

class DD {
 private:
  double deg;

 public:
  DD();
  DD(float degrees);

  DMS dms();
  std::string repr();
  float toFloat();
};

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

  DD dd();
  float toFloat();
  std::string repr();
};

class Coordinate {
 private:
  DMS lon;
  DMS lat;

 public:
  Coordinate();
  Coordinate(DMS longitude, DMS latitude);
  std::string repr();
};
