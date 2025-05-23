
简单写写 unique_ptr，给自己未来可能的面试复习一下。

C++11 引入了智能指针，用于方便的实现 RAII。
这里来手动实现一个简易的 unique_ptr，主要在于我个人的思路理解。
## 前置

首先创建一个类 Obj，用于观察类的构造与析构，以及一个 block 使得智能指针析构。
```cpp
#include <iostream>
using namespace std;

struct Obj {
    Obj() { cout << "Obj()" << endl; }
    ~Obj() { cout << "~Obj()" << endl; }
};

int main()
{
    cout << "start" << endl;
    {

    }
    cout << "end" << endl;
    return 0;
}
```

## 最简版
最简版的实现，构造 UniquePtr 时，接收一个指针；析构时，释放该指针并置空即可。
使用简单的模板即可完成：

```cpp
template <typename T>
class UniquePtr {
public:
    UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr)
    {
        cout << "UniquePtr() : " << static_cast<void*>(ptr_) << endl;
    };

    ~UniquePtr() noexcept
    {
        cout << "~UniquePtr() : " << static_cast<void*>(ptr_) << endl;
        delete ptr_;
        ptr_ = nullptr;
    };

private:
    T* ptr_;
};
```

已可以进行简单的使用：
```cpp
    {
        UniquePtr<Obj> ptr {new Obj};
    }
/* output:
Obj()
UniquePtr() : 0x193d4989fe0
~UniquePtr() : 0x193d4989fe0
~Obj()
*/
```
## 考虑隐式转换

尝试创建后赋值的语句：
```cpp
    {
        UniquePtr<Obj> ptr;
        ptr = new Obj;
    }
/* output:
UniquePtr() : 0
Obj()
UniquePtr() : 0x21c496fe370
~UniquePtr() : 0x21c496fe370
~Obj()
~UniquePtr() : 0x21c496fe370
~Obj()
*/
```

注意到，打印显示发生了 2 次构造、析构，导致了重复释放。

这是由于 `ptr = new Obj` 语句：
T0：执行 new Obj，获得 Obj \* 指针；
T1：调用 operator=()，此时 operator=() 为编译器隐式生成，入参类型为 const UniquePtr\<T\>&
T2：因为存在接收 Obj \* 入参的 UniquePtr 构造函数，因此发生隐式类型转换，创建了 1 个匿名临时对象；
T3：随后使用该临时对象，完成 operator=() 调用；
T4：临时对象在表达式完成后析构。

因此，将构造函数声明为 explicit，即可避免隐式类型转换导致的重复释放问题：

```cpp
    explicit UniquePtr(T* ptr = nullptr) noexcept: ptr_(ptr)
    {
        cout << "UniquePtr() : " << static_cast<void*>(ptr_) << endl;
    };
```

actually，这里已经可以感受到 unique_ptr 的核心设计思路就是需保持资源始终仅有 1 个控制者。

## 考虑拷贝操作

涉及资源的拷贝操作总是比较麻烦的设计。提到拷贝，那么就涉及：
- 拷贝构造函数
- 拷贝复制函数
- 移动构造函数
- 移动赋值函数
虽然多且麻烦，但只要遵守对象始终仅由 1 个 UniquePtr 控制的设计原则即可。

### 拷贝构造函数 T(const T&)

```cpp
    {
        UniquePtr<Obj> ptr1 {new Obj};
        UniquePtr<Obj> ptr2 {ptr1}; // 拷贝构造
    }
/* output:
Obj()
UniquePtr() : 0x27da880e4d0
~UniquePtr() : 0x27da880e4d0
~Obj()
~UniquePtr() : 0x27da880e4cf
~Obj()
*/
```
由于没有 user-defined 拷贝构造函数，因此编译器隐式生成了拷贝构造函数，且其行为是单纯的拷贝。
所以这里看到了对 Obj 的 2 次析构。

而即便我们自定义拷贝构造函数，由于入参为 const 修饰，因此无法对 rsh 置空，故而无法保持对象仅有 1 个 UniquePtr 控制的原则：

```cpp
    explicit UniquePtr(const UniquePtr<T>& rhs) noexcept: ptr_(rhs.ptr_)
    {
        cout << "copyt UniquePtr() : " << static_cast<void*>(ptr_) << endl;
        rhs.ptr_ = nullptr; // const qualifier，无法修改
    };
```

因此，应禁止使用拷贝构造函数，同时为了防止误用编译器隐式生成的版本，将其 delete：
```cpp
    UniquePtr(const UniquePtr<T>& rhs) = delete;
```

### 拷贝赋值函数 T& operator=(const T&)
即便 delete 了拷贝构造函数，也可能通过编译器隐式生成的拷贝赋值函数导致多个控制者：

