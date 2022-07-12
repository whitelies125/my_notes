# Copy Object and Anonymous Object

对象拷贝和匿名对象

## Class is a type 类是一种数据类型

原生数据类型定义变量，类定义对象。

编码规范：

Names representing types must be in mixed case starting with upper case.

代表类型的名字必须首字母大写并且其他字母大小写混合。

例如：Line，SavingAccount，Circle，... 都是类型名。

## Memberwise Copy 成员拷贝

How to copy the content from one object to the other?

如何将一个对象的内容拷贝给另一个对象？

- use the assignment operator: =

  使用赋值运算符：=

- By default, each data field of one object is copied to its counterpart in the other object.

  在默认情况下，对象中的每个数据域都会被拷贝到另一对象的对应部分。

例子：

~~~C++
circle_2 = circle_1;
//将 circle_1 的 radius 拷贝到 circle_2 中
//拷贝后，circle_1 与 circle_2 仍是两个不同对象，各自有一个自己的 radius 成员变量，但 radius 值都是相同的（circle_1 的 radius 的值）
~~~

## Anonymous Object 匿名对象

Occasionally, you may create an object and **use it only once**.

有时，创建一个对象但只使用它一次。

An object without name is called anonymous objects.

这种不命名的对象叫做匿名对象。

例子：

~~~C++
int main(){
    //使用匿名对象做拷贝列表初始化，右边的 Circle{} 并没有给它们名字
    Circle circle_1 = Circle{1.1};
    auto circle_2 = Circle{2.2};
    
    Circle circle_3{};//直接列表初始化，调用默认 ctor
    circle_3 = Circle{3.3};//用匿名对象赋值
    
    Cout << "Area is " << Circle{4.4}.getArea() << endl;
    
    return 0;
}
~~~

## class Replace struct 类代替结构体

The C language has the truct type for representing records.

C 语言中使用结构体来表示一组数据。

In C++, class has replaced struct.

在 C++ 中，结构体已被类取代。

~~~C++
//C
struct Student{
    int id;
    char name[30];
    char mi;
};
//C++
class Student{
public:
    int id;
    char name[30];
    char mi;
};
~~~

## Local class and Nested class 局部类和嵌套类

Local class: a class declared inside a function.

局部类：在一个函数中声明的类。

~~~C++
void func(){
    class C{ //C 及其对象只能在 func() 中可用，也就是作用域在 func(){} 内。
        ...
        void g(){//C 的成员函数只能在 C 中实现，
            ...//访问 func() 的成员受限。
        } 
    };
    C c1,c2;
}
~~~

Nested class: a class declared in another enclosing class.

嵌套类：在另一个类中声明的类。

~~~C++
class E{
    class N{ //N 及其对象可访问 E 的成员
        ...
    };
};
~~~

名字空间声明（namespace）也可以嵌套。

## 对象拷贝例子

~~~C++
#include <iostream>

class Square{
private:
    double side = 1.0;
public:
    Square() = default;//C++11中引入，强制编译器自动生成一个默认构造函数。
    //还有 Square() = delete; 表示禁止编译器自动生成一个默认构造函数。
    Square(double side){
        this->side = side;
    }
    double getArea(){
        return (side*side);
    }
};

int main(){
    Square s1,s2{4.0};
    std::cout << s1.getArea() << " " << s2.getArea() <<std::endl;
    std::cout << &s1 << " " << &s2 << std::endl;

    s1 = s2;
    std::cout << s1.getArea() << " " << s2.getArea() <<std::endl;
    std::cout << &s1 << " " << &s2 << std::endl;
}
//result:
//1 16
//0x61fe08 0x61fe00
//16 16
//0x61fe08 0x61fe00
~~~

## 匿名对象例子

~~~C++
#include <iostream>

using std::cout;
using std::endl;

class Account{
    double balance;
public:
    Account(){
        balance = 0.0;
    }
    Account(double balance_){
        balance = balance_;
    }
    void deposit(double amount){//存钱
        balance += amount;
    }
    double withdraw(double amount){//取钱
        if(balance < amount){
            double temp = balance;
            balance = 0.0;
            return temp;
        }else{
            balance -= amount;
            return amount;
        }
    }

};

int main(){
    Account a1;
    Account a2 = Account(100.0);
    a1.deposit(9.0);
    cout << a1.withdraw(10.0) << endl;
    cout << a2.withdraw(52.0) << endl;
    cout << Account(1000.0).withdraw(1001.0) << endl;
    return 0;
}
//output result:
//9
//52
//1000
//
~~~
