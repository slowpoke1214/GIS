#include "region.hpp"

#include <iostream>

Region::Region() {
  bottomLeft = Coordinate();
  topRight = Coordinate();
};

Region::Region(DMS northLat, DMS southLat, DMS eastLong, DMS westLong) {
  bottomLeft = Coordinate(southLat, westLong);
  topRight = Coordinate(northLat, eastLong);
};

DMS Region::top() { return topRight.lon; }

DMS Region::bottom() { return bottomLeft.lon; }

DMS Region::left() { return bottomLeft.lat; }

DMS Region::right() { return topRight.lat; }
