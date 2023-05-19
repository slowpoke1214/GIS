#include "gis_record.hpp"

#include <iostream>

#include "gis_helpers.hpp"

GISRecord::GISRecord() { isEmpty = true; }

GISRecord::GISRecord(const GISRecord &record) {
  feature_id = record.feature_id;
  feature_class = std::move(record.feature_class);
  state_numeric = std::move(record.state_numeric);
  county_name = std::move(record.county_name);
  county_numeric = std::move(record.county_numeric);
  primary_lat_dms = std::move(record.primary_lat_dms);
  prim_long_dms = std::move(record.prim_long_dms);
  prim_lat_dec = record.prim_lat_dec;
  prim_long_dec = record.prim_long_dec;
  source_lat_dms = std::move(record.source_lat_dms);
  source_long_dms = std::move(record.source_long_dms);
  source_lat_dec = record.source_lat_dec;
  source_long_dec = record.source_long_dec;
  elev_in_m = record.elev_in_m;
  elev_in_ft = record.elev_in_ft;
  map_name = std::move(record.map_name);
  date_created = std::move(record.date_created);
  date_edited = std::move(record.date_edited);
  isEmpty = record.isEmpty;
  feature_name = std::move(record.feature_name);
  state_alpha = std::move(record.state_alpha);
  // std::cout << str();
}

bool GISRecord::empty() { return isEmpty; }

GISRecord::GISRecord(const std::string &line) {
  std::vector<std::string> attributes =
      helpers::splitString(line, GISRecordDelimeter, NumGISRecordFields);

  try {
    if (attributes.size() != NumGISRecordFields) {
      throw std::invalid_argument("Not enough fields in GIS Record");
    }

    if (!attributes[0].empty()) {
      feature_id = std::stoi(attributes[0]);
    }

    feature_name = attributes[1];
    feature_class = attributes[2];
    state_alpha = attributes[3];
    state_numeric = attributes[4];
    county_name = attributes[5];
    county_numeric = attributes[6];
    primary_lat_dms = attributes[7];
    prim_long_dms = attributes[8];
    if (!attributes[9].empty()) {
      prim_lat_dec = std::stof(attributes[9]);
    }
    if (!attributes[10].empty()) {
      prim_long_dec = std::stof(attributes[10]);
    }
    source_lat_dms = attributes[11];
    source_long_dms = attributes[12];
    if (!attributes[13].empty()) {
      source_lat_dec = std::stof(attributes[13]);
    }
    if (!attributes[14].empty()) {
      source_long_dec = std::stof(attributes[14]);
    }
    if (!attributes[15].empty()) {
      elev_in_m = std::stoi(attributes[15]);
    }
    if (!attributes[16].empty()) {
      elev_in_ft = std::stoi(attributes[16]);
    }
    map_name = attributes[17];
    date_created = attributes[18];
    date_edited = attributes[19];
  } catch (const std::exception &e) {
    std::stringstream err;
    err << "Error parsing GIS Record. Line in file does not conform to "
           "standard form: "
        << e.what();
    throw std::invalid_argument(err.str());
  }
  isEmpty = false;
}

GISRecord::~GISRecord() {}
std::string GISRecord::str() {
  return "feature name: " + feature_name + ", state: " + state_alpha +
         ", id: " + std::to_string(feature_id);
}
