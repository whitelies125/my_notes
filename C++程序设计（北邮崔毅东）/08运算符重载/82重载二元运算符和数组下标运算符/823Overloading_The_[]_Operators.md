# Overloading The [] Operators

重载 [] 运算符。

## Overloading The [] Operators

重载 [] 运算符。

array subscript [] can be overloaded to access the contents of the object using the array-like syntax.

[] 重载后可用类似数组的语法格式访问对象内容。

~~~C++
Vec2D v{8,9};
std::cout << "v.x: " << v[0] << "v.y: " << v[1] <<std::endl;
~~~

## [] accessor and mutator

数组下标运算符可作为访问器和修改器。

The [] operator functions as both accessor and mutator.

[] 运算符可既读又写。

- accessor: double a = v[0];
- mutator: v[1] = 3.0;

How to make v[] an Lvalue?

如何使 v[] 成为左值？

- declare the operator to return a reference.

  使 [] 返回一个引用。

所以，我们可以这样实现重载 []：

~~~C++
double& Vec2D::operator [](const int& index){
    if(index == 0) return x;
    else if(index == 1) return y;
    else std::cout << "index out of bound" << std::endl;
    exit(0);
}
~~~

## Example

省略其它的函数，只保留 [] 相关的了。

主文件：test.cpp

~~~c++
#include <iostream>
#include "Vec2D.h"
int main(){
    Vec2D v{8,9};
    v[0] = 1;
    v[1] = 2;
    std::cout << "v.x: " << v[0] << " v.y: " << v[1] <<std::endl;
    return 0;
}
//output:
//v.x: 1 v.y: 2
~~~

头文件：Vec2D.h

~~~c++
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

    //读取或者修改向量元素
    double& at(const int index);

    //运算符重载 []
    double& operator [](const int index);
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

//读取或者修改向量元素
double& Vec2D::at(const int index){
    if(index == 0) return x;
    else if(index == 1) return y;
    else std::__throw_out_of_range("at() only accept 0 or 1 as parameter.");
}

//运算符重载 []
double& Vec2D::operator [](const int index){
    return at(index);
}
~~~

