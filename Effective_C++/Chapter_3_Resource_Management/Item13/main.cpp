// 注意 makefile 中编译选项为 c++98
#include <iostream>
#include <memory>
#include <vector>

class Test {
public:
    Test() {
        std::cout << "call Test()" << std::endl;
    };
    ~Test() {
        std::cout << "call ~Test()" << std::endl;
    };
};

void example_1() {
    std::auto_ptr<Test> ptr (new Test());
}

void example_2() {
    std::vector<std::auto_ptr<Test> > vec;
    vec.push_back(std::auto_ptr<Test>(new Test())); // 编译报错:
    // mingw64/include/c++/13.2.0/bits/new_allocator.h:201:9:
    // error: no matching function for call to 'std::auto_ptr<Test>::auto_ptr(const std::auto_ptr<Test>&)'

    // vector.push_back 的形参为 const T &，注意此时带上了 const 修饰
    // vector.push_back 中一路跟踪下去会在 new_allocator.h 中调用:
    // construct(pointer __p, const _Tp& __val) { ::new((void *)__p) _Tp(__val); }
    // 这里在通过 copy constructor 构造一个新变量
    // 因此会调用 auto_ptr 的 copy constructor，但注意此处为 const _Tp&，即 const auto_ptr&
    // 因为 auto_ptr 在 copy 时需要将 rhs 的指针置为 null，这涉及到修改成员变量
    // 所以 auto_ptr 的 copy constructor 和 copy assignment operator 的形参都不能为 const 修饰
    // 因此无法找到相匹配的 copy constructor，编译报出上文错误信息
}


int main() {
    example_1();
    example_2();
    return 0;
}
