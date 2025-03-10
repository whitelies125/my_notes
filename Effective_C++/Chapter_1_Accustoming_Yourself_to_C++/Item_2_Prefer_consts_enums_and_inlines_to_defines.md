# 条款 2：尽量以 const，enum，inline 替换 \#define

>This Item might better be called "prefer the compiler to the preprocessor,“ because \#define may be treated as if it's not part of the language per se. That's one of its problems.
>这个条款也许称为 “使用编译器而不是预处理器” 更好，因为 \#define 可能不被视为语言的一部分。这正是使用 \#define 的问题所在。

例如，使用
```C++
#define ASPECT_RATIO 1.653
```
ASPECT_RATIO 这个符号名很可能在预处理时就已经替换为 1.653，并没有进入符号表（symbol table）。
你可能会因编译错误信息出现的 1.653 感到困惑，由于不知道其来源，以至于浪费定位时间。
# 解决方法
## 使用常量代替宏
```cpp
const double AspectRatio = 1.653;
```
AspectRatio 作为语言常量，是肯定能被编译器看见的，也当然会加入到符号表。
此外，使用常量代替 \#define 也可能使代码更小，因为预处理器在多处以 1.653 替换 ASPECT_RATIO 时，可能导致目标代码中存在多个 1.653。而使用常量 AspectRatio 则始终只有一个 AspectRatio。

（个人注：现如今对于编译期常量（compile-time constant）推荐使用的是 constexpr。
但考虑到本书成文较早，初版在 05 年出版，而 constexpr 是 11 年的 C++11 引入的，没有 constexpr 内容也属正常。
至于后续全文提到的常量，我也就默认地认为作者指的**都是 const 而不是 constexpr**。）

> Things to Remember
> - For simple constants, prefer const objects or enums to \#defines
>   对于简单的常量，最好使用 const 对象或 enum 而不是 \#define
> - For function-like macros, prefer inline functions to \#defines.
>   对于形式函数的宏，最好使用 inline 函数而不是 \#define
## 使用场景
当使用常量代替宏时，有两个特殊场景尤其需要注意。
### 常量指针（constant pointer)
除了将指针指向的对象声明为 const 外，也需将指针本身声明为 const，防止别人在使用时将指针指向其他对象。
```cpp
const char* const authorName = "Scott Meyers";
```
对于 const 意义与使用的完整讨论，详见 [[Item_3_Use_const_whenever_possible]]。
另外，值得一提的是，string 对象通常比 char*-base 更好用，因此上述定义也可写为：
```cpp
const std::string authorName("Scott Meyers")
```
### 类专属常量（class-specific constant)

>To limit the scope of a constant to a class, you must make it a member, and to ensure there’s at most one copy of the constant, you must make it a static member:
>对于一个类，为了限制一个常量的范围，你必须使其为类的成员；且为了确保其只有一份实体，你必须使其为 static 成员（个人注：即使多个类的实例使用同一个 static 成员。）
```cpp
// gameplayer.h
class GamePlayer {
private:
    static const int NumTurns = 5; // constant declaration
    int scores[NumTurns];          // use of constant
};
```
注意，这里的 NumTurns 是声明（declaration）而不是定义（definition）。

