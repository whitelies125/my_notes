#include "file_system.h"

class Directory {
public:
    std::size_t disks;
    Directory(std::size_t n) : disks(n){
        std::size_t disks = tfs.numDisks();
    }
};

extern Directory dir;
