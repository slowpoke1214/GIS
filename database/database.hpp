# ifndef DATABASE_HPP
# define DATABASE_HPP

#include <functional>
#include <queue>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include "../gis_record.hpp"
#include "../world_systems/region.hpp"
#include "../logger.hpp"

using world = Region;

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
  void rehash();
  int insert(NameNode &node);
  int numInserted;
  constexpr static const float maxLoad = 0.7;
    const static int maxProbes = 100;
  int hash(std::string key, int offset);
  int quadraticResolution(int i);

  NameNode* buckets;
    static const int numPrimeNumbers = 26;
  /// Good prime numbers to use as Hashtable sizes
  /// Copied from https://web.archive.org/web/20120705020114/http://planetmath.org/encyclopedia/GoodHashTablePrimes.html
  int GoodPrimeNumbers[numPrimeNumbers] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289,
    24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469,
    12582917, 25165843, 50331653, 100663319, 201326611, 402653189,
    805306457, 1610612741
};
// To keep track of which capacity values have been used
int capacityPrimeIndex;

 public:
  NameIndex(int n);

  int insert(int index, GISRecord record);
  std::vector<int> search(std::string feature, std::string state);

  std::string str();
};

class BufferPool {
 private:
  const static int maxPoolSize = 15;
  GISRecord searchFile(int index, std::string databaseFile);
  std::deque<std::pair<int, GISRecord>> cache_; // Double ended queue of Key/Value pairs, where the key is the index corresponding to the database, and the value is the GISRecord
 public:
  BufferPool();

  void moveToFront(int index);
  void insert(int index, GISRecord record);
  GISRecord search(int index, std::string databaseFile);

  std::string str();
};

class CoordinateIndex {
 private:
  struct CoordinateIndexPoint;
  struct CoordinateIndexNode;

  CoordinateIndexNode* root;
//  CoordinateIndexNode* NW;
//  CoordinateIndexNode* NE;
//  CoordinateIndexNode* SW;
//  CoordinateIndexNode* SE;
  int leafCapacity;
  world worldBorder;

  void splitNode(CoordinateIndexNode* node);
  CoordinateIndexNode* getQuadrant(CoordinateIndexNode* node, DMS lat_dms, DMS long_dms);
  void recursiveInsertPoint(CoordinateIndexNode* node, const CoordinateIndexPoint& point);
  void what_is_in_recursive(CoordinateIndexNode* node, Region region, std::vector<int>& searchResults);
  void what_is_at_recursive(CoordinateIndexNode* node, Coordinate coord, std::vector<int>& searchResults);
  std::string preorderTraversal(CoordinateIndexNode* node, int depth);
 public:
  explicit CoordinateIndex(int k);

  void insert(int index, GISRecord record, world worldBorder);
  std::vector<int> what_is_at(Coordinate coord);

  // Overload functions for what_is_in
  std::vector<int> what_is_in(Region region);


  std::string str();
  std::string visualize();
};

class Database {
 private:
  std::string databaseFile;
  int indexCount;
  int numInserted;
  int totalNameLength;
  int longestP;

  BufferPool buffer;
  NameIndex* nameIndex;
  CoordinateIndex* coordinateIndex;
  // CoordinateIndex coordIndex;

  void saveToFile(std::string line);

  std::vector<GISRecord> getRecords(std::vector<int> indices);

 public:
  Database(std::string dbFile);

  void insert(std::string recordLine, world worldBorder);

  std::vector<std::string> whatIsAt(Coordinate coord);
  std::vector<std::string> whatIs(std::string feature, std::string state);
  // Overload functions for what_is_in
  std::vector<std::string> what_is_in(Coordinate coord, int halfHeight, int HalfWidth);
  std::vector<std::string> what_is_in(Coordinate coord, std::string& filterType, int halfHeight, int halfWidth);
  std::vector<std::string> what_is_in(Coordinate coord, bool longListing, int halfHeight, int halfWidth);

  std::string debugNameIndex();

  std::string debugBufferPool();

  std::string debugCoordinateIndex();

  int numImported();
  int avgNameLength();
  int longestProbe();
  void resetImportStats();
};

# endif // DATABASE_HPP
