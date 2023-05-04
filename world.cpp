#include "world.hpp"

#include <iostream>

world::world() {}

world::world(DMS westLong, DMS eastLong, DMS southLat, DMS northLat)
    : Region::Region(northLat, southLat, eastLong, westLong) {
  // std::cout << "World bottom left: " << bottomLeft.repr() << std::endl;
}
