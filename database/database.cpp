#include "database.hpp"

#include <fstream>
#include <algorithm>
#include <iostream>
#include <string.h>
#include <sstream>
#include <stack>

NameNode::NameNode() {
  feature_id = -1;
  feature_name = "";
  state_alpha = "";
  index = -1;
  isEmpty = true;
}

NameNode::NameNode(int id, std::string feature, std::string state, int i) {
  feature_id = id;
  feature_name = feature;
  state_alpha = state;
  index = i;
  isEmpty = false;
}

NameNode::NameNode(const NameNode & node) {
  if (node.isEmpty)
  {
    feature_id = -1;
    feature_name = "";
    state_alpha = "";
    isEmpty = true;
    index = -1;
  } else {
    feature_id = node.feature_id;
    feature_name = node.feature_name;
    state_alpha = node.state_alpha;
//      feature_name = std::move(node.feature_name);
//      state_alpha = std::move(node.state_alpha);
    index = node.index;
    isEmpty = node.isEmpty;
  }
}

int NameIndex::quadraticResolution(int i) {
  return i == 0 ? 0 : ((i^2  + i) / 2);
}
 
NameIndex::NameIndex(int n) {
  capacity = GoodPrimeNumbers[0];
  for (size_t i = 0; i < numPrimeNumbers; i++)
  {
    if (n < GoodPrimeNumbers[i])
    {
      capacity = GoodPrimeNumbers[i];
      capacityPrimeIndex = i;
      break;
    }
  }
//  capacityPrimeIndex++;
  buckets = new NameNode[capacity];
  std::fill_n(buckets, capacity, NameNode());
  numInserted = 0;
}

unsigned int elfHash(const std::string key) {
  /**
   * Elf hashing function
   *
   * @param key String of feature and state concatenated together
   * @return The resulting hashed value
   */
  unsigned int hashVal = 0;
  unsigned int len = key.length();
  unsigned int x = 0;

  for ( unsigned int i = 0; i < len; i++ ) {
    // Elf Hash
    hashVal = (hashVal << 4) + (key[i]);
    if ( (x = hashVal & 0xF0000000) != 0 ) {
      hashVal ^= (x >> 24);
    }
    hashVal&= ~x;
  }
  return hashVal;
}

int NameIndex::insert(int index, GISRecord record) {
    NameNode node = NameNode(record.feature_id, record.feature_name, record.state_alpha, index);
    int longestProbe = insert(node);
    if (((float)numInserted / (float)capacity) >= maxLoad) {
        rehash();
    }
    return longestProbe;
}

int NameIndex::insert(NameNode &node) {
    bool inserted = false;
    int longestProbe = -1;
  if (node.isEmpty)
  {
    return inserted;
  }
  
  unsigned int keyHash = elfHash(node.feature_name + node.state_alpha) % capacity;
//  nameMap[keyHash] = node;
  int i = 0;
  int initialIndex = (keyHash + quadraticResolution(i)) % capacity;
  while (!inserted and maxProbes > i)
  {
    int newIndex = (keyHash + quadraticResolution(i)) % capacity;
    NameNode b;
    b = buckets[newIndex];
    if ((initialIndex == newIndex and i != 0))
//      if ((initialIndex == newIndex and i != 0) or (((float)numInserted / (float)capacity) >= maxLoad))
    {
      /* Insertion has looped indices, rehash and insert */
      rehash();
     i = 0;
     longestProbe = -1;
    //  i++;
     initialIndex = (keyHash + quadraticResolution(i)) % capacity;
     newIndex = (keyHash + quadraticResolution(i)) % capacity;
      continue;
    } else if (b.isEmpty)
    {
      buckets[newIndex] = std::move(node);
      inserted = true;
      numInserted++;
      if (longestProbe < i) {
            longestProbe = i;
      }
    } else if (b.feature_name == node.feature_name and b.state_alpha == node.state_alpha)
    {
      /* Do nothing, already inserted */
      inserted = true;
    }
    else {
      i++;
    }
  }
    return longestProbe;
}
std::string tolower(std::string data) {
    std::string r;
    std::transform(data.begin(), data.end(), r.begin(),[](unsigned char c){ return std::tolower(c); });
    return r;
}
int NameIndex::hash(std::string key, int offset) {
    unsigned int keyHash = elfHash(key) % capacity;
    int quadraticOffset = quadraticResolution(offset) % capacity;
    int hash = (keyHash + quadraticOffset) % capacity;
    return hash;
}
std::vector<int> NameIndex::search(std::string feature, std::string state) {
    unsigned int keyHash = elfHash(feature + state) % capacity;
    std::vector<int> indices;
    std::vector<int> searchedHashs;
    std::string key = feature + state;
    int offset = 0;
    int currentPos = (keyHash + quadraticResolution(offset)) % capacity;
    NameNode node = buckets[currentPos];
    while( offset < capacity and offset < maxProbes)
    {
        node = buckets[currentPos];
        if (!node.isEmpty && (node.feature_name == feature) && (node.state_alpha == state)) {
            indices.push_back(node.index);
            break;
        } else {
            searchedHashs.push_back(node.feature_id);
            offset += 1;
            currentPos = (keyHash + quadraticResolution(offset)) % capacity; // Compute ith probe
        }
    }
//    std::cout << "did not find in these hash indices" << std::to_string(searchedHashs.size()) << "\tpossible: " << std::to_string(capacity) << std::endl;

//    for (int i: searchedHashs) {
//        std::cout << std::to_string(i) << std::endl;
//    }
    if (indices.empty()) {
        for (int i = 0; i < capacity; ++i) {
            NameNode node = buckets[i];
            if (!node.isEmpty and node.state_alpha == state and node.feature_name == feature) {
                indices.push_back(node.index);
            }
        }
    }
    return indices;
}

