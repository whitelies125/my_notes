条款 11：在 operator= 中处理自我赋值

# 引子
当对象赋值给自己时，assignment to self 就会发生：
~~~cpp
Widget w;
w = w; // assignment to self
~~~
尽管上述代码很蠢，但是合法的。
因此，需要假定使用者可能会写出 assignment to self 的代码并进行处理。
尽管他可能是无意识地写出这种代码，通过别名操作对象时会使得 assignment to self 并不是如上例那般显而易见的（因为有多种途径访问到同一对象），如通过数组下标或指针进行赋值操作时：
~~~cpp
a[i] = a[j] // 可能导致 assignment to self
*px = *py // 可能导致 assignment to self
~~~
考虑继承特性的场景，甚至两个类型不同的变量也可能导致 assignment to self：
~~~cpp
class Base {};
class Derived : public : Base {};
void doSomething(const Base& rb, Derived* pb); // 此处 rb 和 *pb 可能是同一个对象
~~~
使用对象管理资源时，assignment to self 有可能导致资源在预期外被提前释放。
~~~cpp
class Bitmap {};
class Widget {
public:
    Bitmap* pb; // 指向一个 heap 对象
	Widget& operator=(const Widget& rhs) {
        delete pb; // 先释放当前 heap 对象
        pb = new Bitmap(*rhs.pb); // 使用 *rhs.pb 的拷贝。此处若为 assignment to self 时，rhs.pb 指向的 Bitmap 对象在上一行代码已释放
        return *this; // 见条款10
	}
};

int main() {
    Widget w1; w1.pb = new Bitmap;
    Widget w2; w2.pb = new Bitmap;
    w1 = w2; // ok
    w1 = w1; // w1 中的 Bitmap 在预期外被提前释放
    return 0;
}
~~~
因此，我们需要在 operator= 中处理 assignment to self 场景，避免上述问题。
# 方法

## identity test
在 operator= 的开头加入 identity test，若为自身赋值，则不做任何处理。
~~~cpp
class Widget {
public:
    Bitmap* pb;
	Widget& operator=(const Widget& rhs) {
    	if (this == &rhs) return *this; // identity test
        delete pb; 
        pb = new Bitmap(*rhs.pb);
        return *this;
	}
};
~~~
## operator= 异常安全性
此外，我们还需要考虑异常安全性。
in fact，使得 operator= 为异常安全的，往往也使得其自我赋值也为安全的。

加入了 identity test 的版本，固然是 assignment to self 安全的，但并非异常安全的。
在上例中，先 delete 释放资源，再 new 申请资源，这样的先后顺序会导致，若 new 申请资源导致异常（内存不足或 Bitmap 的 copy constructor 抛出异常等原因），则原资源也已被 delete 释放。此时 pb 仍指向一块已释放的内存。

因此调换逻辑顺序，即可做到：
~~~cpp
class Widget {
public:
    Bitmap* pb;
	Widget& operator=(const Widget& rhs) {
        Bitmap* pOrig = pb; // 保存原 pb 资源;
        pb = new Bitmap(*rhs.pb); // new 申请资源；若此处失败，仍是 assignment to self 安全的
        delete pOrig; // new 成功，再删除原 pb 资源
        return *this;
	}
};
~~~

当然，也可以同 identity test 同时使用，在开头加入自我赋值的判断。
这样 identity test 可以在自我赋值场景下直接 return，使得代码运行速度更快；但同时在未命中（非自我赋值）场景下会代码运行速度会下降，以及多加入了一行代码，也会使得代码略微膨胀。
因此需要考虑自我赋值的发生频率有多高了，是否需要再加入 identity test。
## copy and swap
先拷贝，再交换。
copy and swap 还与[[Item 29]]异常安全有关。思路为：修改对象 A 时，优先创建出一个同类型的临时变量 B，在 B 上完成修改，这样，若 B 顺利完成修改，则将其与对象 A 通过不抛出异常的操作进行 swap；若 B 发生异常，则对象 A 不会受到影响。

~~~cpp
class Widget {

public:
    Bitmap* pb;
    void swap(Widget &rhs); // 交换 *this 与入参 rhs 的数据
	Widget& operator=(const Widget& rhs) {
        Widget temp(rhs); // 创建临时变量，copy rhs；如果发生异常，则 *this 数据未改变，不受影响。
        swap(temp); // 调用 swap 函数，交换 temp 与 *this 数据，
        return *this;
	}
};
~~~

利用 C++ 函数值传递的拷贝行为，代码可以简化为：
~~~cpp
class Widget {

public:
    Bitmap* pb;
    void swap(Widget &rhs); // 交换 *this 与入参 rhs 的数据
	Widget& operator=(Widget rhs) { // 这里去掉了引用传递，使用值传递
        swap(rhs); // 调用 swap 函数，交换 temp 与 *this 数据，
        return *this;
	}
};
~~~
这样将 copy 动作由函数内部前移到函数参数构造阶段，有时可令编译器生成更高效的代码，同时在代码可读性上会略有损失。

# Things to Remember

- Make sure operator= is well-behaved when an object is assigned to itself. Techniques include comparing addresses of source and target objects, careful statement ordering, and copy-and-swap.
    确保自我赋值时 operator= 是行为良好的。技巧包括有：比较源对象与目标对象的地址、精心设计的语句顺序、copy-and-swap。
- Make sure that any function operating on more than one object behaves correctly if two or more of the objects are the same.
    确保任何函数操作多个对象，若其中两个或多个对象是同一对象时，其行为是正确的。

2025.2.16