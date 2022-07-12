# Chain of Constructors and Destructors

构造链和析构链。

创建派生类时，默认地会调用基类的构造函数，自然，销毁派生类时，也会默认地调用基类地析构函数。

## Constructor and Destructor Chaining

构造和析构函数链。

#### constructor chaining

构造函数链。

- Constructing an instance of a class invokes all the base class along the inheritance chain.

  构造类实例会沿着继承链调用所有基类 ctor。

- 调用次序：base first，derive next。父先子后。

#### destructor chaining

析构函数链。

- Conversely，the destructors are automatically invoked in reverse order.

  dtor 与 ctor 正好相反。

- 调用次序：deriver firest, base next。子先父后。

## Example

~~~C++
#include <iostream>
class Computer{
public:
    Computer(){ std::cout << "Computer()" << std::endl; }
    ~Computer(){ std::cout << "~Computer()" << std::endl; }
};
class Pc : public Computer{
public:
    Pc(){ std::cout << "Pc()" << std::endl; }
    ~Pc(){ std::cout << "~Pc()" << std::endl; }
};
class Desktop : public Pc{
public:
    Desktop(){ std::cout << "Desktop()" << std::endl; }
    ~Desktop(){ std::cout << "~Desktop()" << std::endl; }
};
class Laptop : public Pc{
public:
    Laptop(){ std::cout << "Laptop()" << std::endl; }
    ~Laptop(){ std::cout << "~Laptop()" << std::endl; }
};
int main(){
    Desktop d{};
    std::cout << "---" << std::endl;
    Laptop l{};
    std::cout << "---" << std::endl;
    return 0;
}
//output:
//Computer()
//Pc()
//Desktop()
//---
//Computer()
//Pc()
//Laptop()
//---//程序结束时，自动释放内存。
//~Laptop()
//~Pc()
//~Computer()
//~Desktop()
//~Pc()
//~Computer()
~~~

下面测试内嵌成员构造与基类构造函数的调用次序。

~~~C++
#include <iostream>
class Computer{
public:
    Computer(){ std::cout << "Computer()" << std::endl; }
    ~Computer(){ std::cout << "~Computer()" << std::endl; }
};
class Pc : public Computer{
public:
    Pc(){ std::cout << "Pc()" << std::endl; }
    ~Pc(){ std::cout << "~Pc()" << std::endl; }
};
class Desktop : public Pc{
public:
    Desktop(){ std::cout << "Desktop()" << std::endl; }
    ~Desktop(){ std::cout << "~Desktop()" << std::endl; }
};
class Camera{
public:
    Camera(){ std::cout << "Camera()" << std::endl; }
    ~Camera(){ std::cout << "~Camera()" << std::endl; }
};
class Laptop : public Pc{
private:
    Camera c{};
public:
    Laptop(){ std::cout << "Laptop()" << std::endl; }
    ~Laptop(){ std::cout << "~Laptop()" << std::endl; }
};
int main(){
    Laptop l{};
    std::cout << "---" << std::endl;
    return 0;
}
//output:
//Computer()
//Pc()
//Camera()
//Laptop()
//---
//~Laptop()
//~Camera()
//~Pc()
//~Computer()
~~~

可见，
构造时是先调用基类构造函数，然后才是内嵌成员，最后是本类构造函数函数体。
销毁时正好是倒序。