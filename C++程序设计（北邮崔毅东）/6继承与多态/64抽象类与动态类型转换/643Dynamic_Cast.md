# Dynamic Cast

动态类型转换。

## Dynamic Cast-Why

为什么需要动态类型转换。

例子：

~~~C++
//p 可能为 Circle 或 Rectangle 对象
//getArea() 为虚函数
void print(Shape& p){
    std::cout << p.getArea() << std::endl;
}
~~~

上面这个 print() 函数，因为运行时多态，所以可以做到根据 p 的实际对象调用对应的 getArea()。

那如果我们想要这个函数新增一个功能， 使得如果 p 的实际对象为 Circle 类，输出其数据成员 radius；如果为 Rectangle 类，输出其数据成员 width，height。

## Dynamic Casting Example

**dynamic_cast** 运算符

- 沿继承层级向上、向下及侧向转换到类的指针或引用。
- 转指针：成功返回对象指针，失败返回 nullptr。
- 转引用：成功返回对象引用，失败抛出异常。

将 Shape 对象 dynamic_cast 动态转换为 Circle 对象，就能够调用 Circle 对象中独有的函数。

~~~C++
//p 可能为 Circle 或 Rectangle 对象
//getArea() 为虚函数
void print(Shape& p){
    std::cout << p.getArea() << std::endl;
    
    Shape *q = &p;
    Circle *c = dynamic_cast<Circle*>(q);
    //我实测直接用 p 就行，没必要多个中甲变量 q。
    //Circle* c = dynamic_cast<Circle*>(p);
    
	//用引用，失败会抛出异常
    //Circle &c = dynamic_cast<Circle*>(p);

    if(c != nullptr){//转换成功
        std::cout << c->getRadius() << std::endl;
    }
}
~~~

自己的实测：

~~~C++
#include <iostream>
#include <string>

class Shape{
public:
    int k = 3;
    virtual double getArea() const = 0;
};
class Circle : public Shape{
private:
    double radius = 1;
public:
    virtual double getArea() const {
        return 3.14 * 3.14 * radius;
    }
    double get_radius() const { return radius; }
};
class Rectangle : public Shape{
private:
    double width = 2;
    double height = 3;
public:
    virtual double getArea() const {
        return width * height;
    }
    double get_wigth() const { return width; }
    double get_height() const { return height; }
};

void print_pointer(Shape* p){
    std::cout << p->getArea() << std::endl;
    Circle* c = dynamic_cast<Circle*>(p);
    if(c != nullptr){
        std::cout << c->get_radius() << std::endl;
    }else{
        std::cout << "cast fail" << std::endl;
    }
}

void print_reference(Shape& p){
    std::cout << p.getArea() << std::endl;
    Circle& c = dynamic_cast<Circle&>(p);
    std::cout << c.get_radius() << std::endl;
}

int main(){
    Circle c;
    Rectangle r;
    print_pointer(&c);
    print_pointer(&r);
    print_reference(c);
    print_reference(r);
    return 0;
}
//output
//9.8596
//1
//6
//cast fail
//9.8596
//1
//6
//terminate called after throwing an instance of 'std::bad_cast'
//  what():  std::bad_cast
~~~



