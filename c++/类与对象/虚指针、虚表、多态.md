虚指针、虚表是实现多态的一种方式。

基础概念咱就不废话了。
## 代码准备

为了方便说明，同时避免字节对齐影响叙述，这里使用指针类型的类成员变量。
因我电脑是 64 位，所以指针类型的大小为 8：

```cpp
#include <iostream>

using namespace std;

struct Base {
    void* a;
};
struct BaseFunc {
    void* a;
    void foo() {}
    void bar() {}
};
struct BaseVirtualFunc {
    void* a;
    virtual void foo() {}
    virtual void bar() {}
};

struct Derived : public Base {
    void* b;
};
struct DerivedFunc : public BaseFunc {
    void* b;
    void func() {}
    void bar() {}
};
struct DerivedVirtualFunc : public BaseVirtualFunc {
    void* b;
    virtual void foo() override {}
};

int main()
{
    cout << sizeof(Base) << endl; // 8 = sizeof(a)
    cout << sizeof(BaseFunc) << endl; // 8 = sizeof(a)
    cout << sizeof(BaseVirtualFunc) << endl; // 16 = sizeof(vptr): 8 + sizeof(a)
    cout << sizeof(Derived) << endl; // 16 = sizeof(a) + sizeof(b)
    cout << sizeof(DerivedFunc) << endl; // 16 = sizeof(a) + sizeof(b)
    cout << sizeof(DerivedVirtualFunc) << endl; // 24 = sizeof(vptr) + sizeof(a) + sizeof(b)

    return 0;
}
```

观察可知：
- 类的非虚函数不占用类大小
- 若类有虚函数，则相比无虚函数多出一个指针大小
- 类成员函数个数与类大小无关

若类有虚函数，则成员隐式多出的一个指针，称为虚指针 vptr（virtual ptr），其指向虚表 vtbl（virtual table）。

## -fdump-lang-class 查看类内存布局

通过
```shell
g++ -fdump-lang-class ./main.cpp
```

命令可以将文件中每个类的继承与虚表布局输出到同目录下 .class 文件中。

打开文件可见：
```cpp
Class Base
   size=8 align=8 // 大小为 8，字节对齐为 8
   base size=8 base align=8
Base (0x0x174ad1dbf60) 0

Class BaseFunc
   size=8 align=8
   base size=8 base align=8
BaseFunc (0x0x174ad276000) 0

Vtable for BaseVirtualFunc // BaseVirtualFunc 的虚表
BaseVirtualFunc::_ZTV15BaseVirtualFunc: 4 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI15BaseVirtualFunc) // 指向 type_info 信息内存，暂且不管
16    (int (*)(...))BaseVirtualFunc::foo // 虚函数连续放置，此为起始地址
24    (int (*)(...))BaseVirtualFunc::bar // 第二个虚函数的地址

Class BaseVirtualFunc
   size=16 align=8
   base size=16 base align=8
BaseVirtualFunc (0x0x174ad276120) 0
    vptr=((& BaseVirtualFunc::_ZTV15BaseVirtualFunc) + 16) // 隐式多出的 vptr，指向 vtbl 中连续放置虚函数地址的起始地址
```

Vtable for BaseVirtualFunc 是类 BaseVirtualFunc 的虚表，所有实例都是这一份表。
vtbl 中顺序存储着所有虚函数的地址，及其它信息。

BaseVirtualFunc 大小为 16 字节，其中存在虚指针 vptr 其指向 vtbl 中存储首虚函数地址的地址。

而对于子类：

```cpp
Class Derived
   size=16 align=8
   base size=16 base align=8
Derived (0x0x24069597138) 0
Base (0x0x24069596240) 0

Class DerivedFunc
   size=16 align=8
   base size=16 base align=8
DerivedFunc (0x0x240695971a0) 0
BaseFunc (0x0x240695962a0) 0

Vtable for DerivedVirtualFunc // 子类的虚表
DerivedVirtualFunc::_ZTV18DerivedVirtualFunc: 4 entries
0     (int (*)(...))0
8     (int (*)(...))(& _ZTI18DerivedVirtualFunc)
16    (int (*)(...))DerivedVirtualFunc::foo // 子类的 foo
24    (int (*)(...))BaseVirtualFunc::bar // 父类的 bar

Class DerivedVirtualFunc
   size=24 align=8
   base size=24 base align=8
DerivedVirtualFunc (0x0x240695972d8) 0
    vptr=((& DerivedVirtualFunc::_ZTV18DerivedVirtualFunc) + 16) // 隐式多出的 vptr，指向 vtbl 中连续放置虚函数地址的起始地址
BaseVirtualFunc (0x0x240695963c0) 0
      primary-for DerivedVirtualFunc (0x0x240695972d8)
```

Vtable for DerivedVirtualFunc 是类 DerivedVirtualFunc 的虚表，所有实例都是这一份表。
而注意到子类 vtbl 中，顺序存储的所有虚函数的地址，其中是子类 override 的 foo 与父类的 bar。

由此我们可猜测，对虚函数的调用实际上类似于（设 ptr 为对象实例指针）：
```cpp
(*(ptr->vptr)[n]))()
```

由类的虚指针 vptr 根据偏移 offset 找到调用函数。

## 代码尝试验证：
可以强行通过代码验证：
```cpp
#include <iostream>

using namespace std;

struct BaseVirtualFunc {
    void* a;
    virtual void foo() {}
    virtual void bar()
    {
        cout << "BaseVirtualFunc bar" << endl;
    }
};

struct DerivedVirtualFunc : public BaseVirtualFunc {
    void* b;
    virtual void foo() override
    {
        cout << "BaseVirtualFunc foo" << endl;
    }
};

int main()
{
    auto obj = new DerivedVirtualFunc;
    uint64_t* vptr = (uint64_t*)obj;     // 转换为 vptr，64位电脑用 u64
    auto vfuncArr = (uint64_t*)*vptr;    // vptr 指向存储虚函数表的首地址
    auto foo = (void (*)())vfuncArr[0];  // 根据偏移找到函数 foo
    foo();                               // 输出 "DerivedVirtualFunc foo"
    auto bar = (void (*)())vfuncArr[1];  // 根据偏移找到函数 bar
    bar();                               // 输出 "BaseVirtualFunc bar"

    return 0;
}
```

可见，至少在我自己电脑 mingw 环境上，如我所想。


## 多态

可知，在继承的场景下，调用子类还是父类的虚函数，其实是根据类实例的虚指针 vptr 寻找到的。

父类类型指针/引用 可以接收 父类/子类，调用时，根据编译器为类实例隐式生成的虚指针 vptr 寻找，即可找到对应的虚函数。
## 其它
什么多重继承，菱形继承，有点过于麻烦了。
目前简述到这里差不太多，其它相关内容后续有空再写。

参考：
https://www.zhihu.com/question/389546003/answer/1194780618
https://zhuanlan.zhihu.com/p/194423384
https://zhuanlan.zhihu.com/p/563418849

2025.05.24
