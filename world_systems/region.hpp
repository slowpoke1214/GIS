# ifndef REGION_HPP
# define REGION_HPP

#include <string>

#include "coordinate.hpp"

class Region {
 protected:
  // Bottom left is origin similar to Cartesian plane
  // Latitude increase the farther North the coord is
  // Longitude increase the farther East the coord is
  // Width affects Longitude, height affects Latitude
  Coordinate bottomLeft;
  Coordinate topRight;

 public:
  Region(Coordinate center, int halfHeight, int halfWidth);
  Region();
  Region(DMS northLat, DMS southLat, DMS eastLong, DMS westLong);

  DMS top();
  DMS bottom();
  DMS left();
  DMS right();

  Region NW();
  Region NE();
  Region SW();
  Region SE();

  std::string repr();
};

# endif // REGION_HPP