void NameIndex::rehash() {
    capacityPrimeIndex++;
  if (capacityPrimeIndex < numPrimeNumbers)
  {
    int newCapacity = GoodPrimeNumbers[capacityPrimeIndex];
      NameNode *oldBuckets;
      oldBuckets = buckets;
    numInserted = 0;
    buckets = new NameNode[newCapacity];
    int originalCap = capacity;
    for (int i = 0; i < originalCap; i++)
    {
        NameNode node;
       node = std::move(oldBuckets[i]);
       if (!node.isEmpty)
      {
          insert(node);
      }
    }
    capacity = newCapacity;
  }
}

std::string NameIndex::str()   {
  std::stringstream r;
  r << "Format of display is \nSlot number: data record \nCurrent table size is " << capacity << "\nNumber of elements in table is " << std::to_string(numInserted) << std::endl;
  int i;
  int displayed = 0;
  for (i = 0; i < capacity; i++) {
    // int keyHash = hasher(pair.first.feature_name + state);
    NameNode node = buckets[i];
    if(!node.isEmpty) {
        displayed++;
        r << "\t" << std::to_string(i) << ": [" << node.feature_name << ":" << node.state_alpha << ", [" << std::to_string(node.index) << "]]\n";
        // r << "\t" << std::to_string(i) << ": [" << node.feature_name << ":" << node.state_alpha << ", [" << std::to_string(node.index) << "][empty:" << (node.isEmpty ? "T": "F") << "]]\n";
    }
  }
    std::string s = r.str();
  return r.str();
}






BufferPool::BufferPool() {}

void BufferPool::moveToFront(int index) {
  /**
   * Moves a recently used item in the buffer pool to the front
   *
   * @param index The index of the item to move to the front
   */

  for (int i = index; i > 0; --i) {
      std::swap(cache_[i], cache_[i-1]);
  }
}

void BufferPool::insert(int index, GISRecord record) {
  /**
   * Inserts a GIS record ot the top of the dequeue
   *
   * @param index The index of the GIS record in the database
   * @param record The GIS record object
   */

  // Check if the cache is full before inserting
  if (cache_.size() == maxPoolSize) {
    // Cache is full, remove LRU
    cache_.pop_back();
  }

  // Insert the new record to the top of the cache
  cache_.push_front(std::make_pair(index, record));
}

