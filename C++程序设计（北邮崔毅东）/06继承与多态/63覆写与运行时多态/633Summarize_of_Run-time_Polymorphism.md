# Summarize of Run-time Polymorphism

运行时多态的总结。

## Summary: static binding v.s. dynamic binding

基类与派生类中有同名函数。

1. 通过**派生类对象**访问**同名函数**——>静态联编。
2. 通过**基类对象指针**访问**同名函数**——>静态联编。
3. 通过**基类对象的指针或引用**访问**同名虚函数**——>动态联编。

## Summary: 静态联编的简单示例

#### 通过**派生类对象**访问**同名函数**

对象是什么类型，就调用什么类型。

~~~C++
class P{//父类
public:
    f(){}
};
class C : public P{//子类
public:
    f(){}
};
int main(){
    P p;
    C c;
    p.f();//call P::f()
    c.f();//call C::f()
}
~~~

#### 通过基类指针访问同名函数

指针是什么类型，就调用什么类型。

~~~C++
class P{//父类
public:
    f(){}
};
class C : public P{//子类
public:
    f(){}
};
int main(){
    P p;
    C c;
    P* Ptr;
    Ptr = &p;
    p->f();//call P::f()
    Ptr = &c;
    p->f();//call P::f()
}
~~~

## Summary: 动态联编的简单示例

#### 通过基类对象的指针访问同名虚函数

虚函数，不看指针看真对象。

~~~C++
class P{//父类
public:
    virtual f(){}
};
class C : public P{//子类
public:
    f() override {}
};
int main(){
    P p;
    C c;
    P* Ptr;
    Ptr = &p;
    p->f();//call P::f()
    Ptr = &c;
    p->f();//call C::f()
}
~~~

#### 通过基类对象的引用访问同名虚函数

虚函数，不看引用看真对象。

~~~C++
class P{//父类
public:
    virtual f(){}
};
class C : public P{//子类
public:
    f() override {}
};
int main(){
    P p;
    C c;
    P& p1 = P;
    p1->f();//call P::f()
    P& p2 = P;
    p2->f();//call C::f()
}
~~~

