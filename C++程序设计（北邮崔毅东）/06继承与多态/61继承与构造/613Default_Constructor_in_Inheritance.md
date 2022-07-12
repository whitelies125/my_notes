# Default Constructor in Inheritance

继承中的默认构造函数。

## No-Arg Constructor in Base Class

基类的无参构造函数。

If a base constructor is not invoked explicitly, the base class's no-arg constructor is invoked by default.

如果基类 ctor 没有被显式调用，则基类的无参默认构造函数会被调用。

~~~C++
//父类 Shape,子类 Circle
Circle(){ radius = 1.0;}//会调用基类 Shape 的默认构造函数 Shape()。
//等价于
//Circle : Shape{}{ radius = 1.0; }

Circle(double r){ radius = r; }//会调用基类 Shape 的默认构造函数 Shape()。
//等价于
//Circle(double r) : Shape{} { radius = r; }
~~~

所以，当从基类派生出来一个派生类的时候，最好是给基类提供默认构造函数。

## 一些编码规范

- 头文件用 .h，源文件用 .cpp(c++,cc 也可)。

- A class should be declared in a header file and defined in a source file where the name of the files match the name of the class.

  类应该在头文件中声明，并在源文件中定义，俩文件名字应该与类名相同。

  例外的是，模板类的声明和定义都要放在头文件中。

- Class variabels should never be declared public.

  类成员变量不可被声明为 public。

## Example

~~~C++
#include <iostream>
class A{
public:
    //如果注释掉 A()，则会报错。因为 B() B(int i) 会隐式的调用 A()
    //而 A() 被注释掉了，同时我们已经写了一个构造函数 A(int i)，所以编译器并不会帮自动帮我们生成一个无参默认构造函数。A() 就找不到了。
	//这也是为什么上述说 "最好是给基类提供默认构造函数"。
    A() { std::cout << "A()" << std::endl; }
    A(int i) { std::cout << "A(int i)" << std::endl; }
};
class B : public A{
public:
    B() { std::cout << "B()" << std::endl; }//会调用 A();
    B(int i) { std::cout << "B(int i)" << std::endl; }//会调用 A();
};
int main(){
    B b1{};
    B b2{1};
    return 0;
}
//output:
//A()
//B()
//A()
//B(int i)
~~~

如果希望调用的不是无参默认构造函数，则在初始化列表指明就好。

~~~C++
#include <iostream>
class A{
public:
    A() { std::cout << "A()" << std::endl; }
    A(int i) { std::cout << "A(int i)" << std::endl; }
};
class B : public A{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) : A(i) { std::cout << "B(int i)" << std::endl; }
};
int main(){
    B b1{};
    B b2{1};
    return 0;
}
//output:
//A()
//B()
//A(int i)
//B(int i)
~~~

