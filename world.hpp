#include "gis_helpers.hpp"
#include "world_systems/region.hpp"

class world : private Region {
 private:
 public:
  world();
  world(DMS westLong, DMS eastLong, DMS southLat, DMS northLat);
};

// world::world(double westLong, double eastLong, double southLat, double northLat) {}

