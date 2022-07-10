# Overloading the - Operators

重载符号运算符。

正号 + and 符号 - are unary operators.

正号和负号都是一元运算符。

- only operates on the calling object itself.

  只对调用该运算符的对象起作用。

- the unary function operator has NO parameters.

  作为对象成员的一元运算符无参数。

以 Vec2D 类为例：

调用 - 运算符：

~~~C++
Vec2D v1(2,3);
vec2D v2 = - v1;//要实现 (2,3)->(-2,-3)
std::cout << v1.toString();
~~~

重载 - 运算符：

~~~C++
Vec2D Vec2D::operator -(){
    return Vec2D(- this->x,- this->y);
}
~~~

 ## Example

省略其它的函数，只保留 - 相关的。

主文件：test.cpp

~~~C++
#include <iostream>
#include "Vec2D.h"
int main(){
    Vec2D v1{1,2};
    Vec2D v2 = -v1;
    std::cout << "v2: " << v2.toString() <<std::endl;
    return 0;
}
//output:
//v2: (-1.000000,-2.000000)
~~~

头文件：Vec2D.h

~~~C++
#pragma once
#include <string>

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
   
    //运算符重载 -
    Vec2D operator -(const Vec2D& second_Vec2D);
    Vec2D operator -(const double numeral);
    
    //一元运算符重载 -
    Vec2D operator -();
};
~~~

实现：Vec2D.cpp

~~~C++
#include "Vec2D.h"
#include <exception>
#include <cmath>
#include <iostream>

Vec2D::Vec2D(){}
Vec2D::Vec2D(double x, double y) : x{x}, y{y} {}
Vec2D::~Vec2D(){}

//向量转为字符串
std::string Vec2D::toString(){
    return std::string("("+std::to_string(x)+","+std::to_string(y)+")");
}

//一元运算符重载 -
Vec2D Vec2D::operator -(){
    return Vec2D{-x,-y};
}
~~~

