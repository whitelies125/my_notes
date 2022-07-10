# Object Pointer & Dynamic Object

对象指针与动态对象

## Access Object Members via Pointers

用指针访问对象成员。

Object pointers can be assign a new object names.

对象指针可以指向新的对象名。

Arrow operator ->: Using pointer to access object members.

箭头运算符 ->：用指针访问对象。

~~~C++
#include <iostream>
#include "Circle.h"//把 radius 改为 public 先，不然下面访问 radius 报错。
using std::cout;
using std::endl;
int main(){
    Circle circle1;
    Circle* pcircle = &circle1;
    cout << "The radius is " << (*pcircle).radius << endl;
    cout << "The area is " << (*pcircle).getArea() << endl;

    (*pcircle).radius = 5.5;
    cout << "The radius is " << pcircle->radius << endl;
    cout << "The area is " << pcircle->getArea() << endl;
    return 0;
}
~~~

其实和以前 C 语言使用结构体 struct 做链表的访问一样的。

## Create Dynamic Object on Heap

在堆中创建对象。

Object declared in a function is created in the stack. When the function returns, the object is destroyed.

在函数中声明的对象都被创建在栈中。当函数返回时，对象被摧毁。

To retain the object, you may create it dymanically on the heap using the new operator.

为了保留对象，你可以使用 new 运算符在堆上创建它。

~~~C++
ClassName *pObject = new ClassName{};//用无参构造函数创建对象
ClassName *pObject = new ClassName{arguments};//用有参构造函数创建对象
~~~

程序结束时，动态对象会被销毁，或者使用 delete 显式销毁。

~~~C++
delete pObject;//使用 delete 显式销毁
~~~

顺带 C++11 标准中规定了，delete 一个空指针，是可以的，是安全的，不会发生什么事情。
