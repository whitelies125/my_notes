# The Scope of Members and "this" Pointer

## The Scope of Data Members in Class 

数据成员的作用域。

The data members are accessible to all constrctor and function in the class.

数据成员可被类内所有函数访问。

Data fields and functions can be decalared in any order in a class.

数据域与函数可按任意顺序声明。

~~~C++
//任意顺序一
class Circle{
public:
    Circle();
    Circle(double);
    double getArea();
    double getRadius();
    void setRadius(double);
private:
    double radius;
};
//任意顺序二
class Circle{
public:
    Circle();
    Circle(double);
private:
    double radius;
public:
    double getArea();
    double getRadius();
    void setRadius(double);
};
//任意顺序三
class Circle{
private:
    double radius;
public:
    double getArea();
    double getRadius();
    void setRadius(double);
public://当然，此处 public 也可不写
    Circle();
    Circle(double);
};
~~~

## Hidden by same name

同名屏蔽。

If a local variable has the same name as a data field:

如果成员函数中的局部变量与某数据域同名：

- the local variable takes precedence.

  局部变量由优先级高：就近原则。

- the data field with the same name is hidden.

  同名数据域在函数中被屏蔽。

当然，最好还是不要起同名参数。

## The this Pointer

this 指针。

How do you reference a class's hidden data field in a function?

如何在函数中访问类中被屏蔽的数据域？

**this** point:

**this** 指针：

- a special built-in pointer.

  特殊的内建指针。

- reference the calling object.

  指向当前函数的调用对象。

~~~C++
class Circle{
private:
    double radius;
public:
    Circle();
    Circle(double radius){//局部变量与数据域同名
        this->radius = radius;//使用指向调用该函数的对象的 this 指针访问该对象的数据域。
    }
};
~~~

## Simple way to avoid same hidden

避免同名屏蔽的简单方法。

~~~C++
class Circle{
private:
    double radius;
public:
    Circle();
    Circle(double radius_){//加一个后缀 _ 避免同名就好啦。
		radius = radius_;
    }
};
~~~

