#include <string>

template <typename T> class NamedObject {
public:
  NamedObject(std::string &name, const T &value);

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
  NamedObject<int> t(s);

  p = s; // 生成的 copy 操作代码不合法，编译器不生成 copy assignment
         // operator，会报错

  Derived d1;
  Derived d2;
  d2 = d1; // Base 的 copy assignment operator 为 private，编译器不为 Derived
           // 生成 copy assignment operator
  return 0;
}
