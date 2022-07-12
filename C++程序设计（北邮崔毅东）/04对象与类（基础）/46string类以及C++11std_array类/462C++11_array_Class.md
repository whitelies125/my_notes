# C++11 array Class

C++11 的数组类。

## C-Style Array v.s. C++ Style Array

C 风格数组和 C++ 风格数组。

C Style Array（也是 C++ 原生数组，C++ raw array）

- int arr[] = {1,2,3};
- arr 可能会退化为指针：void func(int a[]){ cout << sizeof(a)/sizeof(a[0]); }
- arr 可能不知道自己的大小：sizeof(arr)/sizeof(a[0])
- 两个数组之间无法直接赋值。array1 = array2; 这样做只是把 array1 指向 array2 数组的首地址了。
- 不能自动推导类型：auto a1[] = {1,2,3}; 这样 auto 是无法推导出该数组类型的，比如谁知道你这 1,2,3 是 int 还是 unsigned int 呢。

C++ Style Array

- 是一个容器类，所以有迭代器。
- 可直接赋值。如 std::array<int,3> a = {1,2,3}; 在之后也可以直接使用 a = {4,5,6} 再次赋值，而 C 风格数组只能在初始化时这样赋值。
- 知道自己的大小：size()。
- 能和另一个数组交换内容：swap()。
- 能以指定值填充自己：fill()。
- 取某个位置的元素（会做越界检查）：at()。

## Create C++ Style Array

C++ 风格数组是一个模板类，可以容纳任何类型的数据。

~~~C++
#inclue <array>
std::array<数组类型,数组大小> 数组名字;
std::array<数组类型,数组大小> 数组名字{值1, 值2,...};
~~~

限制与 C 风格数组相同。
比如，数组中只能存同类型的元素，数组大小一旦确定无法更改。

~~~C++
std::array<int, 10> x;
std::array<char, 5> c{'H','e','l','l','o'};
~~~

## C++17 Type Deduction for std::array

std::array 的类型推导。

C++17 引入了一种新特性，对类模板的参数进行推导。

~~~C++
std::array a1{1,3,5};//推导出 std::array<int, 3>
std::array a2{'a','b','c','d'};//推导出 std::array<char, 4>
~~~

## std::array 的成员函数

元素访问：

at：访问指定的元素，同时进行越界检查。

operator[]：访问指定的元素。

front：访问第一个元素。

back：访问最后一个元素。

data：返回指向内存中数组第一个元素的指针。

容量：

empty：检查容器是否为空。

size：返回容纳的元素数。

max_size：返回可容纳的最大元素数。

操作：

fill：以指定值填充容器。

swap：交换内容。

迭代器：

begin，cbegin：返回指向容器第一个元素的迭代器。

end，cend：返回指向容器尾端的迭代器。

rbegin，crbegin：返回指向容器最后元素的逆向迭代器。

rend，crend：返回指向前端的逆向迭代器。

性能上 std::array 与原生数组差不多，同时由于 std::array 自带上述操作，比较方便，所以推荐使用 std::array。