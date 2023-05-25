#include <string>
#include <array>
#include "deque"
#include "gis_record.hpp"

#ifndef SRC_BUFFER_POOL_H
#define SRC_BUFFER_POOL_H

class BufferPool {
private:
    const static int maxPoolSize = 15;
    std::deque<std::pair<int, GISRecord>> cache_; // Double ended queue of Key/Value pairs, where the key is the index corresponding to the database, and the value is the GISRecord
    void moveToFront(int index);
    void insert(int index, std::string value);
public:
    GISRecord search(int index);
    std::string str();
};

#endif //SRC_BUFFER_POOL_H
