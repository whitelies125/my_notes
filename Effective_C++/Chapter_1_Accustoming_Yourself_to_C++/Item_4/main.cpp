#include <cstring>
#include <iostream>
#include "directory.h"

class Cls {
public:
    int a;
    int b;
    // 容易让人误以为 b 先初始化为 1，随后 a 初始化为 b 的取值 1
    // 实际上的初始化顺序是先 a 后 b，因此 a 初始化时，b 还未初始化
    Cls(): b(1), a(b) {}
};

int main() {
    {
        Cls cls;
        std::cout << cls.a << std::endl;
        std::cout << cls.b << std::endl;
        cls.b = 2;
    }
    {
        Cls cls;
        std::cout << cls.a << std::endl;
        std::cout << cls.b << std::endl;
    }
    {
        Cls cls;
        std::cout << cls.a << std::endl;
        std::cout << cls.b << std::endl;
    }
    std::cout << std::endl;
    std::cout << dir.disks << std::endl;
    return 0;
}
