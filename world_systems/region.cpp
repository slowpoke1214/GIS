#include "region.hpp"

#include <iomanip>
#include <iostream>
#include <sstream>

Region::Region() {
  bottomLeft = Coordinate();
  topRight = Coordinate();
};

Region::Region(DMS northLat, DMS southLat, DMS eastLong, DMS westLong) {
  bottomLeft = Coordinate(westLong, southLat);
  topRight = Coordinate(eastLong, northLat);
};

DMS Region::top() { return topRight.lat; }

DMS Region::bottom() { return bottomLeft.lat; }

DMS Region::left() { return bottomLeft.lon; }

DMS Region::right() { return topRight.lon; }

Region Region::NW() {
  return Region(top(), bottom().avg(top()), right().avg(left()), left());
};
Region Region::NE() {
  return Region(top(), bottom().avg(top()), right(), left().avg(right()));
};
Region Region::SW() {
  return Region(top().avg(bottom()), bottom(), right().avg(left()), left());
};
Region Region::SE() {
  return Region(top().avg(bottom()), bottom(), right(), left().avg(right()));
};

std::string Region::repr() {
  std::stringstream r;
  std::string centerMsg = "World boundaries are set to:";
  r << std::string(90, '-') << std::endl
    << "Latitude/longitude values in index entries are shown as signed "
       "integers, in total seconds."
    << std::endl
    << std::string(90, '-') << std::endl
    << std::string(6, '\t') << centerMsg << std::endl
    << std::string(6, '\t') << std::setw(centerMsg.size() / 2)
    << std::to_string(topRight.lat.totalSeconds()) << std::endl
    << std::string(6, '\t') << std::to_string(bottomLeft.lon.totalSeconds())
    << std::setw(centerMsg.size() -
                 (std::to_string(bottomLeft.lon.totalSeconds()).size()))
    << std::to_string(topRight.lon.totalSeconds()) << std::endl
    << std::string(6, '\t') << std::setw(centerMsg.size() / 2)
    << std::to_string(bottomLeft.lat.totalSeconds()) << std::endl
    << std::string(90, '-');
  return r.str();
}
