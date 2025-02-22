条款 7：为多态基类声明 virtual 析构函数

## 引子
考虑如下示例：
~~~cpp
class TimeKeeper {
public:
    TimeKeeper ();
    ~TimeKeeper ();
};

class AtomicClock : public TimerKeeper {};
class WaterClock : public TimerKeeper {};
class WristWatch : public TimerKeeper {};

// 工厂方法
TimerKeeper * getTimerKeepr(int type){
    switch(type) {
        case 1: return new AtomicClock();
        case 2: return new WaterClock();
        case 3: return new WristWatch();
        default: return nullptr;
    }
}

TimerKeeper *ptk = getTimerKeepr(type); // 获得指向子类的父类指针
delete ptk; // 释放，行为未定义，实际往往会导致资源泄漏
~~~

注意到，基类 TimeKeeper 中的析构函数为 non-virtual。
**在 C++ 中，子类对象经由无 non-virtual 的父类指针 delete 时，其结果是未定义的**。
在实际中，这种操作通常造成的结果是子类的部分没有被释放：
假定调用 getTimerKeepr 获得的指针实则指向 AtomicClock 对象，则释放时，对象中的 AtomicClock 部分（例如声明在 AtomicClock 中的 data member）可能不会被销毁，或 AtomicClock 的 destructor 不会被执行。而基类（TimeKeeper 部分）通常会被销毁。

## 父类 destructor 声明为 virtual
解决上述问题很简单，**将父类的 destructor 声明为 virtual**。
~~~cpp
class TimeKeeper {
public:
    TimeKeeper ();
    virtual ~TimeKeeper (); // 加上 virtual 修饰
};

TimerKeeper *ptk = getTimerKeepr(type);
delete ptk; // 释放，correct
~~~

一般而言，设计上作为基类的类，往往拥有多个 virtual 函数，因为 virtual 函数的目的就是是允许子类定制化的实现（详见[[Item_34]]），因此**对于任何有 virtual 函数的类（这意味着这个类往往作为基类使用），几乎都应当有一个 virtual destructor。**

额外补充，在 C++11 primer（第五版） 15.3 虚函数 中写道：
>当我们在派生类中覆盖了某个虚函数时，可以再一次使用 virtual 关键字指出该函数的性质。
>然而这么做并非必须，因为一旦某个函数中被声明成虚函数，则在所有派生类中它都是虚函数。

当然，推荐子类还是写出来 virtual，增强代码可读性。

## 非基类的析构函数无需声明为 virtual
如果一个类没有 virtual 函数，那往往意味着它不用作基类。
当一个类并不打算作为基类，使其 destructor 为 virtual 也是不好的。

考虑以下示例：
~~~cpp
class Point {
public:
    Point(int x, int y);
    ~Point()
private:
    int x;
    int y;
}
~~~

假若 int 为 32 bit，那么一个 Point 对象有 2 个 int，就可以放入 64 bit 的寄存器中，同时也具有良好的移植性。
而倘若将 destructor 作为 virtual 函数，则由于 virtual 函数机制的实现，Point 实例对象会额外增加一个指针大小。

因为，为了实现 virtual 函数机制，知晓在运行期执行哪一个 virtual 函数，对象必须额外携带一些信息。
通常的实现中，对象需要额外携带一个 virtual table pointer（vptr，虚表指针），这个指针指向一个由函数指针构成的数组，称为 virtual table（vtbl，虚表）。
有 virtual 函数的类都有一个相应的 vtbl（其所有实例的 vprt 都指向这一个 vtbl），当对象调用某一 virtual 函数时，实则是在该实例指向的那个 vtbl 中找到对应的函数指针，然后进行调用。

简而言之，若父类有 virtual 函数 baseFuncA，baseFuncB，baseFuncC：
- 子类没有重载父类 virtual 函数，则子类的 vtbl 存储的就是父类的 virtual 函数地址 &baseFuncA，&baseFuncB，&baseFuncC。
- 子类重载了父类 virtual 函数 funcA，funcB，则子类的 vtbl 存储的是子类重载的 virtual 函数和未重载的父类的 virtual 函数 &derivedFuncA，&derivedFuncB，&baseFuncC。
实际上，虚表除了虚函数地址，还存储了其他信息，此处不多说。
总之，当类有 virtual 函数时，这个类的实例是会额外携带一个 vptr。

因此，当将上例中的 Point 类 destructor 声明为 virtual 时，Point 实例的大小会增加一个 vptr 指针大小，从而导致性能上的额外开销。同时该类也不在与 C 语言中相同结构的声明具有相同的结构，失去了良好的移植性。

所以，无缘无故地将所有 destructor 声明为 virtual，和从不声明为 virtual 都是不正确的。
事实上，人们总结为：**当且仅当类包含 virtual 函数时才将其 destructor 声明为 virtual**。

## 利用 pure virtual 析构函数构造抽象类
pure virtual function（纯虚函数）会产生 abstract class（抽象类），你无法创建 abstract class 的实例。

有时，我们想要使一个没有 pure virtual function 类为 abstract class，令用户无法创建其实例，则**可使该类的 destructor 为 pure virtual destructor 从而使其为 abstract class**。
这是因为，abstract class 往往被用作基类，而基类就应当有一个 virtual destructor，于是可以利用该 virtual destructor 为 pure，免去额外思考得将哪个的 function 改为 pure virtual function。

~~~cpp
class AWOV {
public:
 virtual ~AWOV() = 0;
};

AWOV::~AWOV() {}
~~~
如上示例，AWOV 有一个 pure virtual function，因此为抽象类；同时它有 virtual function，所以无需担心前文所述的析构问题。
但请注意：**必须为 pure virtual destructor 提供定义，否则编译报错[^1]**。
因为析构函数总是由子类向父类的顺序进行调用，编译器会在 AWOV 的子类 destructor 中调用父类 desturctor，因此必须为其提供定义。

>[^1]
>The definition of a pure virtual function may be provided (and must be provided if the pure virtual is the destructor)
>可为 pure virtual function 提供定义（且如果 pure virtual function
>为 destructor 则必为其提供定义）。

关于抽象类，参考阅读引用1 [^1]。

## 并非所有类都需要 virtual destructor
给予基类 virtual destructor 仅当适用于多态基类，这些基类本身就被设计为可通过基类接口操作子类。

但并非所有类都被设计为用于多态基类。
一些类不作为基类使用，如标准 string 和 STL 容器。
一些类可能设计为基类，但也未必用于多态。如[[Item_6_Explicitly_disallow_the_use_of_compiler_generated_functions_you_do_not_want]]中的 Uncopyable。
因此，这些类并不需要 virtual destructor。

---
>Thing to Remember
>Polymorphic base classes should declare virtual destructors. If a class has any virtual functions, it should have a virtual destructor.
>多态基类应声明虚析构函数。如果一个类具有任一虚函数，则其应具有虚析构函数。
>Classes not designed to be base classes or not designed to be used polymorphically should not declare virtual destructors.
>不设计为基类或用于多态的类，不应声明虚析构函数。

[^1]: https://en.cppreference.com/w/cpp/language/abstract_class

2025.01.14