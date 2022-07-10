# Overloading General Binary Arithmetic Operators

重载一般二元算术运算符。

二元：两个操作数。

算术：加减乘除 + - * /。

分析：

~~~C++
Vec2D a {1,2};
Vec2D b {3,6};
double z {1.3};
Vec2D c = a + b;
//a.operator+(b); Vec2D Vec2D::operator+(Vec2D)
Vec2D d = a + z;
//a.operator+(z); Vec2D Vec2D::operator+(double)
Vec2D e = z + b;
//z.operator+(b); Vec2D double::operator+(Vec2D)
//错误，由于 z 是 double，是 C++ 内置类型，不能重载内置类型的运算符。
//得换一种方式实现
~~~

函数原型：

~~~C++
struct Vec2D{
    //因为加号 + 是二元运算符，所以需要接受两个操作数/参数
    //当为成员函数时，当前对象就已作为了加号 + 左边的一个操作数。
    Vec2D operator +(Vec2D);//成员函数
    Vec2D operator +(double);//成员函数
	//当为非成员函数时，需要指明两个操作数/参数了。
    //第一个参数为加号 + 左边的操作数，第二个参数为加号 + 右边的操作数
    friend Vec2D operator +(double, Vec2D);
	//这里使用非成员函数，友元函数来实现 double + Vec2D
    //当然也可以非友元函数，用友元函数是为了直接访问类中的数据成员，更方便一些，非友元函数就得通过 public 才能访问，稍微麻烦一点。
}
Vec2D operator +(double, Vec2D){
   //do something
}
~~~

## Example

下面省略了一些没用上的函数。

友元函数实现重载，只写了数字在前的数乘。

主文件：test.cpp

~~~C++
#include <iostream>
#include "Vec2D.h"
int main(){
    //创建向量对象
    Vec2D v1{3,5};
    Vec2D v2{4,6};
    //向量加法：向量+向量，向量+数
    Vec2D v3 = v1 + v2;
	Vec2D v4 = v1 + 10.0;
    std::cout << "v3 = " << v3.toString() << std::endl;
    std::cout << "v4 = " << v4.toString() << std::endl;
    //向量减法
    Vec2D v5 = v2 - v1;
    //向量点积
	double v6 = v2 * v1;
    //向量数乘
    Vec2D v7 = v3 * 2.1;
	std::cout << "v2 - v1 = " << v5.toString() << std::endl;
    std::cout << "v2 . v1 = " << v6 << std::endl;
	std::cout << "v3 * 2.1 = " << v7.toString() << std::endl;
    std::cout << "2.1 * v3 = " << (2.1 * v3).toString() << std::endl;
    return 0;
}
//output:
//v3 = (7.000000,11.000000)
//v4 = (13.000000,15.000000)
//v2 - v1 = (1.000000,1.000000)
//v2 . v1 = 42
//v3 * 2.1 = (14.700000,23.100000)
//2.1 * v3 = (14.700000,23.100000)
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
    //向量减法
    Vec2D subtract(const Vec2D& second_Vec2D);
    Vec2D subtract(double numeral);
    //向量点积
    double dot(const Vec2D& second_Vec2D);
    //向量数乘
    Vec2D multiply(double multiplier);

    //运算符重载 +
    Vec2D operator +(const Vec2D& second_Vec2D);
    Vec2D operator +(const double numeral);
    //运算符重载 -
    Vec2D operator -(const Vec2D& second_Vec2D);
    Vec2D operator -(const double numeral);
    //运算符重载 *
    double operator *(const Vec2D& second_Vec2D);
    Vec2D operator *(const double multiplier);
friend Vec2D operator *(const double  multiplier, Vec2D& vec2d);
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
//向量加法：向量+向量，向量+数
Vec2D Vec2D::add(const Vec2D& second_Vec2D){
    return Vec2D{x + second_Vec2D.x, y + second_Vec2D.y};
}
Vec2D Vec2D::add(double numeral){
    return Vec2D{x + numeral, y + numeral};
}
//向量减法
Vec2D Vec2D::subtract(const Vec2D& second_Vec2D){
    return Vec2D{x - second_Vec2D.x, y - second_Vec2D.y};
}
Vec2D Vec2D::subtract(double numeral){
    return Vec2D{x - numeral, y - numeral};
}
//向量点积
double Vec2D::dot(const Vec2D& second_Vec2D){
    return x * second_Vec2D.x + y * second_Vec2D.y;
}
//向量数乘
Vec2D Vec2D::multiply(double multiplier){
    return Vec2D{x * multiplier, y * multiplier};
}

//运算符重载 +
Vec2D Vec2D::operator +(const Vec2D& second_Vec2D){
    return this->add(second_Vec2D);
}
Vec2D Vec2D::operator +(const double numeral){
    return this->add(numeral);
}
//运算符重载 -
Vec2D Vec2D::operator -(const Vec2D& second_Vec2D){
    return this->subtract(second_Vec2D);
}
Vec2D Vec2D::operator -(const double numeral){
    return this->subtract(numeral);
}
//运算符重载 *
double Vec2D::operator *(const Vec2D& second_Vec2D){
    return this->dot(second_Vec2D);
}
Vec2D Vec2D::operator *(const double multiplier){
    return this->multiply(multiplier);
}
Vec2D operator *(const double  multiplier, Vec2D& vec2d){
    return vec2d.multiply(multiplier);
}
~~~

