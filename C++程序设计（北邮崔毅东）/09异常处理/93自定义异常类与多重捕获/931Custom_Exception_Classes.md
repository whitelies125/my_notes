# Custom Exception Classes

自定义异常类。

## How to create user-defined exception class?

如何创建自定义的异常类？

Thsi class is juet like any C++ class.

这个类与其它 C++ class 是类似的。

often it is derived from exception or its descendants.

通常由 exception 或其后代类派生。

## Why to derive from exception?

为什么从 exception 派生？

To utilize the common fearutes in the excetion class.e.g. the what() function.

为了使用通用的东西，比如 what() 函数。

## Priority of using exception classes

优先使用哪种异常类。

Predefined exception classes of C++.(whenever possible)

优先使用 C++ 预定义好的异常类。

Creating your own exception classes.(the less,the better)

其次才是自定义的异常类。

- merely when you run into a problem, and the predefined exception classes cannot describe it adequately.

  仅仅当你遇到了一个问题，且预定义好的异常类无法替代性的描述该信息。

## Example

test.cpp

~~~C++
#include <iostream>
#include <exception>
#include "Vec3D.h"
int main(){
    Vec3D v1{1.1, 2.2, 3.3};
    try{
        std::cout << v1[3] << std::endl;//故意越界
    }catch(std::exception &e){//使 RangeException 从 exception 类继承
        std::cout << e.what() << std::endl;
    }
    return 0;
}
//output
//out of range
~~~

Vec3D.h

~~~C++
//创建 Vec2D 类
//当访问越界时，抛出自定义异常类 RangeException
#include <array>
#include "RangeException.h"
class Vec3D{
private:
    std::array<double,3> v{1.0, 1.0, 1.0};
public:
    Vec3D() = default;
    Vec3D(double x, double y, double z) : v{x, y, z}{};
    ~Vec3D() = default;
    double& operator[] (const int index){
        if(0 <= index && index <=2) return v[index];
        else throw RangeException(3,index);//在这里 throw
    }
};
~~~

RangeException.h

~~~C++
//自定义异常类 RangeException, 从 exception 继承
#include <exception>
#include <iostream>
class RangeException : public std::exception{
private:
    std::size_t dimension{3};
    int index{0};
public:
    RangeException(std::size_t dimension, const int index)
    : dimension{dimension}, index{index}
    {};
    ~RangeException() = default;
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "out of range";
    }
};
~~~

