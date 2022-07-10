# Name Hiding in Inheritance

继承中的名字隐藏。

在学习变量作用域时知道，全局变量可能会被函数里面的同名局部变量所隐藏。为了在函数中使用同名全局变量，需要使用域解析运算符 ::

在继承中也会出现类似的情况。

## Homonymous Function in Base Class is Hidden

基类同名函数被隐藏。

#### Names in inner scopes hide names in outer scopes.

内部作用域的名字隐藏外部作用域的（同名）名字。

- The derived class acts as an inner scope.

  派生类视作内部作用域。

- The base class as an outer scope.

  基类视作外部作用域。

#### Why do this?

为什么要这样做？

- To avoid certain potentially dangerous behavior.

  避免某些潜在的危险行为。

- Each class starts with a "clean sheet" with respect to each method name it declares.

  每个类在创建时，它的函数名都是写在一张干净的白纸上面，不会被基类函数名干扰。

~~~C++
class A {
public:
    void f(){}
};
class B : public A {
public:
    void f(int i){}//由于 B 中有名为 f 的函数，所以 A 中的同名 f 函数被隐藏
	//即便 B 继承 A，应该有基类的 f()，但是这么设计了，同名会隐藏。
};

int main(){
    B b;
    b.f();//所以，这里由于隐藏，意思是调用的 B 中的 f()，而 B 中并没有 f()，出错。
    return 0;
}
//编译报错
//no matching function for call to 'B::f()'
~~~

#### How to unhiding base class members?

如何取消隐藏基类同名成员？

- **using-declaration**: introduce base class members into derived class definitions.

  using 声明语句可以讲基类成员引入到派生类定义中。

~~~C++
class A {
public:
    void f(){}
};
class B : public A {
public:
    using A::f;//注意这里是 A::f。而不是 A::A。
    //using A::A 是前面学的继承 A 中的所有构造函数。
    void f(int i){}
};
int main(){
    B b;
    b.f();//这样，将 A 中的 f() 引入 B 中后，才允许调用。
    return 0;
}
//编译通过。
~~~

加一道习题：

对于下面的代码

~~~C++
struct A {
    void foo(int x) {};
};
struct B : A{
    void foo() {};
};
int main(){
    B b;
    // 通过对象 b，调用基类的 foo(int)函数;
}
~~~

想要完成第 9 行任务，该怎么办？

A. 在第九行写：

~~~C++
b.foo(2);
~~~

B. 在第九行写：

~~~C++
b.A::foo(2);
~~~

C. 在第五行前面插入：

~~~C++
using A::foo;
~~~

然后在第九行写：

~~~C++
b.foo(2);
~~~

D.在第九行写：

~~~C++
static_cast<A>(b).foo(2);
~~~

答案：B、C、D
