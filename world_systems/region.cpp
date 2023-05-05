#include "region.hpp"

#include <iostream>

Region::Region() {
  bottomLeft = Coordinate();
  height = 0;
  width = 0;
};

Region::Region(DMS northLat, DMS southLat, DMS eastLong, DMS westLong) {
  bottomLeft = Coordinate(southLat, westLong);
  height = northLat.totalSeconds() - southLat.totalSeconds();
  width = eastLong.totalSeconds() - westLong.totalSeconds();
  //   std::cout << bottomLeft.repr() << std::endl;
};

DMS Region::top() {
  return DMS(std::to_string(bottomLeft.lon.totalSeconds() + height));
}

DMS Region::bottom() { return bottomLeft.lon; }

DMS Region::left() { return bottomLeft.lat; }

DMS Region::right() {
  return DMS(std::to_string(bottomLeft.lat.totalSeconds() + width));
}
