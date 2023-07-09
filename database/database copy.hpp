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
  bool isEmpty;
  int feature_id;
  std::string feature_name;
  std::string state_alpha;
  int index;

  NameNode();
  NameNode(int feature_id, std::string feature_name, std::string state_alpha, int index);
  NameNode(const NameNode& node);
};

class NameIndex {
 private:

  int capacity;

  // Temporarily use STD hash class build out NameIndex class
  // Will implement elfhash at later time
  std::hash<std::string> hasher;
  // Temporarily use STD unordered map to build out NameIndex class
  // Will implement hash table at later time
//  std::unordered_map<unsigned int, NameNode> nameMap;

  /// Returns the bit index of the most significant bit.
  /// If the input is zero, it returns zero
  /// This function will help you locate the crrect good prime in the array below
  /// It will also help you compute the next power of two
  static int mostSignificantBit(int x);
  
  int quadraticResolution(int i);

  NameNode* buckets;
  /// Good prime numbers to use as Hashtable sizes
  /// Copied from https://web.archive.org/web/20120705020114/http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
  int GoodPrimeNumbers[26] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
    24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
    12582917, 25165843, 50331653, 100663319, 201326611, 402653189,
    805306457, 1610612741
};

 public:
  NameIndex(int n);

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
  NameIndex* nameIndex;
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
