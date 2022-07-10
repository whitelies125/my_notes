# Redefining Functions

重定义函数。

因为有了继承中的名字隐藏，所以我们可以在子类中重新定义一个和基类中名字、参数完全相同的函数。以实现一些功能。

例如，在前面的 Shape，Rectangle，Circle 的例子中，

~~~C++
//Shape.h

//...
class Shape{
private:
    Color color{Color::black};
    bool filled{false};
public:
	//...
    std::string info() const {
        using namespace std::string_literals;
        std::array<std::string,6> s {"white"s, "black"s, "red"s, "green"s, "blue"s, "yellow"s};
        return "Shape : " + s[static_cast<int>(color)] + " " + (filled ? "filled"s : "not filled"s);
    }
};
~~~

Shape 中的 info() 函数可以输出参数 color，filled 的信息。

而 Shape::info() 被 Circle 继承，因此可以通过 Circle 对象调用 info() 输出 Circle 对象的 color，filled 的信息，即 Circle.info()。

但这样的方式无法输出 Circle 对象的其它信息。

由于继承中的名字隐藏，所以可以在派生类中重新定义 info() 函数以描述派生类的其它信息。

这样，调用 Circle.info() 便会调用的是我们在 Circle 类中重定义的 info()，而不是基类 Shape 中的 info()。

## Example

简化后的例子。

~~~C++
#include <iostream>
class Shape{
public:
    void info(){ std::cout << "Shape" << std::endl; }
};
class Rectangle : public Shape {};
class Circle : public Shape {
public:
    //redifine
    void info(){ std::cout << "Circle" << std::endl; }
    void g(){
        info();//默认为 Circle 中的 info
        Shape::info();//有了重定义函数后，访问基类中的同名函数，需指明。
    }
};

int main(){
    Rectangle r;
    r.info();
    Circle c;
    c.info();
    c.Shape::info();//如果需访问基类中的同名函数，只要指明。
    return 0;
}
//output:
//Shape
//Circle
//Shape
~~~

## Redefine v.s. Overload

重定义与重载。

#### Overload functions

- more than one function with the same name.

  多个函数名字相同。

- But different in at least one of the signatures:

  但至少一个特征不同：

  - parameter type 参数类型
  - parameter number 参数数量
  - parameter sequence 参数顺序

#### Redefine Function

- The functions have the same signature

  函数特征相同

  - Name 同名
  - Parameters(including type, number and sequence) 同参数（类型，数量，顺序 ）
  - Return type 同返回值类型。

- Defined in base class and derived calss, respectively.

  在基类和派生类中分别定义。