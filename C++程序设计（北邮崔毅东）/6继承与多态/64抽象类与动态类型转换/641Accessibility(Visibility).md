# Accessibility(Visibility)

访问控制（可见性控制）。

## The protected Keyword

the **private** and **public** keywords.

- To specify whether data fields and  functions can be accessed form the outside of the class.

  说明数据及函数是否可以从类外面访问。

- Private members can only be accessed from the inside of the class.

  私有成员只能在类内的函数访问。

- Public members can be accessed from any other classes.

  公有成员可被其它类访问。

A **protected** data field or a protected function in a base class can be accessed by name in its derived class.

保护属性的数据或函数可被派生类成员访问。

~~~C++
#include <iostream>
#include <iostream>
#include <string>

class A{
private:
    int i = 1;
protected:
    int j = 2;
public:
    int k = 3;
};
class B : public A{
    void print(){
        //std::cout << i << std::endl;//private，不可访问
        std::cout << j << std::endl;//protected，在子类中可以访问
        std::cout << k << std::endl;//public，在哪都能访问
    }
};

int main(){
    A a;
    //std::cout << a.i << std::endl;//private，不可访问
    //std::cout << a.j << std::endl;//protected，在子类中可以访问
    std::cout << a.k << std::endl;//public，在哪都能访问
    return 0;
}
~~~

## 公有继承

~~~C++
//class Derived : 派生方式 Base
class Derived : public Base{
    //...
}
~~~

此处的派生方式为 public 表示公有继承。

即子类会继承父类所有的 private，protected，public，并且其继承的成员的访问属性也不会改变。
同时，子类只能够访问父类的 protected，public，不能访问父类的 private。
可在类外通过子类对象访问子类继承自父类的 public 成员。因为子类继承的父类成员是 public 的仍然是 public。

## 私有继承

~~~C++
//class Derived : 派生方式 Base
class Derived : private Base{
    //...
}
~~~

此处的派生方式为 private 表示私有继承。

即子类会继承父类所有的 private，protected，public，并且其继承的成员的访问属性都变为 private。
同时，子类只能够访问父类的 protected，public，不能访问父类的 private。
不可在类外通过子类对象访问子类继承自父类的 public 成员。因为子类继承的父类成员都变为 private 了。

## 保护继承

~~~C++
//class Derived : 派生方式 Base
class Derived : protected Base{
    //...
}
~~~

此处的派生方式为 protected 表示保护继承。

即子类会继承父类所有的 private，protected，public，并且其继承的成员的访问属性原本为 public 的变为 protected，另外两种不变。
同时，子类只能够访问父类的 protected，public，不能访问父类的 private。
不可在类外通过子类对象访问子类继承自父类的 public 成员。因为子类继承的父类成员要么仍然是 private，要么变为了 protected。

对于私有继承，若孙类继承子类，由于子类继承父类的成员都变为 private，则孙类是无法访问子类继承自父类的成员的。
而对于保护继承，子类继承父类的成员仅将 public 变为 protected，所以孙类仍然是可以访问子类继承自父类的成员的。