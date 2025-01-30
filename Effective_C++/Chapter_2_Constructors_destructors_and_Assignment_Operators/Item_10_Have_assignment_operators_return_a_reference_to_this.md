条款 10：令 operator= 返回一个 reference to \*this

C++ 中可以使用如下连续赋值的语法：
~~~cpp
int x, y, z;
x = y = z = 15; // 连续赋值
~~~
这是因为赋值语句是遵循右结合律的，所以上述语句被解析为：
~~~cpp
x = (y = (z = 15))
~~~
15 赋值给 z，然后该赋值操作的结果（仍然是 15）赋值给 y，然后该赋值操作的结果（仍然是 15）赋值给 x。
实现这种连续赋值的方法就是，返回左侧参数的引用。

~~~cpp
class Widget {
public:
    int num_;

    Widget() : num_(0) {};
    Widget(int num) : num_(num) {}
	Widget& operator=(const Widget& rhs) {
        num_ = rhs.num_;
		return *this;
	}
};

int main() {
    Widget w1, w2, w3(1);
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl;
    w1 = w2 = w3;
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl;
    return 0;
}
~~~
为 class 实现 operator= 时，返回自身引用是应当遵循的协议。
这个协议不仅限于 operator=，其它赋值相关运算也适用：
~~~cpp
#include <iostream>

class Widget {
public:
    int num_;

    Widget() : num_(0) {};
    Widget(int num) : num_(num) {}
	Widget& operator=(const Widget& rhs) {
        num_ = rhs.num_;
		return *this;
	}

	Widget& operator=(int num) {
        num_ = num;
		return *this;
	}
	Widget& operator+=(const Widget& rhs) {
        num_ += rhs.num_;
		return *this;
	}
};

int main() {
    int x, y, z;
    x = y = z = 15;
    std::cout << x << y << z << std::endl; // 15 15 15

    x += y += z;
    std::cout << x << y << z << std::endl;// 45 30 15

    Widget w1, w2, w3(1);
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl; // 0 0 1
    w1 = w2 = w3;
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl; // 1 1 1

    w3 = 2;
    w1 += w2 += w3;
    std::cout << w1.num_ << w2.num_ << w3.num_ << std::endl; // 4 3 2
    return 0;
}
~~~
但注意，返回自身应用只是一个协议，并非强制性的，即便代码不遵循该协议也无大碍。
但 C++ 所有的 built-in 类型和标准库中的类型都遵循该协议。因此，除非你有什么好的理由，否则最后都遵循该协议。
## Thing to Remember
>- Have assignment operators return a reference to \*this
>- 使得赋值操作符返回对 \*this 的引用

2025.01.30