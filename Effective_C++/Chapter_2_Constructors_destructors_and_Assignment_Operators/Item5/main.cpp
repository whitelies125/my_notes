#include <iostream>
#include <string>

template <typename T> class NamedObject {
public:
  NamedObject(std::string &name, const T &value)
      : nameValue(name), objectValue(value) {}

private:
  std::string &nameValue; // reference
  const T objectValue;    // const
};

class Base {
  Base &operator=(const Base &rhs) { return *this; }
};

class Derived : public Base {};

class A {
public:
  A() = default;
  A(const A &a) { std::cout << "call copy constructor A" << std::endl; }
};

class B : public A {
public:
  B() = default;
  // 使用编译器隐式生成的 copy constructor
};

class C : public A {
public:
  C() = default;
  // 使用用户显式定义的 copy constructor
  C(const C &b) { std::cout << "call copy constructor C" << std::endl; }
};
int main() {
  std::string newDog("Persephone");
  std::string oldDog("Satch");
  NamedObject<int> p(newDog, 2);
  NamedObject<int> s(oldDog, 36);
  NamedObject<int> t(s);

  p = s; // 生成的 copy 操作代码不合法，编译器不生成 copy assignment operator

  Derived d1;
  Derived d2;
  d2 = d1; // Base 的 copy assignment operator 为 private，编译器不为 Derived
           // 生成 copy assignment operator

  B b1;
  B b2(b1); // 编译器隐式生成的 copy constructor 默认会调用父类 copy constructor
            // 因此会打印 call copy constructor A
  C c1;
  C c2(c1); // 用户定义的 copy constructor 不调用父类 copy constructor
            // 因此仅打印 call copy constructor C
  return 0;
}
