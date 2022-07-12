# Overloading the ++ and -- Operators

重载 ++ 和 -- 运算符。

## ++ and -- may be prefix or postfix.

自增/自减运算符可前置也可后置。

- prefix: ++var or --var

  前置，先增减后取值，表达式是 lvalue。

- postfix: var++ or var--

  后置，先取值后增减，表达式是 prvalue。

以 Vec2D 为例，规则设定为：

~~~C++
++(a,b) -> (++a,++b)
(a,b)++ -> (a++,b++)
~~~

## prefix ++/-- v.s. postfix ++/--

前置与后置的差别。

规定：

前置 ++/-- 重载无参数，返回引用类型。

后置 ++/-- 重载带参数，参数为 dummy 参数。

后置 ++/-- 运算符重载时的整型参数 dummy 仅仅是一个标记，用于区分前置后置，并不实际参与函数的处理逻辑。我们实际上也不需要传这个参数。

| 符名   | 语法 | 可重载 | **原型示例（对于类** class T） |                                |
| ------ | ---- | ------ | ------------------------------ | ------------------------------ |
|        |      |        | 类内定义                       | 类外定义                       |
| 前自增 | ++a  | 是     | T& T::operator++();            | T& operator++(T& a);           |
| 前自减 | --a  | 是     | T& T::operator--();            | T& operator--(T& a);           |
| 后自增 | a++  | 是     | T T::operator++(int dummy);    | T operator++(T& a, int dummy); |
| 后自减 | a--  | 是     | T T::operator--(int dummy);    | T operator--(T& a, int dummy); |

## Overloading ++/-- Example

重载示例

Prefix ++/--

~~~c++
Vec2D& Vec2D:: operator ++(){
    x += 1; y += 1;
    retrun *this;
}
~~~

Postfix ++/--

~~~C++
Vec2D Vec2D:: operator ++(int dummy){//dummy 仅用于区分前置 or 后置，不实际参与实际的运算
    Vec2D temp{x,y};	
	x += 1; y += 1;
    retrun temp;
}
~~~

## Example

省略其它的函数，只保留 ++/-- 相关的。

主文件：test.cpp

~~~c++
#include <iostream>
#include "Vec2D.h"
int main(){
    Vec2D v1{1,2};
    std::cout << "++v1: " << (++v1).toString() <<std::endl;
    std::cout << "v1++: " << (v1++).toString() <<std::endl;
    std::cout << "  v1: " << v1.toString() <<std::endl;
    std::cout << "--v1: " << (--v1).toString() <<std::endl;
    std::cout << "v1--: " << (v1--).toString() <<std::endl;
    std::cout << "  v1: " << v1.toString() <<std::endl;
    
    return 0;
}
//output:
//++v1: (2.000000,3.000000)
//v1++: (2.000000,3.000000)
//  v1: (3.000000,4.000000)
//--v1: (2.000000,3.000000)
//v1--: (2.000000,3.000000)
//  v1: (1.000000,2.000000)
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
    
    //一元运算符重载 prefix ++
    Vec2D& operator ++();
    //一元运算符重载 postfix ++
    Vec2D operator ++(int dummy);
    //一元运算符重载 prefix --
    Vec2D& operator --();
    //一元运算符重载 postfix --
    Vec2D operator --(int dummy);
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

//一元运算符重载 prefix ++
Vec2D& Vec2D::operator ++(){
    x ++; y ++;
    return *this;
}
//一元运算符重载 postfix ++
Vec2D Vec2D::operator ++(int dummy){
    Vec2D temp{x,y};
    x ++; y ++;
    return temp;
}
//一元运算符重载 prefix --
Vec2D& Vec2D::operator --(){
    x --; y --;
    return *this;
}
//一元运算符重载 postfix --
Vec2D Vec2D::operator --(int dummy){
    Vec2D temp{x,y};
    x --; y --;
    return temp;
}
~~~

