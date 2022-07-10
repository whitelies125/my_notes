# Delegating Constructor

## What is delegating constructor?

什么是代理构造/委托构造？

One ctor can call another ctor.

一个构造函数可以调用另外的构造函数。

~~~C++
class A{
private:
    int num1;
    int num2;
    int average;
public:
    A():A(0){};//调用构造函数 A(int i), 必须放在初始化列表中，不能放在函数体中
    A(int i):A(i,0){};//调用构造函数 A(int i, int j)，必须放在初始化列表中，不能放在函数体中
    A(int i, int j){
        num1 = i;
        num2 = j;
        average = (i+j)/2;
    }
}
//上面例子中，构造函数的调用次序:
//A()->A(int)->A(int, int)
~~~

## Avoid recursive calls of target constructors

避免递归调用目标 ctor。

递归调用：函数调用其本身。

~~~C++
class A{
private:
   int num1;
   int num2;
   int average;
public:
   A(): A(0){}
   A(int i): A(i, 0){}
   A(int i, int j): A(){}//这里形成环了。
};
//调用次序:
//A()->A(int)->A(int, int)-> 回到A()形成了环
~~~

委托构造的好处：让程序员少些一些代码，让程序逻辑更清晰一些。

自己简单瞎尝试一下：

~~~C++
#include <iostream>

class A{
public:
    int num1;
    int num2;
public:
    A():A(0){
        std::cout << "enter A()" << std::endl;
        num1 = 0;
        std::cout << "num1 = " << this->num1 << std::endl;
    }
    A(int i):A(i,1){
        std::cout << "enter A(int i)" << std::endl;
        num1 = 1;
        std::cout << "num1 = " << this->num1 << std::endl;
    }
    A(int i, int j){
        std::cout << "enter A(int i, int j)" << std::endl;
        num1 = 2;
        std::cout << "num1 = " << this->num1 << std::endl;
    }
};

int main(){
    A a{};
    std::cout << a.num1 << std::endl;
    std::cout << std::endl;
    A b{0};
    std::cout << b.num1 << std::endl;
    return 0;
}
//输出结果:
//enter A(int i, int j)
//num1 = 2
//enter A(int i)
//num1 = 1
//enter A()
//num1 = 0
//0
//
//enter A(int i, int j)
//num1 = 2
//enter A(int i)
//num1 = 1
//1
~~~

