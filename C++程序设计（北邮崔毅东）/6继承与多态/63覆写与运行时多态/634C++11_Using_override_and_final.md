# C++11: Using override and final

C++11：使用 override 和 final。

## override

显式声明覆写。

C++11 引入 override 标识符，指定一个虚函数覆写另一个虚函数。

~~~C++
class A {
public:
    virtual void foo() {}
    void bar() {}
};
class B : public A {
public:
	//错误： B::foo 不覆写 A::foo //签名不匹配，B 这里的有 const,A 中的没有 const。
    //void foo() const override {}
    
    // OK: B::foo 覆写 A::foo
    void foo() override {};

    // 错误： A::bar 不是虚函数 };
    //void bar() override {}
    
    //声明不定义，但是如果定义在类外，由于 override 不能在类外使用，所以不行
    //void foo() override;
};
//错误： override 只能放在类中使用
//void B::foo() override {}
//正确
//void B::foo() {}
~~~

override 的价值在于：避免程序员在覆写时错命名或无虚函数导致隐藏 bug。

## final

显式声明禁止覆写。

C++11 引入 final 特殊标识符，指定派生类不能覆写虚函数。

~~~C++
struct Base {
	virtual void foo();
};
struct A : Base 
{
	// Base foo 被覆写，但 A::foo() 不能被覆写了
    void foo() final;
    //错误：非虚函数不能被覆写 或被 final 修饰
    //void bar() final;
};
struct B final : A // 表明 struct B 为 final，不能被继承
{
    //错误： A::foo 用 final 修饰，不能被覆写
    //void foo() override;
};
~~~

struct 可与 class 互换；差别在于 struct 的默认访问属性是 public。

习题：

final特殊标识符的作用包括

A 声明虚函数不能被覆写

B 声明常量

C 声明类不可被继承

D 声明异常结束块

答案：A,C

