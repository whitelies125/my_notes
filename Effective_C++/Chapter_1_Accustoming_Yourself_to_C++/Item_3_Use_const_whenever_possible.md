`const` 允许程序员明确指出一个语义上的约束：这个对象不能被修改。
编译器则会实施这个约束。

`const` 用法非常灵活：
- 可用于修饰全局或 namespace 作用域内的变量
- 可用于文件中、函数中、区块作用域（block scope） 中的 static 对象
- 可用于修饰类中的的 static 或 non-static 数据成员皆可
- 可用于指明指针自身是常量，指针指向对象是常量，或二者皆为常量。
## const 与指针
虽然 const 用法灵活，但其修饰规则很简单：
- 若 const 在星号左边，则指向的对象是常量
- 若 const 在星号右边，则指针自身是常量
- 若 const 在星号两侧，则二者都是常量。

```cpp
char greeting[] = "Hello";
char* p = greeting;             // non-const pointer, non-const data
const char* p = greeting;       // non-const pointer, const data
char* const p = greeting;       // const pointer, non-const data
const char* const p = greeting; // const pointer, const data
```

因此，下面两个变量类型其实是相同的，const 都在星号左侧：
```cpp
const int* num;
int const * num;
```
## const 与 STL
STL 是基于指针建模的，iterator 的行为就类似于 T* 指针。
但需要**注意**的是：声明一个 const iterator 实际上是类似于声明一个 T\* const 指针（指针自身为常量），而不是 const T\* 指针（所指对象为常量）
即 const iterator 是 iterator 不可变，所指对象可变；STL 提供的 const_iterator 才是 iterator 可变，所指对象不可变。
```cpp
std::vector<int> vec;
const std::vector<int>::iterator iter = vec.begin(); // iter 不可变，所指对象可变
*iter = 10;                                          // ok，所指对象可变
++iter;                                              // error，本身不可变
std::vector<int>::const_iterator cIter = vec.begin();// cIter 可变，所指对象不可变
*cLter = 10;                                         // error，所指对象不可变
++cIter                                              // ok，本身可变
```
## const 与函数

## const 与成员函数
