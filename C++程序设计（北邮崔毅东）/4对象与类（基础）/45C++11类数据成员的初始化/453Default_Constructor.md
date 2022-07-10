# Default Constructor

默认构造函数。

A default constructor is a constructor which can be called with no arguments(either defined with an empty parameter list, or with default arguments provided for every parameter).

默认构造函数是可以无参调用的构造函数（既可以是定义为空参数列表的构造函数，也可以是所有参数都有默认数值的构造函数）。

~~~C++
class Circle{
private:
    double radius;
public:
    Circle(){//无参调用
        redius =1.0;//也可函数体为空。
    }
};

class Circle{
private:
    double radius;
public:
    Circle(double r = 1.0)//所有参数都有默认值
        : redius{r}{
    }
};
~~~

## The Role of Default Constructor

默认构造函数的角色。
也就是为什么需要默认构造函数。

If object type members/embedded objects are not initialized explicitly.

若对象类型成员/内嵌对象没有被显示初始化。

- the default constructor of the embedded object is automatically invoked. 

  该内嵌对象的无参构造函数会被自动调用。

- if a default constructor of the embedded object does not exist, a compilation error will be reported.

  若内嵌对象没有无参构造函数，则编译器报错。

~~~C++
class X{
private:
    Circle c1;//没有显式初始化
public:
    X(){}//所以会自动调用 Circle 的构造函数，构造 c1
}
~~~

You can use the Constructor Initializer to construct the object manually.

你也可以在初始化列表种手工构造对象。

~~~C++
class X{
private:
    Circle c1;
public:
    X()：c1{} {//手动显式调用 Circle 的构造函数，构造 c1
    }
}
~~~

## 自己的测试

~~~C++
#include <iostream>
class A{
public:
    int x;
    int y;
public:
    A(){}//默认构造函数
    A(int x = 1, int y = 2): x{x}, y{y} {}//所有参数都有默认值，也是默认构造函数
};

int main(){
    //A a;//报错，由于有两个默认构造函数，此时无法判断调用 A() 还是 A(int x = 1, int y = 2)
    A a{3};//正确，可以判断出调用 A(int x = 1, int y = 2)
    std::cout << a.x << " " << a.y << std::endl;//输出结果：3 2
    return 0;
}
~~~

