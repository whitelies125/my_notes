# assert and C++11 static_assert

断言与 C++11 的静态断言。

## Assertion is a statement to check assumptions.

断言是一条检查假设成立与否的语言。

### assert: 是 C 语言的宏（Macro），运行时检测。

assert 是宏，而不是函数。

头文件：

~~~C++
#include <cassert>
~~~

用法：包含头文件 cassert 以调试模式编译程序。

~~~C++ 
assert(bool_expr); //bool_expr 为假，则中断程序
~~~

编译程序时，有 release 和 debug 模式。

二者传递给给编译器的参数不一样，所以使得二者有所区别。

在 debug 模式下，编译器一般不会打开优化功能的、不会对你的代码进行优化，而会往里面塞一些额外的、附加的、用于帮助我们调试的信息。

在 release 模式下，则没有这些调试信息，同时还会打开编译优化的开关，对程序进行优化，编译出来的程序小，运行速度快。

~~~C++
std::array a{ 1, 2, 3 };  //C++17 类型参数推导
for (size_t i = 0; i <= a.size(); i++) {
	assert(i < 3);  //断言：i必须小于3，否则失败，中断程序
	std::cout << a[i];
	std::cout << (i == a.size() ? "" : " ");
~~~

### assert() 依赖于 NDEBUG 宏 

NDEBUG 宏是 C/C++ 标准规定的，所有编译器都有对它的支持。

调试模式 Debug 编译时，编译器不会定义 NDEBUG，所以 assert() 宏起作用。

发行模式 Release 编译时，编译器自动定义宏 NDEBUG，使 assert() 不起作用

如果要强制使得 assert() 生效或者使得 assert() 不生效，只要手动 #define NDEBUG 或者 #undef NDEBUG即可。

### assert 帮助调试解决逻辑 bug（颗部分替代 断点/单步调试）。

~~~C++
#undef NDEBUG   // 强制以debug模式使用<cassert>

int main() {
	int i;
	std::cout << "Enter an int: ";
	std::cin >> i;
	assert((i > 0) && "i must be positive");//若 assert 中断了程序则表明程序出 bug 了！程序员要重编代码解决这个 bug，而不是把 assert() 放在这儿当成正常程序的一部分
    //assert 用于调试程序 debug，而不能用于控制程序逻辑。
	return 0;
}
~~~

## C++11: static_assert

C++11 的静态断言。

static_assert 是一个关键字。

~~~C++
static_assert(bool_constexpr,message)//C++17 起，message 可选。
//bool_constexpr: 编译期常量表达式，可转换为 bool 类型
//message: 字符串字面量，是断言失败时的警告信息。
~~~

作用：编译时断言检查。

~~~C++
//例子：确保编译仅在 32 位的平台上进行，不支持 64 位的平台。
static_assert(sizeof(void*) == 4, "64-bit code generation is not supported.")
~~~

使用：

bool_constexpr 中不能有变量表达式。

常用在模板库中，对写库的作者用处大。

## When to use assertions

何时使用断言。

Code Complete 2: "Use error-handling for conditons you expect to occur; use assertions for conditions that should never occur."

《代码大全 2》：若某些情况是你预期中的，那么用错误处理，若某些情况永不该发生，用断言。

~~~C++
int n{1},m{0};
std::cin >> n;
assert((n!=0) && "Divesor cannot be zero!");// 使用 assert 作为正式代码不合适，
//应该在此处的 assert 报 bug 后，写代码处理这个 bug，最终去掉这里的 assert，而不是直接出错终止程序。
int q = m/n;
~~~

修改后

~~~C++
int n{1},m{0};
do{	//处理掉上述的 bug。
    std::cin >> n;
}while(n == 0);
int q = m/n;
~~~

