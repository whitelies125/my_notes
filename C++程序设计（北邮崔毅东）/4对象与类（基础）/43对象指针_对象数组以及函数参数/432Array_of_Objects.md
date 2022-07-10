# Array of Objects

对象数组。

声明方式举例：

最简单的方式：

~~~C++
Circle c1[10];
//c1 中的 10 个对象调用默认的构造函数初始化完成
~~~

使用匿名对象构成的列表初始化：

~~~C++
Circle c2[3] = {
    Circle{3},
    Circle{},
    Circle{5}
};
~~~

使用 C++11 列表初始化，但列表成员为隐式构造的匿名对象：

~~~C++
Circle c3[3] {3.1, {},5};//直接列表初始化
Circle c4[3] = {3.1, {},5};//有个等号，拷贝列表初始化
~~~

使用 new 在堆区生成对象数组：

~~~C++
Circle* p1 = new Circle[3];
Circle* p2 = new Circle[3] {3.1, {},5};
delete [] p1;
delete [] p2;
p1 = p2 = nullptr;
~~~

## 对象数组代码示例

非常简单的例子，创建对象数组，修改对象成员数据，再输出看一下。

Circle.h

~~~C++
#ifndef CIRCLE_H_
#define CIRCLE_H_
class Circle {
    double radius;
public:
    Circle();
    Circle(double newRadius);
    double getArea();
    double getRadius() const;//对应的，声明里也是要加上 const 的
    void setRadius(double radius);
};
#endif
~~~

Circle.cpp

~~~C++
#include "Circle.h"
Circle::Circle() {
    radius = 1;
}
Circle::Circle(double newRadius) {
    radius = newRadius;
}
double Circle::getArea() {
    return radius * radius * 3.14159;
}
double Circle::getRadius() const{// 这里的 const 放在函数参数括号()后，函数体花括号{}前，表示函数为常函数，表示该函数不改变当前对象的状态，即在该函数中没有去修改数据成员的代码。
    //如果在这里面加上 radius = 1.0; 这样修改数据成员的代码，则会报错。
    return radius;
}
void Circle::getRadius(double radius){
    this->radius = radius;
}
~~~

Main.cpp

~~~C++
#include <iostream>
#include "Circle.h"
using std::cout;
using std::endl;
int main(){
    Circle c1[] {Circle{1.0},Circle{2.0},Circle{3.0}};
    Circle c2[] {10.0,11.0,12.0};
    c1[2].setRadius(4.0);
    c2[1].setRadius(100.0);
    for(auto it : c1) cout << it.getRadius() << " ";
    cout << endl;
    for(auto it : c2) cout << it.getRadius() << " ";
    return 0;
}
~~~



