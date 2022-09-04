# auto 类型说明符

auto 一般会忽略掉顶层 const，同时底层 const 则会保留下来，比如当初始值是一个指向常量的指针时：

~~~C++
int i = 0;
const int ci = 1;
const int& cr = ci;
auto b = ci;	// b 是一个整数（ci 的顶层 const 特性被忽略掉）
auto c = cr;	// c 是一个整数（cr 是 ci 的别名，ci 本身就是一个顶层 const）
auto d = &i;	// d 是一个整型指针（整数的地址就是指向整数的指针）
auto e = &ci;	// e 是一个指向整型常量的指针（对常量对象取地址是一种底层 const）
~~~

这里我把这个 <C++ Primer(第 5 版)> 上的这个例子翻译一下：

~~~C++
int i = 0;
const int ci = 1, &cr = ci;
auto b = ci;	// b 的类型是 int
auto c = cr;	// c 的类型是 int
auto d = &i;	// d 的类型是 int*
auto e = &ci;	// e 的类型是 const int*
~~~

如果希望推断出的 auto 类型是一个顶层 const，则需要明确指出：

~~~C++
const auto f = ci;	// ci 的推演类型是 int，f 是const int
~~~

还可以将引用设为 auto，此时原来的引用规则仍然适用：

~~~C++
auto& g = ci;		// g 是一个整型常量引用，绑定到 ci
auto& h = 42;		// 错误，不能为非常量引用绑定字面量
const auto& j = 42;	// 正确，可以为常量引用绑定字面量
~~~

设置一个类型为 auto 的引用时，初始化值中的顶层常量属性仍然保留。
和往常一样，如果我们给初始化值绑定一个引用，则此时的常量就不是顶层常量了。

这句话真难理解，我还是翻译一下：

~~~C++
auto& g = ci; // g 是 const int&
~~~

第一句话「设置一个类型为 auto 的引用时，初始化值中的顶层常量属性仍然保留」。

它就是想说，对绑定对象为 const int 的 auto&，推断出来会是 const int& 而非是 int&。

对比着无引用的 auto 来看：

~~~C++
int x = 1;
const int cx = 1;
auto y1 = cx;	// auto 推断为 int
auto& y2 = cx;	// auto 推断为 const int
const auto& y3 = x; // auto 推断为 int
~~~

可见，y1 的 auto 为 int，y2 的 auto 为 const int 类型。

也就是说，对于 const int 的 cx，即便没有加 const 的 auto& 也会推断为 const int&，而非 int&，表示绑定对象 x/ci 是不变的常量（「初始化值中的顶层常量属性仍然保留」）。

顺带提一句，当使用 const auto& 时（如 y3），auto 推断出来只会是无 const 的类型，这里也就是 为 int 而不会为 const int。这句话是参考 [04模板类型实参](04模板类型实参.md) 中

>当函数参数本身是 const 时，T 的类型推断的结果不会是一个 const 类型：
>const 已经是函数参数类型的一部分了，因此 const 不会是模板参数类型的一部分。

推测的。

第二句话「和往常一样，如果我们给初始化值绑定一个引用，则此时的常量就不是顶层 const 了」。

它就是重新强调了一下 [01顶层\_底层_const](01顶层_底层_const.md) 所说：「用于声明引用的 const 都是底层 const」。

auto& 推断出的 const int& 表示的是 ci 不可变，是底层 const，而不是顶层 const。

实际上吧，**个人猜想**哈
引用，是由编译器来保证不会改变引用绑定的对象，编译器尽力地想让程序员把引用当作被引用对象的别名来使用。
但本质上，个人估计仍然是使用指针来实现的，只不过会自动地替我们完成解引用的操作。

所以 const int& g = ci，将其看作指针，写为 const int* g = ci 这样的形式，则该 const 为底层 const，表示其指向的对象 ci 为 const，是很自然的。

2022.08.31
