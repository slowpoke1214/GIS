#include <sstream>

const char GISRecordDelimeter = '|';
const int NumGISRecordFields = 20;

class GISRecord {
   private:
    int feature_id;
    std::string feature_class;
    std::string state_numeric;
    std::string county_name;
    std::string county_numeric;
    std::string primary_lat_dms;
    std::string prim_long_dms;
    double prim_lat_dec;
    double prim_long_dec;
    std::string source_lat_dms;
    std::string source_long_dms;
    double source_lat_dec;
    double source_long_dec;
    int elev_in_m;
    int elev_in_ft;
    std::string map_name;
    std::string date_created;
    std::string date_edited;

    bool isEmpty;

   public:
    GISRecord();
    GISRecord(const GISRecord &record);
    GISRecord(const std::string &line);
    ~GISRecord();
    std::string feature_name;
    std::string state_alpha;

    bool empty();
};