```cpp
    {
        UniquePtr<Obj> ptr1 {new Obj};
        UniquePtr<Obj> ptr2;
        ptr2 = ptr1; // 拷贝赋值
    }
```
逻辑与拷贝构造函数相同，对于 T& operator=(const T& rhs) 形式的拷贝赋值函数，其入参仍为 const，出于相同的理由，将其 delete：
```cpp
    UniquePtr& operator=(const UniquePtr<T>& rhs) = delete;
```

### 移动构造函数 T(T&&)

```
    {
        UniquePtr<Obj> ptr1 {new Obj};
        UniquePtr<Obj> ptr2 {std::move(ptr1)}; // 移动构造
    }
```
移动构造的语义本就是资源的转移，完美符合，因此允许使用移动构造并无问题：

```cpp
    explicit UniquePtr(UniquePtr<T>&& rhs) noexcept : ptr_(rhs.ptr_)
    {
        cout << "move UniquePtr() : " << static_cast<void*>(rhs.ptr_) << endl;
        rhs.ptr_ = nullptr;
    }
```
当前由于只涉及 ptr_ 指针的拷贝，且并无其他函数调用，因此这里直接拷贝就好。
如果是其他场景，可能需要使用 std::move 或 std::forward。

### 移动赋值函数 T& operator=(const T&&)

```cpp
    {
        UniquePtr<Obj> ptr1 {new Obj};
        UniquePtr<Obj> ptr2 = std::move(ptr1); // 移动赋值
    }
```
分析逻辑同移动构造函数：

```cpp
    UniquePtr& operator=(UniquePtr<T>&& rhs)
    {
        cout << "move operator=() : " << static_cast<void*>(rhs.ptr_) << endl;
        ptr_ = rhs.ptr_;
        rhs.ptr_ = nullptr;
        return *this;
    };
```
### 其他
实际上，拷贝构造函数仍有如 T(T& rhs) 的非 const 入参形式，拷贝赋值函数也有如 T& operator=(T& rhs) 形式。
既然如此，也可以做到如移动构造、移动拷贝的作用。

私以为考虑到语义性，拷贝通常意味着仅拷贝而对源对象无修改，移动则意味着资源的转移。
因此实现非 const 的拷贝构造、赋值函数，不太合理。
而代码里若存在不符直觉的函数、语句，就更容易导致 bug 的产生。
## 数组的模板特化

当前实现中，对 ptr_ 的释放方式为 delete，也就意味着 UniquePtr 尚不支持数组类型，因此需为其单独写一个模板特化。
除了因为类型改为 T\[\] 引起的关联修改外，并无太多额外内容，这里只简单给出代码，略过：
```cpp
template <typename T>
class UniquePtr<T[]> {
public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr)
    {
        cout << "T[] UniquePtr() : " << static_cast<void*>(ptr_) << endl;
    };

    UniquePtr(const UniquePtr<T[]>& rhs) = delete;
    UniquePtr& operator=(const UniquePtr<T[]>& rhs) = delete;

    explicit UniquePtr(UniquePtr<T[]>&& rhs) noexcept : ptr_(rhs.ptr_)
    {
        cout << "T[] move UniquePtr() : " << static_cast<void*>(rhs.ptr_) << endl;
        rhs.ptr_ = nullptr;
    }
    UniquePtr& operator=(UniquePtr<T[]>&& rhs)
    {
        cout << "T[] move operator=() : " << static_cast<void*>(rhs.ptr_) << endl;
        ptr_ = rhs.ptr_;
        rhs.ptr_ = nullptr;
        return *this;
    };

    ~UniquePtr() noexcept
    {
        cout << "~T[] UniquePtr() : " << static_cast<void*>(ptr_) << endl;
        delete[] ptr_;
        ptr_ = nullptr;
    };

private:
    T* ptr_;
};
```

## 自定义 deletor
考虑到析构释放对象时可能存在其他连带操作，因此提供自定义 delete 操作。
实现上，就是支持再接受一个 callable object：

```cpp
struct Deletor {
    template <typename T>
    void operator()(T* ptr) {
        delete ptr;
    }
};

template <typename T, typename D = Deletor>
class UniquePtr {
public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {};

    UniquePtr(const UniquePtr<T>& rhs) = delete;
    UniquePtr& operator=(const UniquePtr<T>& rhs) = delete;

    explicit UniquePtr(UniquePtr<T>&& rhs) noexcept : ptr_(rhs.ptr_) {
        rhs.ptr_ = nullptr;
    }
    UniquePtr& operator=(UniquePtr<T>&& rhs) {
        ptr_ = rhs.ptr_;
        rhs.ptr_ = nullptr;
        return *this;
    };

    ~UniquePtr() noexcept{
        deletor(ptr_);
        ptr_ = nullptr;
    };

private:
    T* ptr_;
    D deletor;
};
```
## 后话

至于其他 operator 运算符的实现，就偷懒不写啦。
全是个人的简单理解，如理解有误还请指出 (つд⊂)。

2025.05.22