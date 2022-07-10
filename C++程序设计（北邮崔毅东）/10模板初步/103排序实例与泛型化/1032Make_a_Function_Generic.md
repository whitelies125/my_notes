# Make a Function Generic

将一个函数泛型化。

## Developing generic function

设计泛型函数。

1. To start with non-generic function.

   先设计/编写一个非泛型函数。

2. To debug and test it.

   调试/测试该函数。

3. To convert it to a generic function.

   将上述非泛型函数转换为泛型函数。

## How to transform(如何泛型化)

1. What DATA does the function process?

   函数处理哪些数据？

2. What are the TYPEs of the DATA?

   数据的类型是什么？

3. Transform the TYPEs to TYPE Parameters.

   将数据类型变为类型参数。

## Generic Sort Function (泛型排序函数)

非泛型函数原型：

~~~C++
void selectionSort(double a[], const int size);
~~~

泛型函数原型：

~~~C++
template <typename T>
void selectionSort(T a[], const int size);
~~~

## Example

#### 最开始 C 风格的数组：

test.cpp

~~~C++
#include <iostream>
#include "select_sort.h"

int main(){
    int a[10] {9,8,7,6,5,4,3,2,1,0};
    for(int i : a) std::cout << i << " ";
    std::cout << std::endl;
    selection_sort(a,10);
    for(int i : a) std::cout << i << " ";
    return 0;
}
//output
//9 8 7 6 5 4 3 2 1 0 
//0 1 2 3 4 5 6 7 8 9
~~~

select_sort.h

因为是模板，所以声明和定义得写在一起，所以都在 select_sort.h 里了。

~~~C++
template <typename T>
void selection_sort(T a[], const std::size_t size){
    for(int i = 0; i < size-1; ++i){
        int index = i;
        T min = a[i];
        for(int j = i; j < size; ++j){
            if(min > a[j]){
                min = a[j];
                index = j;
            }
        }
        if(index != i){
            a[index] = a[i];
            a[i] = min;
        }
    }
}
~~~

#### std::array 的泛型

test.cpp

~~~C++
#include <iostream>
#include "select_sort.h"
#include <array>

int main(){
    std::array a {9,8,7,6,5,4,3,2,1,0};
    for(int i : a) std::cout << i << " ";
    std::cout << std::endl;
    selection_sort(a);
    for(int i : a) std::cout << i << " ";
    return 0;
}
~~~

select_sort.h

~~~C++
#include <array>
template <typename T, std::size_t N>
void selection_sort(std::array<T, N>& a){
    for(int i = 0; i < N-1; ++i){
        int index = i;
        T min = a[i];
        for(int j = i; j < N; ++j){
            if(min > a[j]){
                min = a[j];
                index = j;
            }
        }
        if(index != i){
            a[index] = a[i];
            a[i] = min;
        }
    }
}
~~~

虽然我还是不懂这里 template<typename T, std::sizie_t N> 的用法就算了。