# Overloading Object Converting Operator

重载对象转换运算符。

## Object Conversion

对象转换。

类型转换运算符（conversion operator）是类的一种特殊成员函数，它负责将一个类类型的值转换成其它类型。

类型转换函数的一般形式为：

~~~C++
operator type() const;
~~~

type 表示某种类型。

类型转换运算符既没有显式的返回类型，也没有形参，而且必须定义成类的成员函数。

类型转换运算符通常不应该改变待转换对象的内容，因此，类型转换运算符一般被定义成 const 成员。

注意：一个类型转换函数必须是类的成员函数；它不能声明返回类型，形参列表也必须为空。类型转换函数通常应该是 const。

尽管类型转换函数不负责指定返回类型，但实际上每个类型转换函数都会返回一个对应类型的值。

以 Vec2D 转 double 为例。

~~~C++
Vec2D:: operator double(){//Vec2D 转换为 double
    return magnitude();
}

Vec2D v1{3,4};
double d =  v1 + 5.1;//d = 10.1
//此处 v1 进行了隐式类型转换 Vec2D -> double 调用 Vec2D:: operator double()
//但注意，由于我在 Vec2D 类中重载了 +(double)，所以和预想的不一样,在下面最后讲

double e =  static_cast<double>(v1);//e = 5.0
//显式类型转换 Vec2D -> double，调用 Vec2D:: operator double()
~~~

## Example

省略其它的函数，只保留相关的。

主文件：test.cpp

~~~C++
#include <iostream>
#include "Vec2D.h"
int main(){
    Vec2D v1{3.0,4.0};
    std::cout << "static_cast<double>(v1): " << static_cast<double>(v1) << std::endl;
    std::cout << "v1 + 5.0 = " << (v1 + 5.0) << std::endl;

    return 0;
}
//output:
//static_cast<double>(v1): 5
//v1 + 5.0 = 12.0416
~~~

头文件：Vec2D.h

~~~C++
#pragma once
#include <string>
#include <istream>
#include <ostream>
class Vec2D{
private:
    double x;
    double y;
public:
    Vec2D();
    Vec2D(double x, double y);
    ~Vec2D();

    //求向量长度 magnitude
    double magnitude();

    //类型转换重载 double
    operator double();
};
~~~

实现：Vec2D.cpp

~~~C++
#include "Vec2D.h"
#include <exception>
#include <cmath>
#include <iostream>
#include <istream>
#include <ostream>

Vec2D::Vec2D(){}
Vec2D::Vec2D(double x, double y) : x{x}, y{y} {}
Vec2D::~Vec2D(){}

//求向量长度 magnitude
double Vec2D::magnitude(){
    return sqrt(x*x + y*y);
}

//类型转换重载 double
Vec2D::operator double(){
    return magnitude();
}
~~~

## 一个右值的问题

这里，为什么

~~~C++
std::cout << "v1 + 5.0 = " << (v1 + 5.0) << std::endl;
~~~

输出

~~~C++
//v1 + 5.0 = 12.0416 
~~~

而不是预想的 v1 转换为 double = 5，然后 5+5.0 = 10，输出

~~~C++
//v1 + 5.0 = 10.0
~~~

这是因为，v1 + 5.0 调用了之前写的重载加号 + 运算符，

~~~C++
//运算符重载 +
Vec2D operator +(const double numeral);
Vec2D Vec2D::operator +(const double numeral){
    return this->add(numeral);
}
~~~

于是这里的逻辑是调用

~~~C++
v1.operator+(5.0);//(3.0，4.0) -> (8.0，9.0)
~~~

但奇怪的是，此时 (v1 + 5.0) 返回的是 Vec2D 类型。

~~~C++
std::cout << "v1 + 5.0 = " << /*此处应为 Vec2D类型*/(v1 + 5.0) << std::endl;
~~~

而此前我们也重载了流输出操作数 <<：

~~~C++
//二元预算符重载，流操作运算符 输出运算符<<
friend std::ostream& operator <<(std::ostream& os, Vec2D& vec2d);
std::ostream& operator <<(std::ostream& os, Vec2D& vec2d){
    os << vec2d.toString();
    return os;
}
~~~

于是，在我的设想中，应该是：

~~~C++
//std::cout << "v1 + 5.0 = " 返回 ostream&
ostream << Vec2D << std::endl;
~~~

所以会调用 << 重载函数。输出

~~~C++
//(8.0,9.0)
~~~

然而实际上输出是

~~~C++
//v1 + 5.0 = 12.0416 
~~~

可见，这里应该是进行了隐式类型转换，将 (v1 + 5.0) 转换为了 double，然后输出。

问题在于，流输出操作符 << 的重定义函数，形参为 Vec2D 的应用类型 Vec2D&。

~~~C++
friend std::ostream& operator <<(std::ostream& os, Vec2D& vec2d);
~~~

而 (v1 + 5.0) 返回的是一个右值，不能对其使用左值引用。

于是判定不符合流输出符 << 的重载函数，不会调用。

std::cout 能够输出 double 类型，而我们又给 Vec2D 重载了类型转换运算符 double。

所以，

~~~C++
ostream << (v1+5.0)
~~~

会调用

~~~C++
operator double();
~~~

然后为：

~~~C++
ostream << double
~~~

最终输出

~~~C++
//v1 + 5.0 = 12.0416 
~~~

在把流输出符 << 重载函数，形参改为 Vec2D vec2d 后，就没有问题了

~~~C++
//二元预算符重载，流操作运算符 输出运算符<<
friend std::ostream& operator <<(std::ostream& os, Vec2D vec2d);
std::ostream& operator <<(std::ostream& os, Vec2D vec2d){
    os << vec2d.toString();
    return os;
}
~~~

输出为

~~~C++
static_cast<double>(v1): 5
v1 + 5.0 = (8.000000,9.000000)
~~~

## 显式的类型转换运算符 explicit

为了防止隐式类型转换导致一些异常情况，C++11 引入了显式的类型转换运算符（explicit conversion operator）。

也就是在类型转换重载函数前加上 explicit

~~~C++
explicit operator double();
~~~

这样，编译器通常不会将一个显示的类型转换运算符用于隐式类型转换。

但存在例外，即当表达式被用作条件时，则编译器会将显式的类型转换自动应用于它。换句话说，当表达式出现在下列位置时，显式的类型转换将被隐式地执行：

- if, while 及 do 语句的条件部分。
- for 语句头的条件表达式。
- !, ||, && 的运算对象
- 条件运算符（? :）的条件表达式。
