#include <string>
#include <array>

#ifndef SRC_BUFFER_POOL_H
#define SRC_BUFFER_POOL_H

class BufferPool {
private:
    const static int maxPoolSize = 15;
    std::array<std::pair<int, std::string>, maxPoolSize> cache_;  // Array of Key/Value pairs, where the key is the index corresponding to the database
    void moveToFront(int index);
    void insert(int index, std::string value);
public:
    std::string search(int index);
    std::string str();
};

#endif //SRC_BUFFER_POOL_H
