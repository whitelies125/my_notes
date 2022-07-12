# Inheritance

继承。

继承链上的类的对应叫法：

Parent Class 父类，Child Class 子类。
Base Class 基类，Derived Class 派生类。
Super Class 超类，Sub Class 子类。

继承 vs 泛化：

Inherit 继承：子继承父。子类具有父类的特点
Generalize 泛化：父泛化子。父类具有子类共同的特点。

子类包含父类的大部分成员。
为啥不是全部成员，老师说后面学到会解释。

## 避免一个类被继承（C++11）

C++11 引入 **final** 特殊标识符，可以使得类不能被继承。

~~~C++
class B final {  
};
class D : public B {
};
//会提示报错：不能将 final 类类型用作基类
~~~

## Example

父类：Shape

~~~C++
//Shape.h
#pragma once
#include <array>
#include <string>

enum class Color{
    white, black, red, green, blue, yellow
};

class Shape{
private:
    Color color{Color::black};
    bool filled{false};
public:
    Shape() = default;
    Shape(Color color, bool filled) : color{color}, filled{filled} {}
    Shape(const Shape&) = default;
    ~Shape() = default;

    Color get_color() const { return color; }//const 在这个位置表示该函数不修改对象。
    bool is_filled() const { return filled; }
    void set_color(Color color) { this->color = color; }
    void set_filled(bool filled) { this->filled = filled; }
    std::string info() const {
        using namespace std::string_literals;
        std::array<std::string,6> s {"white"s, "black"s, "red"s, "green"s, "blue"s, "yellow"s};
        return "Shape : " + s[static_cast<int>(color)] + " " + (filled ? "filled"s : "not filled"s);
    }
};
~~~

子类：Circle

~~~C++
//Circle.h
#pragma once
#include "Shape.h"
class Circle : public Shape{//从 Shape 继承
private:
    double radius;
public:
    Circle();
    Circle(double radius);
    void setRadius(double radius);
    double getRadius() const;//
    double getArea() const;
};
//Circle.cpp
#include "Circle.h"
Circle::Circle() : radius{1} {}
Circle::Circle(double radius) : radius{radius} {}

void Circle::setRadius(double radius){ this->radius = radius; }
double Circle::getRadius() const{ return radius; }
double Circle::getArea() const { return radius * radius * 3.14159; }
~~~

子类：Rectangle

~~~C++
//Rectangle.h
#pragma once
#include "Shape.h"
class Rectangle : public Shape{//从 Shape 继承
private:
    double width{1.0};
    double height{1.0};
public:
    Rectangle() = default;
    Rectangle(double width, double height);
    Rectangle(const Rectangle& ) = default;
    ~Rectangle() = default;

    void set_width(double width);
    void set_height(double height);
    double get_width() const;
    double get_height() const;
    double get_area() const;
};
//Rectangle.cpp
#include "Rectangle.h"
Rectangle::Rectangle(double width, double height) : width{width}, height{height} {};

void Rectangle::set_width(double width) { this->width = width; }
void Rectangle::set_height(double height) { this->height = height; }
double Rectangle::get_width() const { return width; }
double Rectangle::get_height() const { return height; }
double Rectangle::get_area() const { return width * height; }
~~~

主函数：test.cpp

~~~C++
#include <iostream>
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"

int main(){
    Shape s1 = Shape(Color::blue, false);
    Circle c1 {1.0};
    Rectangle r1 {1.0, 2.0};

    std::cout << s1.info() << std::endl;
    std::cout << c1.info() << std::endl;//注意到，虽然没有在 Circle 或 Rectangle 中定义 info() 函数，但仍然可用。
    std::cout << r1.info() << std::endl;//因为 Circle 或 Rectangle 都继承自 Shape，自动地拥有了 Shape 的数据成员和成员函数。
    return 0;
}
//output:
//Shape : blue not filled
//Shape : black not filled
//Shape : black not filled

//可见，c1,r1 输出的是就地初始化的值。
~~~

虽然我觉得，可以创建 Circle 或 Rectangle 对象后，在通过 setter 来设置 color 和 filled 的值。

但这里老师的举例是，进行一些修改，使得 Circle 和 Rectangle 在初始化时就能够设置 color 和 filled 的值。

那就照着他来吧。

子类：Circle

~~~C++
//Circle.hh
#pragma once
#include "Shape.h"
class Circle : public Shape{//从 Shape 继承
private:
    double radius;
public:
    Circle();
	Circle(double radius, Color color, bool filled);//多接受两个参数 color，filled
    void setRadius(double radius);
    double getRadius() const;//
    double getArea() const;
};
//Circle.cpp
#include "Circle.h"
Circle::Circle() : radius{1} {}

//Circle::Circle(double radius, Color color, bool filled)
//    : radius{radius}, color{color},filled{filled} {}
//这样写是不行的，会提示 color，filled 不可访问。
//color，filled 是 Shape 的私有成员，只有在 Shape 类中才能够访问。
//即便 Circle 是 Shape 的子类，也不能访问。

Circle::Circle(double radius, Color color, bool filled)
    : Shape{color, filled}, radius{radius} {}
//这样可以，使用父类的构造函数来初始化数据成员。

void Circle::setRadius(double radius){ this->radius = radius; }
double Circle::getRadius() const{ return radius; }
double Circle::getArea() const { return radius * radius * 3.14159; }
~~~

子类：Rectangle

~~~C++
//Rectangle.h
#pragma once
#include "Shape.h"
class Rectangle : public Shape{//从 Shape 继承
private:
    double width{1.0};
    double height{1.0};
public:
    Rectangle() = default;
    Rectangle(double width, double height, Color color, bool filled);//多接受两个参数 color，filled
    Rectangle(const Rectangle& ) = default;
    ~Rectangle() = default;

    void set_width(double width);
    void set_height(double height);
    double get_width() const;
    double get_height() const;
    double get_area() const;
};
//Rectangle.cpp
#include "Rectangle.h"
Rectangle::Rectangle(double width, double height, Color color, bool filled)
    : Shape{color, filled}, width{width}, height{height} {};

void Rectangle::set_width(double width) { this->width = width; }
void Rectangle::set_height(double height) { this->height = height; }
double Rectangle::get_width() const { return width; }
double Rectangle::get_height() const { return height; }
double Rectangle::get_area() const { return width * height; }
~~~

主函数：test.cpp

~~~C++
#include <iostream>
#include "Shape.h"
#include "Circle.h"
#include "Rectangle.h"
int main(){
    Shape s1 = Shape(Color::blue, false);
    Circle c1 {1.0, Color::green,true};
    Rectangle r1 {1.0, 2.0, Color::white, true};

    std::cout << s1.info() << std::endl;
    std::cout << c1.info() << std::endl;
    std::cout << r1.info() << std::endl;
    return 0;
}
//output:
//Shape : blue not filled
//Shape : green filled
//Shape : white filled
~~~



