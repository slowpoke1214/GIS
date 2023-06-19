#include <functional>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../gis_record.hpp"
#include "../world_systems/region.hpp"

class NameNode {
 private:
 public:
  std::string feature_name;
  std::string state_alpha;
  int index;

  NameNode();
  NameNode(std::string feature_name, std::string state_alpha, int index);
  NameNode(const NameNode& node);
};

class NameIndex {
 private:
  // Temporarily use STD hash class build out NameIndex class
  // Will implement elfhash at later time
  std::hash<std::string> hasher;
  // Temporarily use STD unordered map to build out NameIndex class
  // Will implement hash table at later time
  std::unordered_map<unsigned int, NameNode> nameMap;

 public:
  NameIndex();

  void insert(int index, GISRecord record);
  std::vector<int> search(std::string feature, std::string state);

  std::string str();
};

class BufferPool {
 private:
 public:
  BufferPool();

  void insert(int index, GISRecord record);
  GISRecord search(int index);

  std::string str();
};

class CoordinateIndex {
 private:
 public:
  CoordinateIndex();

  void insert(int index, GISRecord record);
  std::vector<int> search(/* Parameters TBD */);

  std::string str();
};

class Database {
 private:
  std::string databaseFile;
  int indexCount;

  BufferPool buffer;
  NameIndex nameIndex;
  // CoordinateIndex coordIndex;

  std::string searchFile(int index);
  void saveToFile(std::string line);

  std::vector<GISRecord> getRecords(std::vector<int> indices);

 public:
  Database(std::string dbFile);

  void insert(std::string recordLine);

  std::vector<GISRecord> whatIsAt(Coordinate coord);
  std::vector<std::string> whatIs(std::string feature, std::string state);
  std::vector<GISRecord> whatIsIn(Region region);

  std::string debugNameIndex();
};
