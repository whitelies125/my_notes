# Default Type and Non-type Parameters

默认类型与非类型参数

## Default type

默认类型。

#### Review：default parameter value.

回顾：参数的默认值。

~~~C++
int circle(int x, int y, int r = 100);
~~~

在模板的类型参数中也有类似的操作。

#### Default type parameter.

默认类型参数。

You can assign a default type for a type parameter in a class tamplate.

可以为类模板的类型参数指定一个默认类型。

例如，指定泛型 Stack 的默认类型参数为 int：

~~~C++
template <typename T = int>
class Stack{
    //...
}
~~~

使用默认类型定义一个对象：

~~~C++
Stack<> stack;//等同于 Stack<int> stack;
~~~

**注意**：默认类型仅可在类模板中使用，函数模板不能使用默认类型。

## Non-type Parameters

非类型参数。

Using non-type  parameters in a template prefix.

在模板前缀中使用非类型参数。

**注意**：非类型模板参数会带来某些限制。通常，他们只可以是：

- lvalue reference type (to object or to function); 
- an integral type;
- a pointer type (to object or to function); 
- a pointer to member type (to member object or to member function);
- an enumeration type;
- std::nullptr_t;(since C++11)
- a floating-point type;(since C++20)
- a literal class type with the following properties:(since C++20)
  - all base classes and non-static data members are public and non-mutable and
  - the types of all base classes and non-static data members are structural types or (possibly multi-dimensional) array thereof.

翻译：

- 对象或函数的左值引用；
- 一个整型；
- 指向成员（成员对象或成员函数）的指针；
- 一个枚举类型；
- std::nullptr_t（C++11）；
- 一个浮点数类型
- 满足下列属性的字面类型：
  - 所有基类和非静态数据成员都是公共的和不可变的，
  - 所有基类和非静态数据成员的类型都是结构类型或（可能是多维的）数组。

例如：整型作为非类型参数

~~~C++
template<typename T, int capacity>
class Stack{
private:
    T elements[capacity];
    int sieze;
    //...
}

Stack<int,100> intStack;
~~~

又如：对象作为非类型参数

~~~c++
template <typename T, Color C>
class Lable{
	//...
}

Color color{0,0,255};
Lable<char,color> label;
~~~

同时，非类型参数也可以使用默认值：

~~~C++
template<typename T = int, int capacity = 100>
class Stack{
private:
    T elements[capacity];
    int sieze;
    //...
}
~~~

