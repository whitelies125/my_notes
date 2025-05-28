在多个方面开了一堆特例，终于是能移动、省下拷贝的开销了。
## 类型与值类别

表达式具有两种独立的特性：类型（type）和值类别（value category）
参见：https://en.cppreference.com/w/cpp/language/value_category
## static_cast

static_cast 不仅进行类型 type 转换，也进行值类别 value category 的转换。
可将 lvalue 转换为 xvalue
参见：https://en.cppreference.com/w/cpp/language/static_cast

划重点：
>If target-type is an rvalue reference type and the referenced type is reference-compatible with the type of expression, static_cast converts the value of { glvalue, class prvalue, or array prvalue(until C++17) }{ any lvalue(since C++17) } expression to xvalue referring to the same object as the expression, or to its base class subobject (depending on target-type).\[1\]
> 
>1. this type of static_cast is used to implement move semantics in std::move.

## 重载决议

函数重载决议时，对于 rvalue 匹配 func(T&&) 优先于 func(const T&)。
参见：https://en.cppreference.com/w/cpp/language/overload_resolution
## std::move
std::move 是 static_cast 的封装，将 lvalue 转换为 xvalue。
参见：https://zh.cppreference.com/w/cpp/utility/move

划重点：
>Names of rvalue reference variables are lvalues and have to be converted to xvalues to be bound to the function overloads that accept rvalue reference parameters, which is why move constructors and move assignment operators typically use std::move:
> 
>右值引用变量的名称是左值，必须转换为 xvalue 才能绑定到接受右值引用参数的函数重载，这就是移动构造函数和移动赋值运算符通常使用 std::move 的原因：

std::move 与 std::forward 的详细解读还涉及引用折叠，但本文只做简述，暂不讲述引用折叠内容
参见：https://en.cppreference.com/w/cpp/language/reference
## 小结

```cpp
#include <iostream>

using namespace std;

struct Obj {};

void func(const Obj &) {
    cout << "const Obj&" << endl;
}

void func(Obj &&) {
    cout << "Obj &&" << endl;
}

int main()
{
    Obj t;
    func(t);            // 左值，仅匹配到 func(const Obj&)
    func(std::move(t)); // 转换为右值，匹配到两个func 但最佳匹配是 func(Obj&&)
    func(static_cast<Obj&&>(t)); // 此处与上一句 std::move 等价，左值到右值的转换,从而影响到重载决议，优先匹配 func(Obj&&)
    return 0;
}

/* output:
const Obj&
Obj&&
Obj&&
*/
```

`func(std::move(t))` ：
T0：调用 std::move(t)
T1：std::move 函数实现本质是 static_cast，将 lvalue 转换为 xvalue
T2：函数重载决议，对于 xvalue 优先匹配 func(T&&) 

从而实现调用移动语义的函数。

注：此处 T2 中 T&& 的 T，指具体的类型，非指模板中的 typename T 的 T&& 或 auto&&。

## 引子

```cpp
#include <iostream>

using namespace std;

struct Data {
    Data& operator=(const Data& rhs) {
        cout << "copy operator=()" << endl;
        return *this;
    }
    Data& operator=(Data&& rhs) {
        cout << "move operator=()" << endl;
        return *this;
    }
};

struct Obj {
    Data data_;
    void Set(const Data& data) {
        data_ = data;  // copy
    }
    void Set(Data&& data) {
        data_ = data;  // copy 非 move，Data&& data 接收右值，但 data 自身是左值，
                       // 函数重载决议是，左值无法匹配 operator=(Data&& rhs)
    }
};

int main()
{
    Obj a;
    Data data;
    a.Set(data); // 调用 Set(const Data& data)
    a.Set(std::move(data)); // 调用 Set(Data&& data)
    return 0;
}
/* output:
copy operator=()
copy operator=()
*/
```

由于**右值引用变量的名称是左值**，因此在进行了一次函数调用后，尽管传入的是右值，但在被调用函数中，对应的形参是左值。
这导致移动这个语义只能够传递一次函数调用。

由于移动语义在函数调用时缺失，因此在被调用的函数中需要再次标识右值：

```cpp
    void Set(Data&& data) {
        data_ = std::move(data); // move 由于移动语义的缺失，所以需要再次标识/转换为右值
    }
/* output:
copy operator=()
move operator=()
*/
```

OK，这样也能解决问题。

但是，一方面对于 copy 和 move 操作每次都需要写 const T& 和 T&& 两个函数；另一方面对于 move 操作，如果调用层数很深，每层都需要再次标识/转换为右值。
## 万能引用

当 T&& 中的 T 是被推导出来的类型（是模板 typename T 或 auto），那这个变量或者参数就是万能引用 universal reference。

参见：https://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers
## std::forward 完美转发 

std::forward，用于万能引用场景下。
std::forward 与 std::move 相同，也是 static_cast 的封装，只是做类型转换，转换结果是实参的值类别。

使用完美转发，可以获取实参的值类别。
自然的，我们可以整合对左值、右值的处理：

```cpp
struct Obj {
    Data data_;
    template <typename T>
    void Set(T&& data) {
        // data:
        //传入的参数为左值，std::forward<T>(data) 为左值
        // 调用 data_ 的 copy operator=()
        // std::move(data): 传入的参数为左值，std::forward<T>(data) 为左值
        // 调用 data_ 的 move operator=()
        data_ = std::forward<T>(data);
    }
};

int main()
{
    Obj a;
    Data data;
    a.Set(data);
    a.Set(std::move(data));
    return 0;
}
/* output:
copy operator=()
move operator=()
*/
```

参考：
https://www.zhihu.com/question/428340896/answer/2913419725
https://zhuanlan.zhihu.com/p/55856487

2025.05.25