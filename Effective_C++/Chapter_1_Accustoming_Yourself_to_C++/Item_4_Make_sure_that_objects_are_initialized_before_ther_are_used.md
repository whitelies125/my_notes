# 条款 4 确定对象在使用前已先被初始化
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
## 初始化顺序
### 类成员的初始化顺序
C++ 对于对象的数据初始化顺序方面，是固定不变的、相同的。其顺序总是
1. 基类（base clase）先于派生类（derived class）（详见[[Item_12]]）
2. 在类中，data member 的初始化顺序即它们的声明顺序
因此，即便我们可以按任意顺序在 member initialization list 中列出 data member，但 data member 实际上的初始化顺序仍然是按照上述原则执行的。
为了避免阅读时产生疑惑，同时也为了避免一些难以察觉的行为 bug，例如：
~~~cpp
class Cls {
public:
    int a;
    int b;
    // 容易让人误以为 b 先初始化为 1，随后 a 初始化为 b 的取值 1
    // 实际上的初始化顺序是先 a 后 b
    Cls(): b(1), a(b) {} 
};

int main() {
    Cls cls;
    std::cout << cls.a << std::endl; // 内存垃圾值，不一定
    std::cout << cls.b << std::endl; // 1
    return 0;
}

~~~

因此，**最好总是以声明顺序为 member initialization list 顺序**。

### 不同编译单元中 non-local static 对象的初始化顺序
先理清概念：
#### 编译单元
编译单元（translation unit）指产出单一目标文件（object file）的源码，基本上是指一个源文件及其所有 \#include 的文件。
#### non-local static object
static object 指存在期为从被构造到整个程序结束的对象，它们在程序退出时被摧毁，即它们的构造函数在 mian 函数结束时执行。
其包含：
- globla 对象
- 定义在 namaspace 作用域内的对象
- 定义在 class 内声明为 static 的对象
- 定义在 function 内声明为 static 的对象
- 定义在 file 作用域内声明为 static 的对象（也就是源文件中 static 修饰的对象）。
而不包含栈和堆上的对象。

function 内声明为 static 的对象称为 local static object，因为它们对于函数来说是 local 的。
其他类别的 static object 则称为 non-local static object。

#### 问题场景
我们在两个编译单元中，各有至少一个 non-local static object，通常而言这并无大碍。
但如果编译单元 A 中的 non-local static object（Obj1），其初始化使用了编译单元 B 中的 non-local static object（Obj2），则此时 Obj2 可能尚未初始化。
这是因为 **C++ 对于定义在不同编译单元中的 non-local static object 初始化相对顺序并没有明确定义**。

~~~cpp
// file_system.h
class FileSystem {
private:
    std::size_t numDisks_;
public:
    FileSystem(std::size_t n) : numDisks_(n) {}
    std::size_t numDisks() const {
        return numDisks_;
    }
};
extern FileSystem tfs;

// file_system.cpp
#include "file_system.h"
FileSystem tfs {10};

// directory.h
#include "file_system.h"
class Directory {
public:
    std::size_t disks;
    Directory(std::size_t n) : disks(n){
        std::size_t disks = tfs.numDisks();
    }
};
extern Directory dir;

// directory.cpp
#include "directory.h"
Directory dir(1);
~~~
如上示例，除非 tfs 在 dir 之前被初始化，否则 dir 的构造函数就会使用到尚未初始化的 tfs。
而 tfs 和 dir 是定义在不同的编译单元的 non-local statis object，其初始化顺序是无法保证的。
#### 解决方法 non-local static -> local static
将每个 non-local static object 搬到专属它的函数内，仍使用 static 进行修饰，即将其变为 local static object。该函数返回对该对象的引用，用户使用该对象的方式变为调用函数获取，而非此前的直接操作。
这实则也就是单例（signleteon）模式的一个常见实现方式。

这个方法是基于 **C++ 保证了对于 local static object，在该函数被调用时首次遇到该 object 的定义时进行初始化**。
因此，使用「调用返回 local static object 引用的函数」替代「直接访问 non-local static object」，就能够保证获得得引用总是一个已经初始化了的 object。
甚至，额外地，倘若你从未调用过该函数，就绝不会引发构造和析构其中 local static object 的开销，相比 non-local static object，这种方式要好很多。
修改后代码如下所示：

