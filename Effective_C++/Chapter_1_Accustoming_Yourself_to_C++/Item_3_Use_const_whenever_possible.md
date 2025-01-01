`const` 允许程序员明确指出一个语义上的约束：这个对象不能被修改。
编译器则会实施这个约束。

`const` 用法非常灵活：
- 可用于修饰全局或 namespace 作用域内的变量
- 可用于文件中、函数中、区块作用域（block scope） 中的 static 对象
- 可用于修饰类中的的 static 或 non-static 数据成员皆可
- 可用于指明指针自身是常量，指针指向对象是常量，或二者皆为常量。
## const 与指针
虽然 const 用法灵活，但其修饰规则很简单：
- 若 const 在星号左边，则指向的对象是常量
- 若 const 在星号右边，则指针自身是常量
- 若 const 在星号两侧，则二者都是常量。

```cpp
char greeting[] = "Hello";
char* p = greeting;             // non-const pointer, non-const data
const char* p = greeting;       // non-const pointer, const data
char* const p = greeting;       // const pointer, non-const data
const char* const p = greeting; // const pointer, const data
```

因此，下面两个变量类型其实是相同的，const 都在星号左侧：
```cpp
const int* num;
int const * num;
```
## const 与 STL
STL 是基于指针建模的，iterator 的行为就类似于 T* 指针。
但需要**注意**的是：声明一个 const iterator 实际上是类似于声明一个 T\* const 指针（指针自身为常量），而不是 const T\* 指针（所指对象为常量）
即 const iterator 是 iterator 不可变，所指对象可变；STL 提供的 const_iterator 才是 iterator 可变，所指对象不可变。
```cpp
std::vector<int> vec;
const std::vector<int>::iterator iter = vec.begin(); // iter 不可变，所指对象可变
*iter = 10;                                          // ok，所指对象可变
++iter;                                              // error，本身不可变

std::vector<int>::const_iterator cIter = vec.begin();// cIter 可变，所指对象不可变
*cIter = 10;                                         // error，所指对象不可变
++cIter;                                             // ok，本身可变

const std::vector<int>::const_iterator ccIter = vec.begin() // ccIter 不可变，所指对象不可变
*ccIter = 10;                                         // error，所指对象不可变
++ccIter;                                             // error，本身不可变
```
## const 与函数

const 最具威力的用法是用于函数声明。在函数声明中，const 可用于修饰函数返回值、形参、函数本身（对于成员函数）
### const 与函数返回值
使函数返回常量，可以减少错误发生同时又不失去安全性与效率。
考虑以下例子：
~~~cpp
// rational.h
class Rational {...};
const Rational operator*(const Rational& lhs, const Rational& rhs);

// main.cpp
Rational a, b, c;
if (a * b = c){ // 也许是想写 == 但是写错了。
    ...
}
~~~
在这种情况下，若 operator* 返回的 non-const，则回认为可赋值，因此不会报错；而返回的是 const，编译器则会给出错误提示，从而避免这类错误。

（个人注：现如今（2024.12.27），这种判断语句中少写等号的错误，编译器或代码检查工具都会提示了。当然这里并不是反驳，只是单纯吐槽这个例子也是有点老了。）
### const 与成员函数
使用 const 修饰成员函数的目的是指明哪些成员函数可以被 const 对象调用。
这类成员函数出于以下两种原因非常重要：
1. 这使得类的接口更易懂。
   因为让人知道哪些函数会修改对象，而哪些不会是非常重要的。
3. 这使得我们可以操作 const 对象。
   如[[Item_20]]所说，一个提升 C++ 编程性能的基本方式是以 reference-to-const 传递对象。而这个技术的前提是我们有 const 成员函数来操作 const 修饰的对象。

**注意**：不同点仅在于是否被 const 修饰成员函数，是重载函数。
考虑以下例子：
~~~cpp
// textBlock.h
class TextBolck {
public:
    ...
    const char& operator[] (std::size_t position) const { // const 对象调用
        return text[position];
    }
    char& operator[] (std::size_t position) { //  non-const 对象调用
        return text[position];
    }
private:
    std::string text;
};

// main.cpp
TextBolck tb ("Hello")
std::cout << tb[0]; // 调用无 const 修饰的 operator[]
const TextBolck ctb ("World");
std::cout << ctb[0]; // 调用 const 修饰的 operator[]
~~~
由此说明，通过 const 修饰进行重载，可对 const 对象和 non-const 对象提供了不同的处理。
#### 关于 const 成员函数到底 const 了个啥
对于 const 成员函数的理解，分为了两派：
1. bitwise constness：
   const 成员函数不应该修改所处理对象的任何数据成员（除了 static 数据成员外）
   **这同时也是 C++ 对常量的定义**
2. logical constness：
   const 成员函数可以修改它处理的对象中的某些 bit，但仅当用户感知不到时才允许。
