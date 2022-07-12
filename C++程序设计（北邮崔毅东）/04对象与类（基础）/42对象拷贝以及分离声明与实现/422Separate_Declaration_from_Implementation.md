# Separate Declaration from Implementation

将声明与实现分离。

## How do we do in C?

将函数的原型（prototype）

~~~c++
float getArea(float radius);
~~~

放在头文件 GetArea.h 中。

将函数的实现（implementation）或者叫作函数的定义

~~~c++
#include "GetArea.h"
float getArea(float radius){
    return radius*radius*3.14;
}
~~~

放在 GetArea.cpp 中。

在 Main.cpp 中放主函数

~~~C++
#include "GetArea.h"
using namespace std;
int main(){
    cout << getArea(2.0) << endl;
    return 0;
}
~~~

## In C++

C++ allows you separate class declaration from implementation.

C++ 类声明与实现可以分离。

- .h 类声明，描述类的结构。

- .cpp 类实现，描述类方法的实现。

  FunctionType ClassName::FunctionName(Arguments){//...}

此处的 :: 称为**二元作用域解析运算符（binary scope resolution operator）**，简称**域分割符**。

前面在 361Scope Of Variables 学过一元作用域解析符。

## Inline Declaration & Inline Function

内联声明与内联函数。

When a function is implemented inside a class declaration, it automatically becomes an inline function.

当函数在类声明中实现，它自动成为内联函数。

~~~C++
class A{
public:
    A() = default;//C++11;
    double f1(){//声明时给出了实现，自动变为内联函数。等同于下面的f1()写法
        //do something
    }
    double f2();
}
double A::f2(){
    //do something
}
~~~

~~~C++
class A{
public:
    A() = default;//C++11;
    double f1()；
    double f2();
}
inline double A::f1(){//等同于上面 f1()的写法
    //do something
}
~~~

## 声明与实现分离例子

主文件：Main.cpp

~~~C++
#include <iostream>
#include "Circle.h"
int main(){
    Circle c1;
    Circle c2{2.0};
    std::cout << c1.getArea() << std::endl;
    std::cout << c2.getArea() << std::endl;
    return 0;
}
~~~

声明：Circle.h

~~~C++
#ifndef CIRCLE_H_
#define CIRCLE_H_
class Circle {
    double radius;
public:
    Circle();
    Circle(double newRadius);
    double getArea();
};
#endif
~~~

实现：Circle.cpp

~~~C++
#include "Circle.h"
Circle::Circle() {
    radius = 1;
}
Circle::Circle(double newRadius) {
    radius = newRadius;
}
double Circle::getArea() {
    return radius * radius * 3.14159;
}
~~~

注意：声明与实现的文件名除扩展名 .h .cpp 外，不是必须完全相同。是可以不同名的。

根据我自己的尝试，在 Code::Blocks 的一个 C++ project 中，进行 build and run 时，project 目录下 obj 文件夹中会生成 main.o（来自 main.cpp） 和 Circle.o（来自 Circle.cpp） 文件。

根据网上的信息，此后，编译器会在链接时，自动地寻找函数声明对应的实现。
通常，寻找时，编译器会对 project 中的每个 .o 或 .obj 文件都进行寻找。
当然也可以由程序员来指定。

我实际测试，Main.cpp，Circle.h，Circle.cpp 与 Main.cpp，Circle.h，Circle_no_same_name.cpp 都是可以通过的。
