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

int main() {
  std::string newDog("Persephone");
  std::string oldDog("Satch");
  NamedObject<int> p(newDog, 2);
  NamedObject<int> s(oldDog, 36);
  NamedObject<int> t(s); // copy constructor 会生成，拷贝构造在这种场景下是合法合理的。

  p = s; // 生成的 copy 操作代码不合法，编译器不生成 copy assignment operator

  Derived d1;
  Derived d2;
  d2 = d1; // Base 的 copy assignment operator 为 private，编译器不为 Derived 生成 copy assignment operator
  return 0;
}
