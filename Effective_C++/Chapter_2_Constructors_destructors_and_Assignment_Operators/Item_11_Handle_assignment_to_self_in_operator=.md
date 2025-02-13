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

## copy and swap
