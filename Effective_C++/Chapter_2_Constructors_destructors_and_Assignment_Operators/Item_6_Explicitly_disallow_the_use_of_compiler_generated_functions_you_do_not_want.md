条款 6：若不想使用编译器自动生成的函数，则明确拒绝

如条款 5 所述，即便我们没有显式地写出一些函数，编译器也可能隐式地自动生成。

考虑以下场景，对于一个类，我并不希望其实例能够被拷贝，因此并未提供 copy constructor 和 copy assignment operator。
但由于编译器在满足条件的情况下会隐式生成这二者，也就绕开且违背了我的设计。

对于这个问题我们有以下处理方法。
## 声明为 private 且不提供定义。
之所以在 class 外部可以完成 copy 操作，是因为编译器隐式生成的函数都是 public inline 的，因此我们手动将其显式声明为 private。
首先，我们提供了用户定义的  copy constructor 和 copy assignment operator ，因此编译器不会再隐式生成 public 的默认版本。
其次，我们将二者声明为 private，从而使其无法在类的外部被调用（如调用，编译器报错）。

~~~cpp
class HomeForSale {
public:
    HomeForSale() {}
private:
    HomeForSale(const HomeForSale&) {}
    HomeForSale& operator=(const HomeForSale&) {}
};
~~~

尽管这种写法已经禁止了外部调用 copy 操作，但仍无法防住类的成员函数或 friend 函数中对 private 函数的调用。
因此上述示例中，对 copy constructor 和 copy assignment operator **仅显式提供了 private 的声明，而不提供具体的定义**。如此，即便用户在类成员函数中调用了 private 的二者，也会在链接阶段因找不到定义而报错（如调用，链接阶段报错）。
## 子类继承基类，且基类声明为 private 并不提供定义

更近一步地，我们可以通过一种写法将上文的链接期报错，提前到编译期报错。

在父类中将 copy constructor 和 copy assignment operator 声明为 private，随后让想禁止 copy 操作的子类继承该父类即可。
~~~cpp
class Uncopyable {
protedcted: // 允许子类构造和析构
    Uncopyable() {}
    ~Uncopyable() {}
private: // 阻止子类 copy 操作
    Uncopyable(const Uncopyable&);
    Uncopyable& operator=(const Uncopyable&);
};

class HomeForSale : private Uncopyable {
    // 子类无需额外操作
}
~~~

因为编译器隐式生成的 copy constructor 和 copy assignment operator，会调用父类的 copy constructor 和 copy assignment operator（用户显式定义的则不会），但这种情况下，子类没有权限调用，因此编译器不隐式生成相应函数，以此阻止了子类的 copy 操作。

>Thing to Remember
>- To disallow functionality automatically provided by compilers, declare the corresponding member function private and give no implementations. Using a base class like Uncopyable is one way to do this.
>  为了禁止编译器自动提供的功能，将相应的成员函数声明为 private 且不提供实现。使用像 Uncopyable 的基类也是一种做法。

## 声明为 delete
C++11 中已经引入了将函数声明为 delete 的特性。

>Any use of a deleted function is ill-formed (the program will not compile)[^1].
>任何对 deleted function 的使用都是不合语法的（程序不会编译）

因此，C++11 及之后，对于禁止使用的函数，将其声明为 delete 即可：
~~~cpp
class HomeForSale {
public:
    HomeForSale() {}
// private:
    HomeForSale(const HomeForSale&) = delete;
    HomeForSale& operator=(const HomeForSale&) = delete;
};
~~~
[^1]: https://en.cppreference.com/w/cpp/language/function

2025.01.08