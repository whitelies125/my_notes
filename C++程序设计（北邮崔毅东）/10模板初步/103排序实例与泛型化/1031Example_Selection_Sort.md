# Example: Selection Sort

例子：选择排序。

#### 以 C 风格数组的选择排序。

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

~~~c++
#include <iostream>
void selection_sort(int a[], const std::size_t size);
~~~

select_sort.cpp

~~~C++
#include "select_sort.h"
void selection_sort(int a[], const std::size_t size){
    for(int i = 0; i < size-1; ++i){
        int index = i;
        int min = a[i];
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

#### 使用 std::array 的选择排序：

问题在于

~~~c++
void select_sort(std::array<int,10> a){//这样只能接受大小为 10 的int array 类型。
	//...
}
~~~

所以使用模板：

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
template <std::size_t N>
void selection_sort(std::array<int, N>& a);
~~~

select_sort.cpp

~~~C++
#include "select_sort.h"
template <std::size_t N>
void selection_sort(std::array<int, N>& a){
    for(int i = 0; i < N-1; ++i){
        int index = i;
        int min = a[i];
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

实际上这样编译时会有问题。

对于模板，需要将模板声明和模板定义写在一起。这里也就是都要放在 select_sort.h 中。

所以修改为：

test.cpp 不变，select_sort.cpp 不用写了。

select_sort.h

~~~C++
#include <array>
template <std::size_t N>
void selection_sort(std::array<int, N>& a){
    for(int i = 0; i < N-1; ++i){
        int index = i;
        int min = a[i];
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

顺带，在函数模板中，视频里老师用的是

~~~C++
template <int N>//这里是 int
void selection_sort(std::array<int, N>& a)
~~~

但我用 int 的话，编辑器会报错，编译也会有提示信息，但能够运行且结果也正确。

使用 std::size_t 就整个过程都正确。

至于为什么以及这个用法，我暂时没懂。
