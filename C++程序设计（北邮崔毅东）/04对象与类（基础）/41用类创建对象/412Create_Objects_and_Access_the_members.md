# Create Objects and Access the members

## Constructors 构造函数

- Automatic invocation. 创建时自动调用。
- Has the same name as the defining class. 与类名同名。
- NO return value(including "void"). 无返回值（包括 void）。
- Can be overloaded. 可以重载。
- May have no arguments. 可以无参数。

~~~C++
class Circle {
    public:
      // The radius of this circle
      double radius;
      // Construct a circle object
      Circle() { //创建时自动调用；与类同名；无返回值，前面 void 也不写；可重载；无参
        radius = 1;
      }
      // Construct a circle object
      Circle(double newRadius) { //创建时自动调用；与类同名；无返回值，前面 void 也不写；可重载；无参
        radius = newRadius;
      }
      // Return the area of this circle
      double getArea() {
        return radius * radius * 3.14159;
      }
};
~~~

A class may be declared without ctors: 类可以不声明构造函数：

1. A no-arg constructor with an empty body is implicitly declared in the class.

   编译器会提供一个带有空函数体的无参构造函数。

2. This constructor, called **a default constructor** is provided automatically **only if no constructors are explicitly declared in the class**. 

   只有当未明确声明构造函数时，编译器才会提供这个构造函数，并成为**默认构造函数**。

~~~C++
class Circle{
    public:
        double radius;
};
// 没写构造函数，编译器自动提供一个带有空函数体的无参构造函数。
//相当于：
class Circle{
    public:
        double radius;
        Circle(){}
};
~~~

## Create Objects 创建对象

仍然使用上面 Circle 类的例子。

~~~C++
Circle circle_1; //不带参数
Circle circle_2(5.5); //带参数
Circle circle_3{5.5}; //带参数，这个老师喜欢用列表初始化，进行窄化检测 narrowing check
~~~

## Object Member Access Operator 对象访问运算符

To access the data & functions of an object: 访问对象中的数据和函数：

- The dot operator (.), namely, the object member access operator.

  点运算符，也成为对象成员访问运算符。

~~~C++
Circle circle_1;
circle_1.radius = 10;
int area = circle_1.getArea();
~~~

在这段代码中，circle_1 是 Circle 类的一个实例，radius 称为**实例变量（instance function）**，getArea() 称为**实例函数（instance function）**。

其他：

顺带一提，如果类里面不写 public 之类的，则默认其成员都是 private。
