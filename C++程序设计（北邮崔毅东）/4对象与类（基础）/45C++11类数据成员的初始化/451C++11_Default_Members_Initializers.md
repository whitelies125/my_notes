# C++11: Default Members Initializers

类成员的就地初始化。

## What is Default Members Initializers

什么是就地初始化。

就地初始化的英文对应多个，如 In-class initializer, default member initializer 都是就地初始化。

顾名思义，就地初始化也就是类中的数据域在声明时就初始化。

In C++03, only static const members of integral types could be initialized in-class.

在 C++03 标准中，只有静态常量整型成员才能在类中就地初始化。

~~~C++
//C++03
class X{
    static const int a = 1;//ok
    cont int b = 2;//no,non-static
    static c = 3;//no,no-const
    static const string d = "ddd";//no,non-int
};
~~~

C++11 was to allow a non-static data member be initialized where it is declared in its calss.

C++11 标准中，非静态成员可以在它声明的时候初始化。

## Example and Rules

例子和规则。

~~~C++
class S{
    int m = 1;//ok,copy-initializes m
    int n(2);//no,不允许在类中使用小括号的列表初始化，因为会与函数混淆。
    int o {2};//ok,使用花括号列表初始化可以，毕竟这样不会与函数混淆了。
    std::string s{'a','b','c'};//ok
    std::string t{"Constructor run"};//ok
    int a[] = {1,2,3};//no,类中的数组类型成员不能自动推断大小
    int b[3] = {1,2,3};//ok
};
~~~

