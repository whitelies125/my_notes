# Function Template Instantiation

函数模板实例化。

## What is function template instantiation

什么是模板实例化。

Function Template & Instantiation

- A function template is just a blueprint, not a type, or a function.

  函数模板本身只是一个蓝图，并不是一个类型或一个函数。

- 编译器扫描代码，遇到模板定义时，并不立即产生代码。

- The template arguments must be determined so that the compiler can generate an actual function.

  必须在确定了模板实参后，编译器才生成一个实际函数的代码。

两种实例化方法（确定模板实参的方法）

1. Explicit instantiation 显式实例化
2. Implicit instantiation 隐式实例化

模板多态，实际上也是编译时多态，属于静态联编。

## Explicit instantiation

显式实例化。

前置某些函数实例化，可出现于程序中模板定义后的任何位置。

~~~C++
template <typename T>
void f(T s){
    std::cout << s << std::endl;
}

template void f<double>(double);
//实例化，<> 内为 T 的类型，编译器生成代码,
//void f(double s){
//    std::cout << s << std::endl;
//}

template void f<>(char);//实例化 f<char>(char),自动推导出 T 为 char
template void f(int);//实例化 f<int>(int),自动推导出 T 为 int
~~~

## Implicit instantiation

隐式实例化

编译器查看函数调用，推断模板实参，实现隐式实例化。

~~~C++
template<typename T>
void f(T s){
    std::cout << s << std::endl;
}
int main(){
    f<double>(1);//实例化并调用 f<double>(double)
    f<>('a');//实例化并调用 f<char>(char)
    f(7);//实例化并调用 f<int>(int)
    void (*ptr)(std::string) = f;//实例化 f<string>(string)
    //ptr 为函数指针
}
~~~

## Instantiated function/class

实例函数/实例类。

C++11(Section:14.7):

- A function instantiated from a function template is called an **instantiated function**.

  由函数模板实例化得到的函数叫作实例函数。

- A class instantiated from a class template is called an **instantiated class**.

  由类模板实例化得到的类叫作实例类。

顺带，偶尔会看到的一些称呼：template function / template class 模板函数 / 模板类。
实际是指 function template / class template 函数模板 / 类模板。

## Example

~~~c++
#include <iostream>
#include <string>
//函数模板定义
template <typename T>
T my_max(T x, T y){
    return x > y ? x : y;
}
//显式实例化:int
template int my_max<int>(int,int);

int main(){
    //调用 int
    std::cout << my_max(2,1) << std::endl;
    //浮点数实例化
    std::cout << my_max(4.3,2.1) << std::endl;
    //字符实例化
    std::cout << my_max('B','A') << std::endl;
    //字符串字面量实例化
    //注意，字符串字面量本质是传递的 const char *,比大小也只是比的地址的大小。
    //这里将 "ABC" 放在了高地址，"ABD" 放在了低地址，所以输出 ABC
    std::cout << my_max("ABC","ABD") << std::endl;
    //这么这里应该输出 ABD，因为 ABD 在前？实际上，这里复用了上面的 "ABC" "ABD"，所以仍然是输出 ABC
    std::cout << my_max("ABD","ABC") << std::endl;
    //下同
    std::cout << my_max("123","124") << std::endl;
    std::cout << my_max("124","123") << std::endl;
    std::cout << my_max("99","98") << std::endl;
    std::cout << my_max("99","98") << std::endl;
    //std::string 实例化
    using namespace std::string_literals;
    //这里才是传递的 string 类型，输出结果是预期的 ABD
    std::cout << my_max("ABD"s,"ABC"s) << std::endl;
    std::cout << my_max("ABC"s,"ABD"s) << std::endl;
    return 0;
}
//output:
//2
//4.3
//B
//ABC
//ABC
//123
//123
//99
//99
//ABD
//ABD
~~~



