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
  isEmpty = true;
}

NameNode::NameNode(int id, std::string feature, std::string state, int i) : isEmpty(false) {
  feature_id = id;
  feature_name = feature;
  state_alpha = state;
  index = i;
}

NameNode::NameNode(const NameNode & node) {
  if (node.isEmpty)
  {
    feature_id = -1;
    feature_name = "";
    state_alpha = "";
    isEmpty = true;
    return;
  } 

  feature_id = node.feature_id;
  feature_name = std::move(node.feature_name);
  state_alpha = std::move(node.state_alpha);
  index = node.index;
  isEmpty = node.isEmpty;
}


int NameIndex::mostSignificantBit(int x) {
  if (x == 0)
    return 0;
  int mostSignificantBit = 31;
  while ((x & (1 << mostSignificantBit)) == 0)
    mostSignificantBit--;
  return mostSignificantBit;
}

int NameIndex::quadraticResolution(int i) {
  return ((i^2  + i) / 2);
}
 
NameIndex::NameIndex(int n) {
  capacity = GoodPrimeNumbers[mostSignificantBit(n) - 1];
  buckets = new NameNode[capacity];
  std::fill_n(buckets, capacity, NameNode());
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

void NameIndex::insert(int index, GISRecord record) {
  unsigned int keyHash = elfHash(record.feature_name + record.state_alpha);
  NameNode node = NameNode(record.feature_id, record.feature_name, record.state_alpha, index);
  // std::cout << "inserting node:\t" << node.feature_id << "\t" << node.feature_name << "\t" << node.index << "\t" << node.isEmpty  << "\t" << node.state_alpha << std::endl;
  nameMap[keyHash] = node;
  int i = 0;
  bool inserted = false;
  int initialIndex = (keyHash + quadraticResolution(i)) % capacity;
  while (!inserted)
  {
    int newIndex = (keyHash + quadraticResolution(i)) % capacity;
    NameNode b = buckets[newIndex];
    if (initialIndex == newIndex)
    {
      /* Insertion has looped indices, rehash and insert */
      return;
    } else if (b.isEmpty)
    {
      buckets[newIndex] = node;
      inserted = true;
    } else if (b.feature_id == record.feature_id)
    {
      /* Do nothing, already inserted */
      inserted = true;
    }
     else {
      i++;
    } 
  }
}

std::vector<int> NameIndex::search(std::string feature, std::string state) {
  unsigned int keyHash = elfHash(feature + state);
  std::vector<int> indices;
  // I dont like auto types but the alternative is just as bad...
  // std::unordered_map<int, int>::iterator
  auto resultIndex = nameMap.find(keyHash);
  if (resultIndex != nameMap.end()) {
    std::cout << "Found indices in name index" << std::endl;
    indices.push_back(resultIndex->second.index);
  } else {
    std::cout << "Did not find indices in name index" << std::endl;
  }

  return indices;
}

std::string NameIndex::str()   {
  std::stringstream r;
  r << "Format of display is \nSlot number: data record \nCurrent table size is " << capacity << "\nNumber of elements in table is " << nameMap.size() << std::endl;
  for (auto const& pair : nameMap) {
    // int keyHash = hasher(pair.first.feature_name + state);
    NameNode node = pair.second;
    r << "\t" << pair.first << ": [" << node.feature_name << ":" << node.state_alpha << ", [" << std::to_string(node.index) << "]]\n";
    // r << "\t" << pair.first << ": [" << node.feature_name << ":" << node.state_alpha << ", [" << std::to_string(node.index) << "]]\tID: " << node.feature_id << "\n";
  }
  return r.str();
}










BufferPool::BufferPool() {}

void BufferPool::insert(int index, GISRecord record) {
  return;
}

GISRecord BufferPool::search(int index) {
    // TODO: Bufferpool
  std::cout << "searching buffer pool" << std::endl;
  GISRecord rec;
  return rec;
}

























Database::Database(std::string dbFile) {
  databaseFile = std::move(dbFile);
  indexCount = 1;
  std::ofstream dbStream(databaseFile);
  dbStream.close();
  nameIndex = new NameIndex(1000);
}

void Database::insert(std::string recordLine) {
  // GISRecord record;
  GISRecord record(recordLine);
  // TODO: insert to coordinate index
  // TODO: insert to name index
  nameIndex->insert(indexCount, record);
  saveToFile(recordLine);
  indexCount++;
}
void Database::saveToFile(std::string line) {
  std::fstream dbStream(databaseFile, std::ios::app);

  dbStream << line << std::endl;

  dbStream.close();
}

std::vector<GISRecord> Database::getRecords(std::vector<int> indices) {
  std::vector<GISRecord> records;
  for (int index : indices) {
    // GISRecord rec;
    GISRecord rec = buffer.search(index);
    if (rec.empty()) {
      std::string recLine = searchFile(index);
      // std::cout << "recLine: " << recLine << std::endl;
      rec = GISRecord(recLine);

      buffer.insert(index, rec);
    }
    records.push_back(rec);
    debugNameIndex();
  }
  return records;
}

std::string Database::searchFile(int index) {
  std::ifstream file(databaseFile);
  std::string line;

  while (index > 0) {
    std::getline(file, line);
    index--;
  }
  return line;
}

std::vector<GISRecord> Database::whatIsAt(Coordinate coord) {
  // TODO: Search coordinate index
  std::vector<int> indices = {1, 2, 3, 4};
  return getRecords(indices);
}

std::vector<std::string> Database::whatIs(std::string feature, std::string state) {
  std::cout << "What is that? " << feature << ", " << state << std::endl;
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
