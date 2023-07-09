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
  unsigned int keyHash = elfHash(record.feature_name + record.state_alpha) % capacity;
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
  unsigned int keyHash = elfHash(feature + state) % capacity;
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

GISRecord BufferPool::search(int index) {
  /**
   * Search's for the specified record
   *
   * Search's the buffer pool first for the record, then if it's not found the database file is searched.
   *
   * @param indices The index of all records to be searched
   * @return all found records
   */

  // Search for the key in the cache
  std::cout << "searching buffer pool" << std::endl;
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

  // Return empty record if nothing is found
  GISRecord rec;
  return rec;
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

struct CoordinateIndex::CoordinateIndexPoint {
  int index;
  GISRecord record;

  CoordinateIndexPoint() : index(0), record(GISRecord()) {}
};

struct CoordinateIndex::CoordinateIndexNode {
  std::vector<CoordinateIndexPoint> points;
//  std::vector<CoordinateIndexNode*> children;
  CoordinateIndexNode* NW;  // NW Pointer
  CoordinateIndexNode* NE;  // NE Pointer
  CoordinateIndexNode* SW;  // SW Pointer
  CoordinateIndexNode* SE;  // SE Pointer
  Region nodeBorder;

  CoordinateIndexNode() : NW(nullptr), NE(nullptr), SW(nullptr), SE(nullptr), nodeBorder() {}
};

CoordinateIndex::CoordinateIndex(int k) {
  /**
   * Constructor function for creating a Quad Tree
   *
   * @param k Number of GIS records permit-able in each quadrant. If exceeded, resizing occurs.
   */

  leafCapacity = k;  // Max capacity of GIS records for each leaf
  root = nullptr;  // Root Pointer
  // worldBorder = world
}

void CoordinateIndex::insert(int index, GISRecord record, world worldBorder) {
  /**
   *
   */

  // try to insert into quadrant
  // run get quadrant, if occupied then recursively run get quadrant??

  // Check if root node does not exist
  if (root == nullptr) {
    // Start of quad tree
    root = new CoordinateIndexNode();
    // Set border
    root->nodeBorder = worldBorder;
  }

  // total seconds values of the records coordinate
  int recordLatTotalSeconds = DMS(record.primary_lat_dms).totalSeconds();
  int recordLongTotalSeconds = DMS(record.prim_long_dms).totalSeconds();

  // For testing purposes to view values
  int top = worldBorder.top().totalSeconds();
  int bot = worldBorder.bottom().totalSeconds();
  int left = worldBorder.left().totalSeconds();
  int right = worldBorder.right().totalSeconds();

  // Ensure point is within global world bounds
  if (worldBorder.top().totalSeconds() >= recordLatTotalSeconds &&
      worldBorder.bottom().totalSeconds() <= recordLatTotalSeconds &&
      worldBorder.left().totalSeconds() <= recordLongTotalSeconds &&
      worldBorder.right().totalSeconds() >= recordLongTotalSeconds) {
    // The point is within the region bounds
    // Create Point
    CoordinateIndexPoint p;
    p.index = index;
    p.record = record;

    // Recursively insert the point
   recursiveInsertPoint(root, p);
  } else {
    // The point is outside the region bounds
    return;
  }
}

void CoordinateIndex::recursiveInsertPoint(CoordinateIndexNode* node, const CoordinateIndexPoint& point) {
  /**
 * Inserts a CoordinateIndexPoint into the quad tree node or its child nodes.
 *
 * @param node The quad tree node.
 * @param point The point to be inserted.
 */

  // Check if children are >= leafCapacity
  if (node->points.size() >= leafCapacity) {
    // Split the node into 4 quadrants
    splitNode(node);
  }

  // NODE BORDER TESTING VALUE
  int top = node->nodeBorder.top().totalSeconds();
  int bottom = node->nodeBorder.bottom().totalSeconds();
  int left = node->nodeBorder.left().totalSeconds();
  int right = node->nodeBorder.right().totalSeconds();

  // Check if the retrieved child node is a leaf
  if (node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SE == nullptr) {
    // If it is a leaf, add the point to its points vector
    node->points.push_back(point);
  } else {
    // If it is not a leaf, recursively insert the point into the child node
    // Get the quadrant that the point belongs to
    DMS p1 = DMS(point.record.primary_lat_dms);
    DMS p2 = DMS(point.record.prim_long_dms);
    CoordinateIndexNode* child = getQuadrant(node, p1, p2);
    recursiveInsertPoint(child, point);
  }
}

CoordinateIndex::CoordinateIndexNode* CoordinateIndex::getQuadrant(CoordinateIndexNode* node, DMS lat_dms, DMS long_dms) {
  /**
   * Gets the quadrant a Point belongs to
   */
  CoordinateIndexNode* child = nullptr;

  // TESTING VALUES
      int half1 = node->nodeBorder.top().totalSeconds();
      int half2 = node->nodeBorder.bottom().totalSeconds();
      int half3 = node->nodeBorder.left().totalSeconds();
      int half4 = node->nodeBorder.right().totalSeconds();

  // Node border centers
  int borderCenterLong = (node->nodeBorder.left().totalSeconds() + node->nodeBorder.right().totalSeconds()) / 2;  // Vertical center
  int borderCenterLat = (node->nodeBorder.bottom().totalSeconds() + node->nodeBorder.top().totalSeconds()) / 2;  // Horizontal center
  // total seconds values of the records coordinate
  int recordLatTotalSeconds = lat_dms.totalSeconds();
  int recordLongTotalSeconds = long_dms.totalSeconds();

  if (recordLatTotalSeconds <= borderCenterLat) {
    // South Half
    if (recordLongTotalSeconds <= borderCenterLong) {
      // South West
      if (node->SW == nullptr) {
        node->SW = new CoordinateIndexNode();
      }
      child = node->SW;
    } else {
      // South East
      if (node->SE == nullptr) {
        node->SE = new CoordinateIndexNode();
      }
      child = node-> SE;
    }
  } else {
    // North Half
    if (recordLongTotalSeconds <= borderCenterLong) {
      // North West
      if (node->NW == nullptr) {
        node->NW = new CoordinateIndexNode();
      }
      child = node->NW;
    } else {
      // North East
      if (node->NE == nullptr) {
        node->NE = new CoordinateIndexNode();
      }
      child = node->NE;
    }
  }

  return child;
}

std::vector<int> CoordinateIndex::what_is_at(Coordinate coord) {
  /**
   *
   */
   std::vector<int> searchResults;
   what_is_at_recursive(root, coord, searchResults);
   return searchResults;
}

void CoordinateIndex::what_is_at_recursive(CoordinateIndexNode* node, Coordinate coord, std::vector<int>& searchResults) {
  /**
   *
   */

  if (node == nullptr) {
    return;
  }

  if (node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SE == nullptr) {
    // Leaf node, check if any point matches the coordinate
    for (const CoordinateIndexPoint& point : node->points) {
      if (
          DMS(point.record.primary_lat_dms).totalSeconds() == coord.lat.totalSeconds() &&
          DMS(point.record.prim_long_dms).totalSeconds() == DMS(coord.lon).totalSeconds()) {
        // Coordinates match, add to array
        searchResults.push_back(point.index);
      }
    }
  } else {
    // Not a leaf node
    DMS p1 = DMS(coord.lat);
    DMS p2 = DMS(coord.lon);
    int lat_secs = p1.totalSeconds();
    int lon_secs = p2.totalSeconds();
    CoordinateIndexNode* child = getQuadrant(node, p1, p2);
    what_is_at_recursive(child, coord, searchResults);
  }
}

std::vector<int> CoordinateIndex::what_is_in(Region region) {
  /**
   * Returns all points within a specified region
   */

  // TODO: Traverse the quad tree and only return points that are within the region parameter
  std::vector<int> searchResults;
  what_is_in_recursive(root, region, searchResults);
  return searchResults;
}

void CoordinateIndex::what_is_in_recursive(CoordinateIndex::CoordinateIndexNode *node, Region region,
                                           std::vector<int> &searchResults) {
  /**
   * Recursively iterates through the quad tree to search for points within a specified region
   */
   if (node == nullptr) {
     return;
   }

   // Check if part of the region is within the nodes borders

}

void CoordinateIndex::splitNode(CoordinateIndexNode* node) {
  /**
   * Splits a node when a new point is assigned to an occupied region
   */
   // split the node into 4 quadrants
      node->NW = new CoordinateIndexNode();
      node->NE = new CoordinateIndexNode();
      node->SW = new CoordinateIndexNode();
      node->SE = new CoordinateIndexNode();

      // TESTING VALUES
      int um = node->nodeBorder.top().totalSeconds();
      int umm = node->nodeBorder.bottom().totalSeconds();
      int ummm = node->nodeBorder.left().totalSeconds();
      int ummmm = node->nodeBorder.right().totalSeconds();

      // TESTING VALUES
      int half1 = node->nodeBorder.top().half().totalSeconds();
      int half2 = node->nodeBorder.bottom().half().totalSeconds();
      int half3 = node->nodeBorder.left().half().totalSeconds();
      int half4 = node->nodeBorder.right().half().totalSeconds();

      // Sets the correct border size of each new quadrant
      node->NW->nodeBorder = node->nodeBorder.NW();
      node->NE->nodeBorder = node->nodeBorder.NE();
      node->SW->nodeBorder = node->nodeBorder.SW();
      node->SE->nodeBorder = node->nodeBorder.SE();

      // TESTING VALUES
      // NW
      int hm = node->NW->nodeBorder.top().totalSeconds();
      int hmm = node->NW->nodeBorder.bottom().totalSeconds();
      int hmmm = node->NW->nodeBorder.left().totalSeconds();
      int hmmmm = node->NW->nodeBorder.right().totalSeconds();
      // NE
      int dm = node->NE->nodeBorder.top().totalSeconds();
      int dmm = node->NE->nodeBorder.bottom().totalSeconds();
      int dmmm = node->NE->nodeBorder.left().totalSeconds();
      int dmmmm = node->NE->nodeBorder.right().totalSeconds();
      // SW
      int fm = node->SW->nodeBorder.top().totalSeconds();
      int fmm = node->SW->nodeBorder.bottom().totalSeconds();
      int fmmm = node->SW->nodeBorder.left().totalSeconds();
      int fmmmm = node->SW->nodeBorder.right().totalSeconds();
      // SE
      int tm = node->SE->nodeBorder.top().totalSeconds();
      int tmm = node->SE->nodeBorder.bottom().totalSeconds();
      int tmmm = node->SE->nodeBorder.left().totalSeconds();
      int tmmmm = node->SE->nodeBorder.right().totalSeconds();

      // Transfer the points from parent node to their respective quadrants
      for ( const CoordinateIndexPoint& p : node->points ) {
        // Get the quadrant that the point belongs to
        DMS p1 = DMS(p.record.primary_lat_dms);
        DMS p2 = DMS(p.record.prim_long_dms);
        CoordinateIndexNode* quadrant = getQuadrant(node, p1, p2);

        if (quadrant != nullptr) {
          quadrant->points.push_back(p);
        }
      }

      // Clear the points from the parent node
      node->points.clear();
}

std::string CoordinateIndex::str() {
  /**
   * Pre-order traversal of the Quad Tree
   */
  std::string preorderPrint = preorderTraversal(root, 0);
  preorderPrint += "------------------------------------------------------------------------------------------";
//  std::cout << preorderPrint << std::endl;

  return preorderPrint;
}

std::string CoordinateIndex::preorderTraversal(CoordinateIndex::CoordinateIndexNode *node, int depth) {
  /**
   * Preorder Traversal function for printing the Quad Tree's contents
   */
  std::stringstream result;

  if (node != nullptr) {
    // Print the current node
    result << std::string(depth, '\t') << "@\n";

    // Check if the node is a leaf
    if (node->NW == nullptr && node->NE == nullptr && node->SW == nullptr && node->SE == nullptr) {
      // Print the leaf node's points
      result << std::string(depth + 1, '\t') << "* ";
      for (const CoordinateIndexPoint& point : node->points) {
        std::string recordLatTotalSeconds = std::to_string(DMS(point.record.primary_lat_dms).totalSeconds());
        std::string recordLongTotalSeconds = std::to_string(DMS(point.record.prim_long_dms).totalSeconds());
        result << "[(" + recordLatTotalSeconds << "," << recordLongTotalSeconds << "), " << std::to_string(point.index) << "] ";
      }
      result << "\n";
    } else {
      // Print the child nodes recursively
      if (node->NW != nullptr)
        result << preorderTraversal(node->NW, depth + 1);
      if (node->NE != nullptr)
        result << preorderTraversal(node->NE, depth + 1);
      if (node->SW != nullptr)
        result << preorderTraversal(node->SW, depth + 1);
      if (node->SE != nullptr)
        result << preorderTraversal(node->SE, depth + 1);
    }
  }

  return result.str();
}

std::string CoordinateIndex::visualize() {
  /**
   *
   */
   std::string display = "return";
   return display;
}

Database::Database(std::string dbFile) {
  databaseFile = std::move(dbFile);
  indexCount = 1;
  std::ofstream dbStream(databaseFile);
  dbStream.close();
  nameIndex = new NameIndex(1000);
  coordinateIndex = new CoordinateIndex(4);
  // TODO: Create new Quad tree with k = 4
}

void Database::insert(std::string recordLine, world worldBorder) {
  // GISRecord record;
  GISRecord record(recordLine);
  // TODO: insert to coordinate index
  // TODO: insert to name index
  coordinateIndex->insert(indexCount, record, worldBorder);
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
    // GISRecord rec;
    GISRecord rec = buffer.search(index);
    if (rec.empty()) {
        // Record does not exist in buffer pool, search db file instead
      std::string recLine = searchFile(index);
      // std::cout << "recLine: " << recLine << std::endl;
      rec = GISRecord(recLine);

      buffer.insert(index, rec);
    }
    records.push_back(rec);
    debugNameIndex();
    debugBufferPool();
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

std::vector<std::string> Database::whatIsAt(Coordinate coord) {
  // TODO: Search coordinate index
//  std::vector<int> indices = {1, 2, 3, 4};
  std::vector<int> indices = coordinateIndex->what_is_at(coord);
  std::vector<std::string> recordStrings;
  if (!indices.empty()) {
    std::stringstream str;
    str << "\t The following feature(s) were found at " + coord.repr();
    recordStrings.push_back(str.str());
    std::vector<GISRecord> records = getRecords(indices);
    for (int i = 0; i < records.size(); i++) {
      GISRecord rec = records[i];
      std::stringstream rStr;
      rStr << "\t\t" << indices[i] << ":  \"" << rec.feature_name << "\"  \"" << rec.county_name << "\"  \"" << rec.state_alpha << "\"";
      recordStrings.push_back(rStr.str());
    }

  } else {
    std::stringstream rStr;
    rStr << "  Nothing was found at " << coord.repr();
    recordStrings.push_back(rStr.str());
  }
  return recordStrings;
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

std::vector<std::string> Database::what_is_in(Coordinate coord, int halfHeight, int halfWidth) {
  // TODO: Create region object
  Region region = Region(coord, halfHeight, halfWidth);

  std::vector<int> indices = coordinateIndex->what_is_in(region);
  return std::vector<std::string>();
}

std::vector<std::string>
Database::what_is_in(Coordinate coord, std::string &filterType, int halfHeight, int HalfWidth) {
  return std::vector<std::string>();
}

std::vector<std::string> Database::what_is_in(Coordinate coord, bool longListing, int halfHeight, int HalfWidth) {
  return std::vector<std::string>();
}

//std::vector<std::string> Database::whatIsIn(Coordinate coord) {
//  // TODO: add the <half_height and width> parameter, and possibly an optional parameter? like pythons **kwargs.
////  std::vector<int> indices = {1, 2, 3, 4};
//  std::vector<std::string> recordStrings;
//  return recordStrings;
//
//  // TODO: Possibly create the region object here? Then pass only the region object and the additional object to the CoordinateIndex::what_is_in function. Yeah seems best
//  // TODO: Potentially need to have the coordinates here to call coord.repr() for the "The following 1 feature(s) were found in (38d 21m 48s North +/- 15, 79d 31m 9s West +/- 15)" type shit
////  std::vector<int> indices = coordinateIndex->what_is_in(coord);
////  if (!indices.empty()) {
////    std::stringstream str;
////    str << "\t The following feature(s) were found at " + coord.repr();
////    recordStrings.push_back(str.str());
////    std::vector<GISRecord> records = getRecords(indices);
////    for (int i = 0; i < records.size(); i++) {
////      GISRecord rec = records[i];
////      std::stringstream rStr;
////      rStr << "\t\t" << indices[i] << ":  \"" << rec.feature_name << "\"  \"" << rec.county_name << "\"  \"" << rec.state_alpha << "\"";
////      recordStrings.push_back(rStr.str());
////    }
////
////  } else {
////    std::stringstream rStr;
////    rStr << "  Nothing was found at " << coord.repr();
////    recordStrings.push_back(rStr.str());
////  }
////  return recordStrings;
//}

std::string Database::debugNameIndex() {
  return nameIndex->str();
}

std::string Database::debugBufferPool() {
  return buffer.str();
}

std::string Database::debugCoordinateIndex() {
  return coordinateIndex->str();
}

