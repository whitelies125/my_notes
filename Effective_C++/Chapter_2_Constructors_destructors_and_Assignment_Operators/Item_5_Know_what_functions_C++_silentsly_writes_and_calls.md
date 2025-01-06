条款 5：了解 C++ 默默编写并调用了哪些函数

当我们写了一个空类，
~~~cpp
class Empyt {
// there is nothing
}
~~~
但经过编译器处理后，其很可能由编译器自动添加上如下 public 且 inline（详见[[Item_30]] 的函数：
~~~cpp
class Empty {
public:
    Empty() {...} // default constructor
    Empty(const Empty& rhs) {...} // copy constructor
    ~Empty() {...} // destructor
    Empty & operator=(const Empty& rhs) {...} // copy assignment operator
}
~~~
当然这些函数的定义并不是无条件生成的，**只有当这类函数在程序中确有调用，才会生成相应的函数体定义，否则不生成**：
~~~cpp
Empty e1; // 生成 default constructor 和 destructor
Empty e2(e1);  // 生成 copy constructor
e2 = e1; // 生成 copy assignment operator
~~~
## default constructor，default destructor
default constructor 与 default destructor 的作用是调用基类和调用 non-static 成员变量的 constructor 和 destructor。
### default constructor
见下文[[#copy constructor，copy assignment operator]]
### default destructor

**需注意**，一般而言编译器自动生成的 destructor 是 non-vitrual 的（详见[[Item_7]]），除非该类继承自父类且父类已有声明为 virtual 的 destructor，则生成的 destructor 为 virtual，即这种情况下子类的 desructor 的虚函数属性来自于父类。
~~~cpp
class Base {
public:
  virtual ~Base(){}
};

class Empty : public Base{
    // 生成 virtual 的 default destructor
};

int main() {
  	Empty e1;
  	return 0;
}
~~~
## copy constructor，copy assignment operator
对于拷贝构造函数和拷贝赋值操作符，编译器生成的版本仅简单地拷贝每个 non-static data member。

考虑以下示例：
~~~cpp
template<typename T>
class NamedObject {
public:
    NamedObject(const char *name, const T &value);
    NamedObject(const std::string& name, const T& value);
private:
    std::string nameValue;
    T objectValue;
}
~~~
**因为 NamedObject 已声明了 constructor，所以编译器不会为其生成 default constructor。**
这意味着，当你设计了一个构造函数需要提供参数的类时，无需担心编译器会自动生成（无需提供传参的） default constructor 绕开了你的设计。

NamedObject 既没有声明了 copy constructor 也没有声明 copy assignment operator，因此编译器会自动生成这二者（如果其确有被调用的话）。
### copy constructor

~~~cpp
NamedObject<int> no1("Smallest Prime Number", 2);
NamedObject<int> no2(no1); // 调用 copy constructor
~~~
编译器生成的 copy constructor 必须以 no1.nameValue 和 no1.objectValue 的值初始化 no2.nameValue 和 no2.objectValue。
nameValue 的类型为 string，同时 string 类型有 copy constructor，因此，no2.nameValue 会经以 no1.nameValue 为传参调用 string 的 copy constructor 进行初始化。
NamedObject\<int\>::objectValue 是 int 类型，是 built-in 类型，因此，no2.obejctValue 会经以拷贝 no1.obejctValue 的每一个 bit 进行初始化。
### copy assignment operator
编译器生成的 copy assignment operator 在与上述 copy construtor 的行为基本相同。
但一般情况下，只有在生成的代码既合法又合理时，编译器生成的拷贝赋值运算符才会按照上述的方式工作。
当生成的代码不合法或不合理时，则编译器不会生成 operator=

考虑以下示例：
~~~cpp
template<typename T>
class NamedObject {
public:
    // 因为 nameValue 改为了 reference 所以传参 name 去除 const
    NamedObject(std::string& name, const T& value);
private:
    std::string& nameValue; // 改为 reference
    const T objectValue; // 改为 const
}

std::string newDog("Persephone");
std::string oldDog("Satch");
NamedObject<int> p(newDog, 2);
NamedObject<int> s(oldDog, 36);

p = s; // 生成的 copy 操作代码不合法，编译器不生成 copy assignment operator，会报错
~~~
注意到最后一行代码 p = s，考虑如何实现其 copy 操作。
p.nameValue 与 s.nameValue 都为 string 类型的 reference。
倘若将 p.nameValue 变更为 "Satch" 的引用，然而 C++ 中不允许一个 reference 绑定不同对象。
又或者 p.nameValue 引用对象值修改为 "Satch"，但这又会导致所有引用或指向 "Satch" 的其他对象发生改变。
objectValue 为 const 修饰，修改 const 对象在 C++ 中也是不合法的操作。

对此，C++ 选择拒绝编译赋值操作代码，不会为其生成 copy assignment operator。
如果你想要在包含 reference 成员的类中支持 copy assignment，则需要自己定义 copy assignment operator。
想要在包含 const 成员的类中支持 copy assignment，也一样需要自定义。
因为编译器不知道在 copy assignment operator 中如何正确处理这些场景。

此外，若子类继承父类且父类声明了 private 属性的 copy assignment operator，则编译器拒绝为子类生成 copy assignment operator。
因为，一般情况下，编译器生成的 copy assignment operator 应当处理父类中的部分（子类调用父类的 copy assignment operator）。但这种情况下，子类没有权限调用父类的 copy assignment operator，无法处理。

---

（个人注：查阅 cppreference[^1]关于 default constructor 和 copy assignment 的页面，关于其不满足自动生成条件的场景，在 C++11 前，编译器的处理为不定义；在C++11 及之后，编译器的处理为将其定位为 deleted。
使上述例子通过 cppinsight[^2]修改编译的 C++标准验证了一下，得到的报错也不一样，以 copy assignment operator 为例，C++11之前报错 cannot define the implicit copy assignment operator，C++11 及之后报错 copy assignment operator is implicitly deleted）


>Things to Rember
>- Compilers may implicitly generate a class's default constructor, copy constructor, copy assignment operator, and destructor.
>  编译器可能会隐式生成类的默认构造函数，拷贝构造函数，拷贝赋值函数，析构函数。

[^1]: https://en.cppreference.com/w/cpp/language/copy_assignment
[^2]: https://cppinsights.io/
2025.01.06