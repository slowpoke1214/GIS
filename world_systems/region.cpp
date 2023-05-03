#include "region.hpp"
#include <iostream>

Region::Region(DMS northLat, DMS southLat, DMS eastLong, DMS westLong) {
  bottomLeft = Coordinate(southLat, westLong);
  height = northLat.toFloat() - southLat.toFloat();
  width = eastLong.toFloat() - westLong.toFloat();
  //   std::cout << bottomLeft.repr() << std::endl;
};