通常而言，C++ 要求你对你使用的所有东西提供一个定义（详见 ODR[^1]，One Definition Rule）。
但如果它：
- 是类专属常量（即是类的成员，且为常量）
- 是 static
- 是整数类型（integral type，如 integer，char，bool）
则是例外。
只要你**不对其取地址**，那么就可以**仅声明且使用它，而无需提供定义**。
（个人注：为什么此处不需要提供定义，实际上有些情况是需要提供定义的，详见后文：[[#为什么 static non-const 成员不能 in-class initialization]]

如果你对其取地址，或你没有对其取地址但你所使用的编译器（错误地）要求一个定义，则可在实现文件中提供一个分离的定义：
```cpp
// gameplayer.cpp
const int GamePlayer::NumTurns; // definition of NumTruns，no value is given
```
且由于在声明中已经提供了初始值，NumTruns 在声明时被初始化为5，因此在定义的地方不需要，也（因 const 修饰）不能再次赋值。
#### 对比 \#define 和 const 修饰的数据成员。
**\#define：**
与作用域无关，只要宏被定义，之后的编译除非有该宏的 \#undef，则其一直有效。
这意味着 \#define 不仅不能用于 class-specific constant，也不能提供任何封装（encapsulation），也就是说，不存在 private \#define 这样的东西。

（个人注：即使将 \#define 写在 private 而不是 public 中，也无用，预处理时仍旧直接将宏替换，可在类外直接使用宏。因此 \#deinfe 无法提供封装，而封装，对于面向对象编程（OOP，object-oriented programming) 是相当重要的。）

**const 的类数据成员：**
则可被封装，能提供封装。
#### 不支持 static 常量整数类型成员 in-class initialization 的解决方法

一些比较老的编译器可能不支持上述语法：在声明的地方提供初始值。
并且，in-class 初始化（in-class initialization，即上述语法，在声明时提供初始值）也仅允许对 integral type 使用。
##### 不使用 in-class initialization
如果你遇到上述语法无法使用的场景，可以将初始值放在定义的地方即可：
```cpp
// cost_estimate.h
class CostEstimate {
private:
    static const double FudgeFactor;
    ...
};

// cost_estimate.cpp
const double CostEstimate::FudgeFactor = 1.35
```
这实际上也就是声明与定义分离，不使用 in-class initialization 了。
##### enum hack
一些情况下，我们必须要在声明式中获得初始值（如上文中的 NumTurns，由于作为了数组 sores 的长度，被要求是编译期常量），但编译器错误地不支持 static 整数类型常量成员 in-class initialization 特性，也可使用称为 enum hack 的技巧：
```cpp
class GamePlayer {
private:
    enum { NumTurns = 5}；
    int scores[NumTurns];
}
```
（个人注：强枚举类型在 C++11 才引入，所以书这里只是普通枚举类型吧。）

 出于以下愿意，值得我们对 enum hack 进行一定了解：
- enum hack 的行为某种程度上更贴近 \#define 而非 const，有时候我们会更想要这类特性。
  比如：我们不能获取 enum 的地址。当我们不想让别人通过指针或引用来指向你的某个整数常量时，使用 enum 可以实现这个约束（关于约束条件，更多详见[[Item 18]]；
- 此外，虽然大部分实现优秀的编译器不会为整数型 const 对象分配额外的存储空间（除非你创建指针或引用指向它）；但部分编译器可能会这样，这就造成了额外的空间浪费。在这一点上使用 enum 和 \#define 都可以避免出现额外分配空间。
- 从功利主义的思想来看，很多代码都会使用 enum，所以我们也必须要去学会如何正确使用它。
#### inline 替代 \#define 实现的函数
使用 \#define 定义宏以模仿函数时，固然可以省去函数调用的开销，但极易出错：
```cpp
// 定义宏，传入 a 与 b 的较大值调用函数 f
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

int a = 5, b = 0;
CALL_WITH_MAX(++a, b);    // a 被累加两次
CALL_WITH_MAX(++a, b+10); // a 被累加一次
```
使用 template 和 inline（详见[[Item 30]]）可方便的代替上述技巧：
```cpp
template<typename T>
inline void callWithMax(const T &a, const T &b){
    f(a > b ? a : b);
}
```
### 个人注：
##### C++11 non-static in-class member initialization
C++11 支持了对 non-static 变量的 in-class initialization[^2]，实则为构造函数的语法糖[^2][^3]。

>[^2]
>In C++98, only `static const` members of integral types could be initialized in-class, and the initializer has to be a constant expression.
>在 C++98，仅有 `static const` 的整数类型成员可 in-class 初始化，且其 initializer 必须为常量表达式。
>
>The basic idea for C++11 was to allow a non-static data member to be initialized where it is declared (in its class)
>C++11 的基本思想是，允许 non-static 的数据成员在（它的类中）声明的地方被初始化。

但与类的 static 成员的 in-class initialization **并不相同**，只是形式上相似，所以这里提一下。

类的 static 成员仍然需是 const 或 constexpr（constexpr 隐含了 inline）才可 in-class initialization。

##### 为什么 static non-const 成员不能 in-class initialization
根据 C++ 声明与定义的划分[^1]：
```cpp
class S {
    static int a;     // 这是声明
    static int b = 1; // 这是定义
}
```

>[^1]
>One and only one definition of every non-inline function or variable that is odr-used (see below) is required to appear in the entire program (including any standard and user-defined libraries). 
>在整个程序（包括所有的标准或用户定义的程序库）中，被 odr-use（见下文）的非内联函数或变量只允许有且仅有一个定义。

可见，对于类的 static non-const 成员 `b`，如果允许其 in-class initialization，那么此处实则为 `b` 的定义式而不是声明式。
因此，当多个翻译单元包含该头文件时，`b` 就会在多个翻译单元中存在定义，即在整个程序中存在多处定义。

>[^1]
>ODR-use
>Informally, 
>- an object is odr-used if its value is read (unless it is a compile time constant) or written, its address is taken, or a reference is bound to it, 
>  如果一个对象的值被读取（除非它是编译期常数）或写入，或取其地址，或者被绑定引用，那么这个对象就是 odr-used 的
>- a reference is odr-used if it is used and its referent is not known at compile time, 
>  如果一个引用被使用，而它引用的对象在编译时是未知的，那么这个引用就是 odr-used 的
>- a function is odr-used if a function call to it is made or its address is taken.
>  如果一个函数被调用，或它的地址被获取，那么这个函数就是 odr-used 的。
>- If an object, a reference or a function is odr-used, its definition must exist somewhere in the program; a violation of that is usually a link-time error.
>  如果对象、引用或函数是 odr-used 的，则其定义必须存在于程序的某个地方；违反这一规定通常会导致链接时错误。

因此，我**个人理解**，这种情况下 `b` 往往是 odr-use 的（毕竟如果完全不使用 b，那也没必要定义一个 `b` 出来了），因此极易违背 ODR，导致出现链接错误，所以不允许其直接 in-class initialization。

前文的 `static const int NumTurns = 5` 之所以不需要提供定义，是因为它不是 ODR-use（是编译器常量，不会被写入，没有被取地址，没有被绑定引用），所以不需要提供定义。
但如果 `NumTurns` 满足了 ODR-use，例如被取地址或被绑定引用，仍然需要提供一个定义。
```cpp
// gameplayer.h
class GamePlayer {
private:
    static const int NumTurns = 5;
    int scores[NumTurns];
};

// main.cpp
#include "gameplayer.h"
int main() {
    // 对其绑定引用，此时为 ODR-use
    // 该场景下编译，链接时会报错：undefined reference to `GamePlayer::NumTurns'
    const int& i = GamePlayer::NumTurns;
}
```
提供一个定义即可编译通过：
```cpp
// gameplayer.cpp
const int GamePlayer::NumTurns;
```
##### C++17 inline static
C++17 支持了使用 inline 修饰变量（内联变量，inline variable)[^4]，允许内联变量在程序中可以有多于一次的定义，只要每个定义都出现在不同翻译单元中（对于非静态的内联函数和变量(C++17 起)）且所有定义完全一致即可。。

于是可以更方便地使用 inline 修饰来解决，同时因为是 inline 也不再需要在源文件中提供定义了：
```cpp
class Solution {
    inline static int m = 123;
};
```
实则是通过 inline 告知编译器允许多个翻译单元对同一个变量都存在定义，最后链接时只保留其中一个作为定义，从而不违背 ODR。

[^1]: https://en.cppreference.com/w/cpp/language/definition
[^2]: https://isocpp.org/wiki/faq/cpp11-language-classes#member-init  推荐阅读
[^3]: https://cppinsights.io/s/e2e0638a
[^4]: https://en.cppreference.com/w/cpp/language/inline

2024.12.12