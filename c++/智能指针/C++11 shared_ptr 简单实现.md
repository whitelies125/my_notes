shared_ptr 与 unique_ptr 固然存在一些区别，但都是为了避免资源泄漏。

但是有点尬的是，工作环境中根本没用过 shared_ptr = =。

## 计数器
显而易见的，我们同 unique_ptr 一样需要一个指针 ptr_ 指向对象，此外仍需要一个计数器 count_ 用于记录所指对象的使用者数量。

自然，计数器可以想到以下实现方式：
- 全局变量
- 类的 static 成员变量
- 堆

实际上，前两者方式是不可行的。
因为计数器需满足同一对象使用同一计数器，不同对象使用不同计数器。

如果说 unique_ptr 的核心思路是维持对象始终仅有唯一的使用者兼控制者，那么对于 shared_ptr 而言核心思路是计数器是对象的唯一控制者，主要重点都在于计数器上。

同时在多线程场景，对计数器的操作还需考虑加锁。
但多线程过于麻烦且偏离主题，本文简化处理，暂不考虑。
（其实是不熟，毕竟工作也只涉及简单的多线程）

## 代码准备
同前文 [[C++11 unique_ptr 简单实现#代码准备]]
## 最简版
先写个最简版：

```cpp
template <typename T>
class SharedPtr {
public:
    SharedPtr() noexcept : ptr_(nullptr), count_(nullptr) {}
    SharedPtr(T* ptr) noexcept : ptr_(ptr), count_(new size_t(1)) {}
    ~SharedPtr() noexcept {
        if (count_ && !--*count_) { // count_ 非空 且 --count_ 为 0
            delete count_;
            delete ptr_;
        }
    }

private:
    T* ptr_;
    size_t* count_;
};
```
## 考虑隐式类型转换

```cpp
    {
        SharedPtr<Obj> ptr1 = new Obj; // 隐式类型转换，创建临时对象，随后临时对象析构
    }
```
若允许隐式类型转换，则该语句中会创建 SharePtr 临时对象，随后在表达式结束时析构，导致 Obj 随临时对象释放。
因此禁止隐式类型转换，给构造函数添加上 explicit：
```cpp
    explicit SharedPtr() noexcept : ptr_(nullptr), count_(nullptr) {}
    explicit SharedPtr(T* ptr) noexcept : ptr_(ptr), count_(new size_t(1)) {}
```

## 考虑拷贝、移动操作

不同于 unique_ptr 只允许对象存在一个使用者，shared_ptr 允许多个使用者，因此支持拷贝操作是合乎逻辑的。
当然，编译器隐式生成的拷贝构造、赋值函数仅作单纯的拷贝，因此需要 user-defined 实现计数器的增减逻辑。
同时需小心遗漏**自我赋值**的情况。

尽管是拷贝操作，但实际上源对象在逻辑上是被修改了的。不过仍然没有违背 C++ 对 constness 常量性的看法罢。

### 拷贝构造函数

```cpp
    {
        SharedPtr<Obj> ptr1 {new Obj};
        SharedPtr<Obj> ptr2 {ptr1}; // 拷贝构造
        SharedPtr<Obj> ptr3;
        auto ptr4 = ptr3; // 拷贝构造，注意判别 count_ 是否为空
    }
```

由于是构造函数，需小心从空类构造时，其计数器可能为空指针：
```cpp
    explicit SharedPtr(const SharedPtr<T>& rhs) noexcept : ptr_(rhs.ptr_), count_(rhs.count_) {
        if (count_) ++*count_;
    }
```
### 拷贝赋值函数

```cpp
    {
        SharedPtr<Obj> ptr1 {new Obj};
        SharedPtr<Obj> ptr2;
        ptr2 = ptr1; // 拷贝赋值
        SharedPtr<Obj> ptr3 {new Obj};
        ptr3 = ptr1; // 拷贝赋值，需减少 ptr3 原本的计数
    }
```

逻辑类似拷贝构造函数，但需多考虑被赋值的变量可能是有值的类，需先减少原计数器，以及自我赋值的处理：
```cpp
    SharedPtr& operator=(const SharedPtr<T>& rhs)
    {
		if (this == &rhs) return *this; // 处理自我赋值
        if(count_ && !--*count_) { // 处理可能的原计数器减少
            delete count_;
            delete ptr_;
        }
        count_ = rhs.count_; // 拷贝
        ptr_ = rhs.ptr_;
        if (count_) ++*count_;
        return *this;
    }
```

### 移动构造函数
移动构造，并不会增加使用者的数量，因此无需增加计数器。
```cpp
    explicit SharedPtr(SharedPtr<T>&& rhs) noexcept : ptr_(rhs.ptr_), count_(rhs.count_) {
        rhs.count_ = nullptr;
        rhs.ptr_ = nullptr;
    }
```

### 移动赋值函数
注意点同拷贝赋值函数：
```cpp
    SharedPtr& operator=(SharedPtr<T>&& rhs) {
        if (this == &rhs) return *this; // 处理自我赋值
        if (count_ && !--*count_) { // 处理可能的原计数器减少
            delete count_;
            delete ptr_;
        }
        count_ = rhs.count_; // 移动
        ptr_ = rhs.ptr_;
        rhs.count_ = nullptr;
        rhs.ptr_ = nullptr;
        return *this;
    }
```

## 完整代码

```cpp
template <typename T>
class SharedPtr {
public:
    explicit SharedPtr() noexcept : ptr_(nullptr), count_(nullptr) {}
    explicit SharedPtr(T* ptr) noexcept : ptr_(ptr), count_(new size_t(1)) {}

    explicit SharedPtr(const SharedPtr<T>& rhs) noexcept : ptr_(rhs.ptr_), count_(rhs.count_)
    {
        if (count_) ++*count_;
    }
    explicit SharedPtr(SharedPtr<T>&& rhs) noexcept : ptr_(rhs.ptr_), count_(rhs.count_)
    {
        rhs.count_ = nullptr;
        rhs.ptr_ = nullptr;
    }
    SharedPtr& operator=(const SharedPtr<T>& rhs)
    {
        if (count_ && !--*count_) {
            delete count_;
            delete ptr_;
        }
        count_ = rhs.count_;
        ptr_ = rhs.ptr_;
        if (count_) ++*count_;
        return *this;
    }
    SharedPtr& operator=(SharedPtr<T>&& rhs)
    {
        if (count_ && !--*count_) {
            delete count_;
            delete ptr_;
        }
        count_ = rhs.count_;
        ptr_ = rhs.ptr_;
        rhs.count_ = nullptr;
        rhs.ptr_ = nullptr;
        return *this;
    }
    ~SharedPtr() noexcept
    {
        if (count_ && !--*count_) {
            delete count_;
            delete ptr_;
        }
    }

private:
    T* ptr_;
    size_t* count_;
};
```

operator重载、自定义 deletor、也就偷懒不写了。

2025.05.24