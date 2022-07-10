# Constant Expression and C++11 constexpr keyword

## Constant expressions

常量表达式。

Constant expression is an expression that can be evaluated at compile time.

常量表达式是编译期可以计算值的一个表达式。

C++ 数组的大小设置要求是一个编译期的一个常量（原生数组及 std::array）。

~~~C++
int n = 1; 
n++;
std::array<int, n> a1; // error: n is not a constant expression

const int cn = 2;
std::array<int, cn> a2; // OK: cn is a constant expression
~~~

但 const 修饰的变量未必是编译期常量，也可能是运行时常量。

~~~C++
const int rcn = n; // rcn is runtime constant, compiler does NOT know its value at compile-time
rcn = ++n;// error: rcn is read-only
std::array<int , rcn> a3; // error: rcn is NOT known at compile-time
~~~

注意区分，使用 constexpr 表示编译期常量，而 const 既可能是编译期常量，也可能是运行时常量。

##  C++11 constexpr: 编译期常量表达式说明符

constexpr specifier declares that it is possible to evaluate the value of the function or variable at compile time.

constexpr 说明符声明可在编译时计算函数或变量的值。

~~~C++
constexpr int max(int a, int b){// c++11 引入 constexpr
	//注意，使用 constexpr 修饰函数，只有当其传入的参数也全为 constexpr 时，才能在编译时返回 constexpr 常量，否则就是普通的函数。
    if(a > b) return a;// c++14 才允许constexpr函数中有分支循环等
    else return b;
}

int main() {
	int m = 1;
	const int rcm = m++;// rcm 是运行期常量
	const int cm = 4;// cm 是编译期常量，等价于: constexpr int cm = 4;
	int a1[ max(m, rcm) ]; // 错误：m 和 rcm 都不是编译期常量，m 是变量，rcm 是运行期常量
	std::array<char, max(cm, 5)> a2; // OK: cm 和 5 都是编译期常量，constexpr int max() 可以在编译器求解出返回值。cm 是编译器常量，5 是字面常量。
}
~~~

上述代码段的注释为老师的注释。

但我自己测试时，int a1[max(m,rcm)] 的结果和老师举例不一样。

我猜也许是因为编译器，或者 C++ 标准的问题？

~~~C++
#include <iostream>
#include <array>
constexpr int max(int a, int b){// c++11 引入 constexpr
    if(a > b) return a;// c++14 才允许constexpr函数中有分支循环等
    else return b;
}
int main() {
	int variable = 1;//变量
	const int run_time_const = variable++;//运行时常量
	const int compile_const = 4;//编译期常量
    //array<int, 此处必须为编译期常量>
    std::array<int,max(compile_const,2)> a;//此行，编译正常通过。
    
    int b[ max(variable, run_time_const) ];//此行，我编译确实是正常通过了的。
    std::array<int, max(variable, run_time_const)> c;//此行，报错。
	//明明都是 max(variable, run_time_const)，但 int c[_]通过，但 array<int,_> 不行。
    //明确一点，array<int, 此处必须为编译期常量>
    //网上搜索到，变长数组(Variable-Length Array)是 C 语言 C99 标准中支持的，而并不属于 C++ 的标准。能否通过编译就看你用的编译器是否选择了支持了。
    //而我用的编译器 g++，它是支持了变长数组的。
	return 0;
}
~~~

再次注意一下：

constexpr 修饰常量，则为编译期常量。

constexpr 修饰函数，返回值未必为编译期常量，只有当其传入的参数也全为 constexpr 时，即只有当其传入的参数都能在编译期计算出来，该函数才能在编译期返回 constexpr 常量。否则就是普通的函数。

关于 constexpr 修饰函数，我这里利用 array 的数组大小参数必须为编译期常量的特点，可以测试：

~~~C++
#include <iostream>
#include <array>
constexpr int max(int a, int b){// c++11 引入 constexpr
    if(a > b) return a;// c++14 才允许constexpr函数中有分支循环等
    else return b;
}
int main() {
	int variable = 1;//变量
	const int run_time_const = variable++;//运行时常量
	const int compile_const = 4;//编译期常量
	
	std::array<int,max(compile_const,4)>;//编译通过，正确
    std::array<int,max(compile_const,variable)>;//编译出错
    std::array<int,max(compile_const,run_time_const)>;//编译出错

    return 0;
}
~~~

## const vs constexpr

主要区别：

- const：告知程序员，const 修饰的内容是不会被修改的。主要目的是帮助程序员避免 bug。

  ~~~C++
  char * s1 = "Hello";//C 允许，C++ 编译出错
  *s1 = 'h';//c 中，语法正确，但运行时会出错
  const char* s2 = "World";//c++ 要求加 const
  *s = 'W';//C++ 编译器报错。
  ~~~

- constexpr：用在所有被要求使用 constant expression 的地方（即 constexpr 修饰的东西可以在编译期计算得到其值），主要目的是让编译器能够优化代码提升性能。

constexpr：初学者只需了解其含义即可。

constexpr 用法有非常多的细节（cppreference.com 列出了30 多个条目）。

C++14 、C++17 、C++20 对它都有细节修改。
