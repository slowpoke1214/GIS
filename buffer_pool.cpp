#include "buffer_pool.hpp"

GISRecord BufferPool::search(const int index) {
    // Searches the buffer pool for provided items
    GISRecord gisRecord("5");  // Temp Variable for testing

    // Search for the key in the cache
    for (int i = 0; i < cache_.size(); ++i) {
        if (cache_[i].first == index) {
            moveToFront(i);  // Move the item to the top of the cache
            return cache_[0].second;  // Return the value from the Key/Value pair, which corresponds ot the line from the database
        }
    }

    // If key is not in cache:
    if (cache_.size() == maxPoolSize) {
        // Cache is full, remove LRU
    }

    // Insert new item to front of cache
    // TODO: Call database function to retrieve GISRecord from database file

    return gisRecord;
}

void BufferPool::moveToFront(int index) {
    // Moves a recently used item in the buffer pool to the front
    for (int i = index; i > 0; --i) {
        std::swap(cache_[i], cache_[i-1]);
    }
}

void BufferPool::insert(int index, std::string value) {
    // Inserts a new entry into the buffer pool
}

std::string BufferPool::str() {
    // Prints the contents of the buffer pool in the desired format
    return "BufferPool";
}
