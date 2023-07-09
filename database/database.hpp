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
  int numInserted;
  int totalNameLength;
  int longestP;

  BufferPool buffer;
  NameIndex* nameIndex;
  // CoordinateIndex coordIndex;

  void saveToFile(std::string line);

  std::vector<GISRecord> getRecords(std::vector<int> indices);

 public:
  Database(std::string dbFile);

  void insert(std::string recordLine);

  std::vector<GISRecord> whatIsAt(Coordinate coord);
  std::vector<std::string> whatIs(std::string feature, std::string state);
  std::vector<GISRecord> whatIsIn(Region region);

  std::string debugNameIndex();

  std::string debugBufferPool();
  int numImported();
  int avgNameLength();
  int longestProbe();
  void resetImportStats();
};

# endif // DATABASE_HPP