~~~cpp
// file_system.h
class FileSystem {
private:
    std::size_t numDisks_;
public:
    FileSystem(std::size_t n) : numDisks_(n) {}
    std::size_t numDisks() const {
        return numDisks_;
    }
};
FileSystem& tfs();

// file_system.cpp
#include "file_system.h"
FileSystem& tfs() {
    static FileSystem tfs {10}; // 仅在第一次调用时进行初始化
    return tfs;
}

// directory.h
#include "file_system.h"
class Directory {
public:
    std::size_t disks;
    Directory(std::size_t n) : disks(n){
        // 调用 tfs() 获得的引用已确保一定是以初始化的对象
        std::size_t disks = tfs().numDisks(); 
    }
};
Directory& dir();

// directory.cpp
#include "directory.h"
Directory& dir() {
    Directory dir(1); // 仅在第一次调用时进行初始化
    return dir;
};
~~~
此外，采用这个方案的返回 local static object 引用的函数往往非常简单：第一行定义和初始化 local statis object，第二行返回其引用。
因此非常适合作为 inline 函数（详见[[Item_30]]），尤其是当它被频繁调用时。

从另一个视角来看，内含 local static object 的函数在多线程场景下可能会引发问题。（因为可能多个线程**同时首次**访问该函数，导致对 local static object 进行了多次构造，进而造成内存泄漏等其他异常情况）

解决多线程多次构造问题的一个方式是，在程序的单线程启动阶段手动调用所有的返回 local static object 引用的函数（也就是在程序只有一个线程的时候就手动完成所有初始化），从而消除与初始化相关的 race conditon（竞态条件[^1]）。

>race conditon
>A race condition or race hazard is the condition of an electronics, software, or other system where the system's substantive behavior is dependent on the sequence or timing of other uncontrollable events, leading to unexpected or inconsistent results. It becomes a bug when one or more of the possible behaviors is undesirable.
>竞争条件或竞争危险是指电子设备​​、软件或其他系统的一种情况，即系统的实质性行为取决于其他不可控事件的顺序或时间，从而导致意外或不一致的结果。当一个或多个可能的行为不合要求时，就会出现错误。
>
>A race condition can arise in software when a computer program has multiple code paths that are executing at the same time.
>当计算机程序有多个同时执行的代码路径时，软件中可能会出现竞争条件。

这里的 race condition 就是说，进程或线程并发执行时，其最终的结果依赖于进程或者线程执行的精确时序，称为存在竞态条件。当一个计算机程序有多个同时执行的代码路径时，软件中就会出现竞态条件。

（个人注：书太老了，**在 C++11 后，对 local static object 的构造已保证是线程安全的**[^2]，多线程同时调用同一函数时，编译器会确保 local static object 初始化只发生一次。）

事实上，任何类型的 non-const static object——无论是 local 还是 non-local 的——在多线程环境中存在「等待某事发生」的操作，都使其成为潜在的隐患。

---

总而言之，为了避免在对象初始化前使用它们，你需要做三件事：
- 手动初始化 built-in 类型。
- 使用 member initialization list 初始化对象的所有部分。
- 针对定义在不同编译单元中的 non-local static object 初始化顺序不确定性进行设计。

>Things to Remember
>- Manually initialize object of built-in type, because C++ only sometimes initialize them itself.
>  手动初始化 built-in 类型，因为 C++ 不保证一定初始化它们
>- In a constructor, prefer use of the member initialization list to assignment inside the body of the constructor. List data members in the initialization list in the same order they're declared in the class.
>  在构造函数中，最好使用 member initialization list 而不是在构造函数体内赋值。在 member initialization list 中按 data member 的声明顺序列出。
>- Avoid initialization order problems across translation units by replacing non-local static objects with local static objects.
>  以 local static objects 代替 non-local static object 的方式，避免跨编译单元的初始化顺序问题。

[^1]: https://en.wikipedia.org/wiki/Race_condition
[^2]: https://stackoverflow.com/questions/1661529/is-meyers-implementation-of-the-singleton-pattern-thread-safe

2025.01.05