##### Bitwise Consness
使用 bitwise constness 的定义对于编译器来说也是极为方便的，编译器只需在函数中寻找是否有对类（non-static 除外）的数据成员赋值的操作，即可判定该const 成员函数是否违背了 bitwise constness。
实际上，**bitwise constness 也正是 C++ 对常量性（constness）的定义**，const 成员函数不可更改类（non-static 除外）的数据成员。

然而设想以下场景，如果对象的成员变量为指针类型，该指针保存指向某一对象的地址且所指对象并非指针所属对象的成员，则当通过函数修改所指对象时，所属对象仍然是符合 bitwise constness 定义的。
更详细地说明：const 对象 A 的成员变量指针 ptr 指向对象 B，在对象 A 的成员函数 func 中通过 ptr 修改对象 B 时，对象 A 是符合 bitwise constness 的，因此，将该函数修饰为 const 也是正确无误的。如下例子中的 func() 函数。
~~~cpp
class Cls {
private:
    int* const ptr;
public:
    Cls(int* n) : ptr(n) {}
    // 修改的内存并不是 cls对象成员, 因此该成员函数符合 C++ 对 constness 的定义（bitwise constness)
    // 则使用 const 修饰该成员函数无误，编译器不会报错
    void func(int n) const {
        *ptr = n;
    }
};

int main() {
    int n = 0;
    const Cls cls (&n);
    cls.func(1); // 调用 const 修饰的 func 函数，无报错
    return 0;
}

~~~

又设想，假定 const 成员函数返回成员指针所指地址的 non-const 引用（详见[[Item 28]]，由于在 const 成员函数中并未对对象内部数据有修改，编译器认为其是 bitwise constness，使用 const 修饰并无问题，但实际上对象内部数据仍可能被外部修改。如下例子中的 operator[] 函数。
~~~cpp
class CTextBlock {
private:
    char *pText;
public:
    CTextBlock(char* ptr) : pText(ptr) {}
    // 返回所指内存的引用，仍符合 C++ 对 constness 的定义（bitwise constness)
    // 则使用 const 修饰该成员函数无误，编译器不会报错
    char& operator[] (std::size_t position) const {
        return pText[position];
    }
};

int main() {
    std::string str {"Rest"};
    const CTextBlock cctb (str.data());
    char* pc = &cctb[0];
    char& ch = cctb[0];
    *pc = 'S';
    ch = 'T';
    std::cout << str << std::endl;
    return 0;
}
~~~

在上述两个例子中，尽管我们创建的是 const 对象，调用的也是 const 成员函数，但实际上我们还是修改了一些数据的值。
而**大部分场景下，其实我们是抱有着不应修改的期望**。例如：
~~~cpp
int main() {
    std::string str {"Rest"};
    const CTextBlock cctb (str.data());
    if (cctb[0] = U ) { // 返回 char& 会导致判断为 true（虽然这个例子也并不非常好罢）
        ...
    }
    std::cout << str << std::endl;
    return 0;
}
~~~

##### Logical Constness
因此引入了第二种定义，logical constness。
logical constness 认为 const 成员函数可以修改所处理对象的某些 bit，但仅当用户感知不到时才允许。

如以下示例，假定我们需要提供 length() 函数以返回字符串长度：
~~~cpp
class LogicalCTextBlock {
private:
    char *pText;
public:
    LogicalCTextBlock(char* ptr) : pText(ptr) {}
    std::size_t length() const {
        return std::strlen(pText); // 每次调用都需要重复计算长度
    }
};
~~~

在频繁调用的场景下，这样的实现每次都需要计算一次字符串长度，性能不佳。
因此，为了提高程序性能，我们在类中添加 textLength 成员作为 pText 的缓存，当缓存有效时，返回缓存，无效时才计算字符串长度：
~~~cpp
class LogicalCTextBlock {
private:
    bool lengthIsValid;
    std::size_t textLength;
    char *pText;
public:
    LogicalCTextBlock(char* ptr) : pText(ptr) {}
    std::size_t length() const {
        if (!lengthIsValid) {
            textLength = std::strlen(pText); // 编译器报错，不允许 const 成员函数对处理对象的成员进行修改
            lengthIsValid = true; // 同上
        }
        return textLength;
    }
};
~~~

这种实现尽管不是 bitwise constness 的，但从 logical constness 观点看来是自然的、方便的、正确的。
length() 允许 lengthIsValid 和 textLength 可被修改这样的实现是可接受的，无非是在 bitwise constness 的是实现上添加了额外的变量进行性能优化，pText 本身也并没有被修改，这一点实质上是与 bitwise constness 一致的。

