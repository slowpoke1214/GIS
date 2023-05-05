#include "coordinate.hpp"

class Region {
 protected:
  // Bottom left is origin similar to Cartesian plane
  // Latitude increase the farther North the coord is
  // Longitude increase the farther East the coord is
  // Width affects Longitude, height affects Latitude
  Coordinate bottomLeft;
  float height;
  float width;

 public:
  // Region(Coordinate bottomLeft, float height, float width);
  Region();
  Region(DMS northLat, DMS southLat, DMS eastLong, DMS westLong);

  DMS top();
  DMS bottom();
  DMS left();
  DMS right();
};
