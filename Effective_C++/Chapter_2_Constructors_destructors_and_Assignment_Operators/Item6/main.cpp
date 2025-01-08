#include <iostream>
class A {
public:
  A() = default;
  A(const A &a) { std::cout << "call copy constructor A" << std::endl; }
  A &operator=(const A &a) {
    std::cout << "call copy assignment operator A" << std::endl;
    return *this;
  }
};

class B : public A {
public:
  B() = default;
  // 使用编译器隐式生成的 copy constructor
  // 使用编译器隐式生成的 copy assignment operator
};

class C : public A {
public:
  C() = default;
  // 使用用户显式定义的 copy constructor
  C(const C &b) { std::cout << "call copy constructor C" << std::endl; }
  // 使用用户显式定义的 copy assignment operator
  C &operator=(const C &b) {
    std::cout << "call copy assignment operator C" << std::endl;
    return *this;
  }
};
int main() {
  B b1;
  B b2(b1); // 编译器隐式生成的 copy constructor 默认会调用父类 copy constructor
            // 因此会打印 call copy constructor A
  B b3;
  b3 = b1; // 打印 call copy constructor A
  C c1;
  C c2(c1); // 用户定义的 copy constructor 不调用父类 copy constructor
            // 因此仅打印 call copy constructor C
  C c3;
  c3 = c1; // 仅打印 call copy constructor C
  return 0;
}
