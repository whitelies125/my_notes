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

**C++ 规定，对象的 data member 在进入构造函数的函数体之前已被初始化。**
因此，在 ABEntry 的构造函数中，theName，theAddress，thePhones 并不是初始化，而是赋值。
初始化发生在更早——在进入 在 ABEntry 的构造函数的函数体之前，它们（theName，theAddress，thePhones）的 default 构造函数就被调用了。同时，这句话对 numtimesConsulted 不为真，因为如本文开篇所言， numtimesConsulted 是内置类型，本身就不保证在赋值前会对它进行初始化。

### initialization list 初始化列表
更好的方法是使用 member initialization list（成员初始化列表）代替赋值：
~~~cpp
ABEntey::ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones) 
: theName(name)  // 及以下语句都是 初始化，而不是 赋值
  theAddress(address)
  thePhones(phonoes)
  numTimesConslted(0)
{}
~~~
与前文的构造函数相比，这个构造函数的结果与之的相同，但效率更高。
前者先是调用 default 构造函数对 theName，theAddress，thePhones 进行初始化，然后立即在 default 构造的值上赋予新的值。default 构造函数的做的事全被浪费了。
而 member initialization list 避免了这个问题。
因为 initialization list 中的参数用于了成员变量的构造函数参数。在该例中，theName，theAddress，thePhones 都是各由 name，address，phones 进行拷贝构造而来。

对大部分类型而言，一次拷贝构造的调用是更有效率的，相比先调用 default 构造函数再调用拷贝复制操作符。
对于类成员 built-in 类型变量而言，如 numTimesConsulted，二者的代价则没有不同。但为了**保持一致性**，最好也将其写在 initialization list 中。

当然，member initialization list 中也可以调用 default 构造函数，不提供参数就行：
~~~cpp
ABEntey::ABEntry(const std::string &name, const std::string &address, const std::list<PhoneNumber> &phones) 
: theName()  // call default constructor
  theAddress()
  thePhones()
  numTimesConslted(0)
{}
~~~

因为对于未在 member initialization list 中的  user-defined 类型 data member，编译器也会自动地调用其 default 构造函数，所以一些程序员觉得上述写法比较繁琐、多此一举，是可以省去 theName()，theAddress()，thePhones()的。
这种看法当然是可以理解的，但**始终保持将所有的 data member 都放在 initialization list 的策略**，可以免除我们需要记忆哪些 data member 已初始化/未初始化，降低了程序员的心智负担。
例如，如果采用可以在 member initialization list 不列全所有 data member 的写法，一旦不小心遗漏了如 numTimesConslted 这样的 built-in 类型 data member，那么 numTimesConslted 就遗漏了初始化，后续对其进行读写就可能引发 undefined behavior。

一些场景下，即便是 built-in 类型，也必须使用 initialization list。
例如，data member 是 const 或 reference。因为它们不能被赋值[[Item 5]]，自然也无法在函数体中通过赋值的方式置为想要的值。
类似的，为了避免记住哪些数据成员必须在初始化列表中初始化，哪些是可选的，降低心智负担，最简单的做法是**始终使用 initialization list**。因为它有时是必须的，而且往往比赋值更高效。

许多 class 拥有多个 constructor，每个 constructor 有自己的 member initialization list。这也就导致在不同的 member initialization list 中存在不必要的重复。
这种场景下，我们可以合理地将某些赋值操作与初始化表现一样好的 data member 在 member initialization list 省略，改用赋值操作，更好的是将其赋值移动到单独的函数中，供所有构造函数都调用。
这种情况比较适用于初值是由文件或数据库读入的情况。当然通常情况下还是使用初始化列表中对变量初始化这种操作更为可取。
（个人注：详见[[Item_2_Prefer_consts_enums_and_inlines_to_defines#个人注：#C++11 non-static in-class member initialization]]中的引用 2，从 C++11 起又有了一种解决方式。）