GISRecord BufferPool::search(int index, std::string databaseFile) {
  /**
   * Search's for the specified record
   *
   * Search's the buffer pool first for the record, then if it's not found the database file is searched.
   *
   * @param indices The index of all records to be searched
   * @return all found records
   */

  // Search for the key in the cache
//  std::cout << "searching buffer pool" << std::endl;
  for (int i = 0; i < cache_.size(); ++i) {
      if (cache_[i].first == index) {
          moveToFront(i);  // Move the item to the top of the cache
          return cache_[0].second;  // Return the value from the Key/Value pair, which corresponds ot the line from the database
      }
  }

  // Key is not in the cache, check if full
  if (cache_.size() == maxPoolSize) {
    // Cache is full, remove LRU
    cache_.pop_back();
  }

  // Return record from file if not in buffer
  GISRecord rec = searchFile(index, databaseFile);
  insert(index, rec);
  return rec;
}
GISRecord BufferPool::searchFile(int index, std::string databaseFile) {
  std::ifstream file(databaseFile);
  std::string line;

  while (index >= 0) {
    std::getline(file, line);
    index--;
  }
  return GISRecord(line);
}
std::string BufferPool::str() {
  /**
   * Prints the contents of the buffer pool
   *
   * Iterates through the Buffer Pool deque cache and prints each item in order from Most Recently
   * Used (MRU) to Least Recently Used (LRU).
   *
   */
  std::stringstream r;

  if (!cache_.empty()) {
      r << "MRU" << std::endl;
      for (auto it : cache_) {
          r << "\t" << std::to_string(it.first) << ": " << it.second.str() << std::endl;
      }
      r << "LRU" << std::endl;
  } else {
      r << "Buffer Pool is empty" << std::endl;
  }
  r << "------------------------------------------------------------------------------------------" << std::endl;

  return r.str();
}







Database::Database(std::string dbFile) {
  databaseFile = std::move(dbFile);
  indexCount = 0;
  numInserted = 0;
  totalNameLength = 0;
  longestP = 0;
  std::ofstream dbStream(databaseFile);
  dbStream.close();
  nameIndex = new NameIndex(10);
}

void Database::insert(std::string recordLine) {
  // GISRecord record;
  GISRecord record(recordLine);
  // TODO: insert to coordinate index
  // TODO: insert to name index
  int lp = nameIndex->insert(indexCount, record);
  saveToFile(recordLine);
  indexCount++;
  numInserted = numInserted + (lp != -1 ? 1 : 0);
  totalNameLength = totalNameLength + record.feature_name.size();
    if (longestP < lp) {
        longestP = lp;
    }
}
void Database::saveToFile(std::string line) {
  std::fstream dbStream(databaseFile, std::ios::app);

  dbStream << line << std::endl;

  dbStream.close();
}

std::vector<GISRecord> Database::getRecords(std::vector<int> indices) {
  /**
   * Search's for the specified record
   *
   * Search's the buffer pool first for the record, then if it's not found the database file is searched.
   *
   * @param indices The index of all records to be searched
   * @return all found records
   */
  std::vector<GISRecord> records;
  for (int index : indices) {
    GISRecord rec = buffer.search(index, databaseFile);
    records.push_back(rec);
    debugNameIndex();
    debugBufferPool();
  }
  return records;
}

std::vector<GISRecord> Database::whatIsAt(Coordinate coord) {
  // TODO: Search coordinate index
  std::vector<int> indices = {1, 2, 3, 4};
  return getRecords(indices);
}

std::vector<std::string> Database::whatIs(std::string feature, std::string state) {
//  std::cout << "What is that? " << feature << ", " << state << std::endl;
  std::vector<int> indices = nameIndex->search(feature, state);
  std::vector<std::string> recordStrings;
  if (!indices.empty()) {
    std::vector<GISRecord> records = getRecords(indices);
    for (int i = 0; i < records.size(); i++) {
      GISRecord rec = records[i];
      std::stringstream rStr;
      rStr << "  " << indices[i] << ":  " << rec.county_name << "  (" << rec.primary_lat_dms << ", " << rec.prim_long_dms << ")";
      recordStrings.push_back(rStr.str());
    }

  } else {
    std::stringstream rStr;
    rStr << "  No records match \"" << feature << "\" and \"" << state << "\"";
    recordStrings.push_back(rStr.str());
  }
  
  return recordStrings;
}

std::vector<GISRecord> Database::whatIsIn(Region region) {
  // TODO: Search coordinate index
  std::vector<int> indices = {1, 2, 3, 4};
  return getRecords(indices);
}

std::string Database::debugNameIndex() {
  return nameIndex->str();
}

std::string Database::debugBufferPool() {
  return buffer.str();
}


int Database::numImported(){
  return numInserted;
}
int Database::avgNameLength(){
  return totalNameLength/numInserted;
}
int Database::longestProbe() {
    return longestP;
}
