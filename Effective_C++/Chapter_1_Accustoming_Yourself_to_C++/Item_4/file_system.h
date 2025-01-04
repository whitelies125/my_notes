#include <cstddef>

class FileSystem {
private:
    std::size_t numDisks_;
public:
    FileSystem(std::size_t n) : numDisks_(n) {}
    std::size_t numDisks() const {
        return numDisks_;
    }
};
extern FileSystem tfs;
