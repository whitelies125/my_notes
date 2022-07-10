# Overloading Shorthand Binary Arithmetic Operators

重载简写/复合二元算术运算符。

shorthand operators：+=, -=, *=, /=.

~~~C++
v1 += v2;//执行后，v1 的值被改变了
//等同于
v1 = v1 + v2
~~~

所以，我们在 Vec2D 类中实现 += 方法之一可为：

~~~C++
Vec2D vec2D::operator +=(const Vec2D& second_Vec2D){
    *this = this->add(second_Vec2D);
    //this->add(second_Vec2D) 返回一个匿名对象
    //匿名对象赋值给 *this。
    return *this;
}
~~~

## Example

这里就只显示 += 和 -= 的例子了。

主文件：test.cpp

~~~C++
#include <iostream>
#include "Vec2D.h"
int main(){
    Vec2D v1{10,12};
    Vec2D v2{1,2};
    //+=
    std::cout << "v1 += v2: " << (v1 += v2).toString() << std::endl;
    //-=
    std::cout << "v1 -= v2: " << (v1 -= v2).toString() << std::endl;

    return 0;
}
//output:
//v1 += v2: (11.000000,14.000000)
//v1 -= v2: (10.000000,12.000000)
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
    //向量加法：向量+向量，向量+数
	Vec2D add(const Vec2D& second_Vec2D);
    Vec2D add(double numeral);

    //运算符重载 +=
    Vec2D& operator +=(const Vec2D& second_Vec2D);
    //运算符重载 -=
    Vec2D& operator -=(const Vec2D& second_Vec2D);
};
~~~

实现：Vec2D.cpp

~~~C++
#include "Vec2D.h"
#include <exception>
#include <cmath>

Vec2D::Vec2D(){}
Vec2D::Vec2D(double x, double y) : x{x}, y{y} {}
Vec2D::~Vec2D(){}

//向量转为字符串
std::string Vec2D::toString(){
    return std::string("("+std::to_string(x)+","+std::to_string(y)+")");
}
//运算符重载 +=
Vec2D& Vec2D::operator +=(const Vec2D& second_Vec2D){
    x += second_Vec2D.x;//这里 += 就是普通的 += 啊，是 double 的+=
    y += second_Vec2D.y;//同上
    return *this;
}
//运算符重载 -=
Vec2D& Vec2D::operator -=(const Vec2D& second_Vec2D){
    x -= second_Vec2D.x;//这里 -= 也是 double 的 -=
    y -= second_Vec2D.y;//同上
    return *this;
}
~~~
