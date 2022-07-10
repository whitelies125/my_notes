# Overloading <</>> Operators

重载流操作运算符。

<< 和 >> 是在 ostream 和 istream 类中定义（重载）的：

- (<<): send values to cout（将信息 输出到 ostream 对象）; 比如 cout << vec2d;
- (>>): read values from cin（从 istream 对象中读取信息）；比如 cin >> vec2d;

<< and >> are binary operator（流提取/插入运算符是二元运算符）.

## Overloading << and >> as friend

重载 << 和 >> 为友元。

通常使用 << 和 >> 运算符时，第一个参数是流类的实例：

~~~C++
cin >> x >> y;//cin 在最左边起始
cout<< x << y;//cout 在最左边起始
~~~

假设我们试图将运算符重载为类的成员函数，则以这种方式，在调用该运算符时，左操作数必须是该类的实例。

首先，我们是不能修改 cout/cin 所属的类，加进去一个成员函数

~~~C++
class ostream{
public:
    ostream& operator <<(Vec2D& vec2d);
}
~~~

毕竟这样岂不是要对每一个自定义类都在 ostream/istream 中写一个成员函数。

那么，如果我们将 << 在我们自定义类中重载为其成员函数：

~~~C++
class Vec2D{
public:
    std::ostream& operator<<(std::ostream &stream);
}
~~~

这样是可以的。
但使用起来，其形式与我们通常的习惯的写法 cout << v1 是相反的。

~~~C++
//调用 v1 的成员函数则应该写为
v1 << cout;//v1 为该类的对象。
//等同于
//v1.operator <<(cout);
~~~

这样的做法会导致使用时的不便。

所以，为了保持与其它的类型输入输出格式一致，使用方式一致

~~~C++
cout << v1;
cout << v1 << v2;
cout << "v1 = " << v1 << " v2 = " << v2;
~~~

<</>> should be overloaded as "friend function".

<</>> 应当重载为友元函数。

即

~~~C++
class Vec2D{
public:
    friend std::ostream& operator<<(std::ostream &stream,Vec2D& vec2d);
    friend std::istream& operator>>(std::istream &stream,Vec2D& vec2d);
}
~~~

## Example

省略其它的函数，只保留 <</>> 相关的。

主文件：test.cpp

~~~C++
#include <iostream>
#include "Vec2D.h"
int main(){
    std::cout << "input 2 number:" << std::endl;
    Vec2D v;
    std::cin >> v;
    std::cout << v;
    return 0;
}
//output:
//input 2 number:
//1.1 2.2
//(1.100000,2.200000)
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

    //向量转为字符串
    std::string toString();

//二元预算符重载，流操作运算符
friend std::ostream& operator <<(std::ostream& os, Vec2D& vec2d);
//二元预算符重载，流操作运算符 输入运算符
friend std::istream& operator >>(std::istream& is, Vec2D& vec2d);
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

//向量转为字符串
std::string Vec2D::toString(){
    return std::string("("+std::to_string(x)+","+std::to_string(y)+")");
}

//二元预算符重载，流操作运算符 输出运算符
std::ostream& operator <<(std::ostream& os, Vec2D& vec2d){
    os << vec2d.toString();
    return os;
}
//二元预算符重载，流操作运算符 输入运算符
std::istream& operator >>(std::istream& is, Vec2D& vec2d){
    is >> vec2d.x >> vec2d.y;
    return is;
}
~~~

