C++ 在初始化上看起来相当反复无常，在不同的上下文中，对象被保证初始化或并不保证。
虽然有一些规则用来描述对象哪种情况被保证进行初始化，哪种不保证。但这些规则过于复杂。
通常而言：
- 如果你使用的是 C++ 中的 C 部分（见[[Item_1_View_C++_as_a_federation_of_languages]]]]），并且初始化会导致运行期成本，那么就不保证初始化。
- 如果你使用的是 C++ 中 C 部分以外的部分，那么就会保证初始化。

这也解释了为什么 array（C part of C++）不被保证初始化，而 vector（STL part of C++）保证初始化。

**但处理这种不确定事务的最好方式就是在使用对象前进行初始化。**
## built-in type
对于没有成员的内置类型（built-in type），我们手动进行初始化：
~~~cpp
int x = 0;
const char *text = "A C-style string";
double d;
std::cin >> d;
~~~
## constructor
对于内置类型以外的其他类型，初始化的责任则在构造函数身上。
规则很简单：**确保所有构造函数对初始化对象的所有成员。**
### assignment、initialization 赋值与初始化的区别
首先辨析一下 assignment 和 initialization 的区别：
~~~cpp
class PhoneNumber {...};

class ABEntry {
public:
    ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones);

private:
    std::string theName;
    std::string theAddress;
    std::list<PhoneNumber> thePhones;
    int numTimesConsulted;
}

ABEntry::ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones) {
    // 下面函数体内的语句，都是 赋值，而不是 初始化
    theName = name;
    theAddress = address;
    thePhones = phones;
    numTimesConsulted = 0;
};
~~~
如上实例，构造函数的函数体中语句全为赋值，而非初始化。
尽管这样也能够使得 ABEntry 对象实例的值是我们所期望的值，但这并不是最佳做法。

**C++ 规定，对象的数据成员在进入构造函数的函数体之前已被初始化。**
因此，在 ABEntry 的构造函数中，theName，theAddress，thePhones 并不是初始化，而是赋值。
初始化发生在更早——在进入 在 ABEntry 的构造函数的函数体之前，它们（theName，theAddress，thePhones）的 default 构造函数就被调用了。同时，这句话对 numtimesConsulted 不为真，因为如本文开篇所言， numtimesConsulted 是内置类型，本身就不保证在赋值前会对它进行初始化。

### initialization list 初始化列表
更好的方法是使用初始化列表代替赋值：
~~~cpp
ABEntey::ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones) 
: theName(name)
  the Address(address)
  thePhones(phonoes)
  numTimesConslted(0)
{}
