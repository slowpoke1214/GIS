#include "coordinate.hpp"

#include <string>
#include <iostream>
#include <sstream>

Coordinate::Coordinate() {
    lon = DMS();
    lat = DMS();
}

Coordinate::Coordinate(DMS longitude, DMS latitude) {
    lon = longitude;
    lat = latitude;
}
std::string Coordinate::repr() {
    std::stringstream r;
    r << "lat: " << lat.repr() << " lon: " << lon.repr() << std::endl;
    return r.str();
}