然而这样的写法会因为 C++ 坚持 bitwise constness 而不允许编译通过。
因此 C++ 提供了一种解决方法，通过 **mutable** 关键字修饰成员变量，从而告知编译器允许这些变量总是可被修改的。
~~~cpp
class LogicalCTextBlock {
private:
    mutable bool lengthIsValid;
    mutable std::size_t textLength;
    char *pText;
public:
    LogicalCTextBlock(char* ptr) : pText(ptr) {}
    std::size_t length() const {
        if (!lengthIsValid) {
            textLength = std::strlen(pText); // 添加 mutable 后无报错
            lengthIsValid = true; // 同上
        }
        return textLength;
    }
};
~~~

另一个更易懂的例子是：
~~~cpp
class BitwiseCMapFind {
private:
    std::map<int, int> m;
public:
    const std::map<int, int>::const_iterator get(const int key) const {
        return m.find(key); // 每次调用都需要搜索
    }
};

class LogicalCMapCacheFind {
private:
    mutable std::map<int, int>::const_iterator cache; // 缓存上一次的查询结果
    std::map<int, int> m;
public:
    const std::map<int, int>::const_iterator get(const int key) const {
        // cache 命中时，直接返回结果，省去在 map 中搜索的性能开销
        if (cache->first != key) {
            const auto itor = m.find(key);
            if (itor != m.end()) {
                cache = itor; // 若不以 mutable 修饰，则编译器会报错，因为 const 成员函数不能对处理对象的成员进行修改
            }
        }
        return cache;
    }
};

~~~
（个人注：总结，C++ 对 constness 的定义是 bitwise constness，提供了 mutable 关键字供用户实现 logical constness。
我们在写代码时，需要了解到编译器是使用 bitwise constness 的判别逻辑对 const 成员函数进行检查，但 const 修饰的成员函数如上文所述，是有可能对内存进行修改的（修改指针指向的不归属所处理对象的内存）。
我们的代码实现原则应该也往往按照 logical constness 来实现更好。）
### 避免 const 与 non-const 成员函数重复代码

出于某些原因，我们的代码中可能出现 const 和 non-const 成员函数实则为等价逻辑、重复代码的现象。
考虑以下示例：

~~~cpp
class TextBlock {
private:
    std::string text;
public:
    const char& operator[] (std::size_t position) const {
        ... // 边界检查
        ... // 访问日志
        ... // 数据完整性校验
        return text[position]
    }
    char& operator[] (std::size_t position) {
        ... // 边界检查
        ... // 访问日志
        ... // 数据完整性校验
        return text[position]
    }
}
~~~

这里我们假设上文提到过的 operater[] 函数不止提供返回字符的引用，而是包含了边界检查、访问日志、数据完整性校验等等操作，
出于提供只读访问和读写访问两种接口的原因，我们使其中一个成员函数提供只读访问，另一个则可以读写。
因此也就导致了存在两个函数 const 与 non-const，但二者除了返回值不同外，在其函数体的代码实现上，是完全一致的重复代码。

第一反应是将重复代码提取为公共（成员）函数，在 const 和 non-const 函数都调用它即可：
~~~cpp
class TextBlock {
private:
    std::string text;
    void publicFunc() const {
        ...
    };
public:
    const char& operator[] (std::size_t position) const {
        publicFunc()
        return text[position]
    }
    char& operator[] (std::size_t position) {
        publicFunc()
        return text[position]
    }
}
~~~

这种方式固然可行，但实际上我们还是重复了调用公共函数、return 语句这两行代码：
~~~cpp
        publicFunc()
        return text[position]
~~~

更好的做法是一次实现，两次调用，即使用 non-const 成员函数调用 const 成员函数：
~~~cpp
class TextBlock {
private:
    std::string text;
public:
    ...
    const char& operator[] (std::size_t position) const {
        ... // 边界检查
        ... // 访问日志
        ... // 数据完整性校验
        return text[position]
    }
    char& operator[] (std::size_t position) {
        return const_cast<char &>(static_cast<const TextBlock&>(*this)[positon])
    }
}
~~~
第一步，使用 static_cast 将 TextBlock& 转换为 const TextBlock& 类型，从而调用 const operator[] 函数。若不进行转换，则调用的仍然是 non-const operator[] 函数，会发生无限地递归调用。
第二步，使用 const_cast 移除 const operator[] 函数返回值的 const 属性。

这样也就没有了重复代码。

另请**注意：只能够由 non-const 函数调用 const 版本的函数，而不能反过来。**

>Thing to Remember
>1. Declaring something const helps compilers detect usage errors. const can be applied to objects at any scope, to function parameters and return types, and to member functions as a whole.
>   声明为 const 帮助编译器发现错误用法。const 可用于作用域内的任何对象，函数参数，函数返回值，成员函数本身。
>3. Compilers enforce bitwise constness, but you should program using logical constness
>   编译器强制实施 bitwise constness，但你编程是应使用 logical constness。
>5. When const and non-const member functions have essentially identical implementations, code duplication can be avoided by having the non-const version call the const version.
>   当 const 和 non-const 成员函数有着本质上等价的实现时，通过 non-const 版本调用 const 版本的函数可以避免代码重复。

2024.12.29