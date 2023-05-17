#include <string>

#ifndef SRC_BUFFER_POOL_H
#define SRC_BUFFER_POOL_H

class BufferPool {
private:
    const static int maxPoolSize = 15;
    void insert();  // Private method because will be used only by buffer pool within search()?
public:
    std::string search();
    std::string str();
};

#endif //SRC_BUFFER_POOL_H
