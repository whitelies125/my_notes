# Structured Binding Declaration for Array

用于数组的结构化绑定声明。

注意的是，此时我使用的编译器是 g++ (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0，查询网上信息后，发现在不指定 C++ 版本时 g++ 可能默认使用的是 C++14。
所以使用结构化绑定时，需要输入：

~~~C++
g++ -std=c++17 test.cpp -o test.exe
~~~

指定 C++ 版本为 17 或以上才行。

由于我现在才发现，所以前面包括后面的 C++ 的学习内容，大部分都是直接输入

~~~C++
g++ test.cpp -o test.ext
~~~

即默认 C++ 版本为 14 的情况。

## What is "structured binding declaration"?

什么是结构化绑定声明。

It's a declaration statement, which means some identifiers are declared and initialized.

结构化绑定声明是一个声明语句，意味着声明了一些标识符并对标识符做了初始化。在 C++17 中引入。

Binds the specified name to subovjects or elements of the initializer.

将指定的一些名字绑定到初始化器的子对象或者元素上。

例如：

~~~c++
int x = {5};
~~~

x 就是标识符 identifier，
花括号 {} 括起来的 5 就是初始化器 initializer。

形态：

~~~C++
cv-auto &/&&(可选) [标识符列表] = 表达式;
cv-auto &/&&(可选) [标识符列表] (表达式);
cv-auto &/&&(可选) [标识符列表] {表达式};
~~~

说明：

~~~C++
cv-auto： 表示可能由 const(c) 或 volatile(v) 修饰的 auto 关键字。
&/&&：& 表示左值引用，&& 表示右值引用。
标识符列表：为逗号分隔的标识符。
~~~

## Structured Binding Declaration for Array

用于原生数组的结构化绑定声明。

若初始化表达式为数组类型，则标识符列表中的名字绑定到数组元素。

- 标识符数量必须等于数组元素数量。
- 标识符类型与数组元素类型一致。

~~~C++
int main(){
    int priArr [] {42, 21, 7};
    //下面 ai/bi/ci 基本类型都是 int，但前面修饰不同 auto/const auto/auto &
    auto [a1, a2, a3] = priArr;//ai 是 priArr[i] 的拷贝
    const auto [b1, b2, b3] = priArr;//bi 是 priArr[i] 的只读拷贝
    auto &[c1, c2, c3] {priArr};//ci 是 priArr[i] 的引用
    c3 = 14;//修改 priArr[2] 的值为 14
    return 0;
}
~~~

## Structured Binding Declaration for std::array

用于 std::array 的结构化绑定声明。

若初始化表达式为数组类型，则标识符列表中的名字绑定到数组元素。

- 标识符数量必须等于 std::array 中的元素数量。
- 标识符类型与 std::array 中的元素类型一致。

~~~C++
int main(){
    std::array stdArr = {'a', 'b', 'c'};//c++17 自动推导为 char 类型。
    auto [d1, d2, d3] {stdArr};
    return 0;
}
~~~

---

补充用法：

~~~C++
#include <iostream>
#include <string>

struct Student{
    int number;
    float score;
    char ch;
};

int main(){
    struct Student mio { 1, 100.00, 'm'};
    auto& [a,b,c] = mio;
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
//g++ -std=c++17 test.cpp -o test.exe
//output:
//1
//100
//m
~~~

