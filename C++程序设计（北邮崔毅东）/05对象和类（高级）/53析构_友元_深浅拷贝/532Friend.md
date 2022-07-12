# Friend

友元。

## Why Friend

为何需要友元。

Private members: Cannot be accessed from outside of the class.

私有成员：无法从类外访问。

Occasionnaly, it is convenient to allow some trusted functions and classes to access a class's private members.

有时，允许一些信任的函数或类访问这些私有成员会很方便。

## Friend functions and classes

友元函数和友元类。

C++ enables you to use the **friend** keyword to declare **friend functions** and **friend classes** for a class.

C++ 使你可以使用 friend 关键字来声明友元函数或友元类。

Disadvantage of friend: break the encapsulation.

友元的缺点：打破的封装。

## 自己的测试及补充

Friend 就像是授权一样，允许其它类或者函数访问它的私有成员。

友元声明只能在类的定义的内部，但在类中出现的位置不限，放哪都行。

在类中使用友元修饰的函数或类，并不属于该类的成员，也不受所在区域访问控制级别的约束（所以说在类中的友元声明放哪都行嘛，不受 private，pubilic 的影响）。

友元函数：

~~~C++
#include <iostream>
class Point{
//声明 int distance(Point &a,Point &b); 是 Point 的友元函数
//感觉就是授权给 int distance(Point &a,Point &b) 可以访问 Point 类对象的私有成员
//这个声明放哪都可以，放在 private public 里面都行，没影响
friend int distance(Point &a,Point &b);
private:
    int x;
    int y;
public:
    Point()
    : Point(0, 0)
    {}
    Point(int x, int y)
    : x{x}, y{y}
    {}
};
//该函数为 Point 类的友元函数，可以在这个函数中，可直接访问 Point 类对象的私有成员。
int distance(Point &a,Point &b){//返回两点间距离的平方，懒得开方，所以就返回距离平方吧
    int x = a.x - b.x;
    int y = a.y - b.y;
    return x*x + y*y;
}
int main(){
    Point a{0,0};
    Point b{1,2};
    //std::cout << a.x << std::endl;//编辑器提示 x 不可访问
    std::cout << distance(a,b) << std::endl;//在 Point 类的友元函数里访问
    return 0;
}
~~~

友元类：

友元类的所有成员函数都是另一个类的友元函数。

注意，声明 B 是 A 的友元类，是单向的，并不等于 A 同时也是 B 的友元类。此时 B 可访问 A 的私有成员，但 A 是不能访问 B 的私有成员的。

~~~C++
#include <iostream>
class A{
friend class B;//声明 B 是 A 的友元类，友元类 B 中的所有成员函数都是 A 的友元函数。
friend void print(A &a);//声明 print() 是 A 的友元函数。
//显然，上面只是声明，实际上 B，void print(A &a)，都并不属于 A
private:
    int x; int y;
public:
    A() = default;
    A(int x, int y): x{x}, y{y} {}
};

class B{
private:
    int x; int y;
public:
    B() = default;
    B(int x, int y) : x{x}, y{y} {}
    void set_A_x_y(A &a,int x, int y){
        a.x = x; a.y = y;
    }
};

void print(A &a){
    std::cout << a.x << " " << a.y << std::endl;
}

int main(){
    A a{0,0};
    print(a);//输出：0 0
    B b{1,1};
    //a.x = 1;//编辑器提示 x 不可访问
    b.set_A_x_y(a,2,2);//修改 a 的私有成员，因为 B 是 A 的友元类，所以其成员函数也是 A 的友元函数，可以修改。
    print(a);//输出：2 2
    return 0;
}
~~~

