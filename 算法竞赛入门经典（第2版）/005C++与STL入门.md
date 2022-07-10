# C++与STL入门

[TOC]

## 排序与检索

### sort()

默认以升序排序。

~~~c
sort(a,a+n)
~~~

对区间 $[a[0],a[n])$ （左闭右开）中的元素，进行排序。

例如：

~~~c++
#include <iostream>
#include <algorithm>

using namespace std;

int main(){
    int a[5] = {5,3,2,4,1};
    cout << a[0] << a[1] << a[2] << a[3] << a[4] << "\n";
    sort(a,a+5);
    cout << a[0] << a[1] << a[2] << a[3] << a[4] << "\n";
    cout << a << "\n";
    cout << lower_bound(a,a+5,1) << "\n";
    cout << lower_bound(a,a+5,2) << "\n";
    return 0;
}
//运行结果
//53241
//12345
//0x6afec4
//0x6afec4
//0x6afec8
~~~

若要依其他顺序排序，则需传入额外的比较函数。

### lower_found()

~~~c++
lower_found(a,a+n,x)
~~~

对区间 $[a[0],a[n])$ （左闭右开）中的元素，使用二分查找，查找大于或等于 x 的第一个位置。

## 不定长数组：vector

头文件：

~~~c++
#include <vector>
~~~

声明示例：

~~~c++
vector<int> a;
vector<double> b;
~~~

常用操作：

~~~c++
a.size();//读取大小

a.resize();//改变大小
a.resize(h+1);//改变 a 的大小为 h=1，即只有 0~h 个元素

a.push_back();//向尾部添加元素
a.push_back(b);//将 b 添加到 a 的尾部

a.pop_back();//删除最后一个元素
~~~

## 集合：set

set 就是数学上的集合，每个元素最多只出现一次。且 set 中的元素是自动排好序的，默认为升序。

头文件：

~~~c++
#include <set>
~~~

声明示例：

~~~c++
set<string> dict;//名为 dict 的 string 集合
~~~

例子：

~~~c++
#include <iostream>
#include <set>

using namespace std;

int main(int argc, char* argv[])
{
	set<int> dict;
	dict.insert(3);
	dict.insert(1);
	dict.insert(2);
	dict.insert(1);
	set<int>::iterator it;
	for (it = dict.begin(); it != dict.end(); it++){
		cout << *it << " ";
	}
	return 0;
}
//运行结果：
//1 2 3
~~~

可见，本是插入 3，1，2，1，但按序输出 dict 这个 set 的结果是 1 2 3，说明了 set 的不重复和默认升序的特点。

## 映射：map

map 就是从键（key）到值（value）的映射。
每个 key 最多只出现一次。
也称为 " 关联数组 "。

例如，可以使用

~~~C++
map<string,int> month_name;
~~~

来表示 " 月份名字到月份编号 " 的映射，然后用

~~~c++
month_name["July"] = 7
~~~

这样的方式来赋值。

头文件：

~~~C++
#include <map>
~~~

声明示例：

~~~C++
map<string,int> month_name;
~~~

## 栈：stack

头文件：

~~~C++
#include <stack>
~~~

声明示例：

~~~C++
stack<int> s;//名为 s 类型为 int 的栈
~~~

常用操作：

~~~C++
s.push();//入栈
s.pop();//出栈
s.top();//取栈顶元素，但不删除（不出栈）
~~~

## 队列：queue

头文件：

~~~C++
#include <queue>
~~~

声明示例：

~~~C++
queue<int> s;//名为 s 类型为 int 的队列
~~~

常用操作：

~~~C++
s.push();//入队
s.pop();//出队
s.front();//取队首元素，但不删除（不出队）
~~~

## 测试 STL

### rand()

头文件：

~~~c++
#include <cstdlib>
~~~

生成一个 $[0,RAND\_MAX]$ （闭区间）内的均匀随机整数，其中 RAND_MAX 至少为 $32767(2^{15}-1)$ 。

### srand()

头文件：

~~~C++
#include <cstdlib>
~~~

初始化随机数种子。

如果不调用 srand() 而直接使用 rand()，相当于调用过一次 srand(1)，因此程序每次执行时，将得到同一套随机数。

使用

~~~c++
srand(time(NULL))
~~~

来方便的使得程序每次运行得到不同的随机数。

C++ 中 time() 的头文件是 ctime。

注意，由于 time() 返回的是自 1970 年 1 月 1 日 0 点以来的秒数，所以如果程序短时间多次运行（小于 1 秒），得到随机数会是一样的。

### assert 宏

~~~C++
assert(表达式)
~~~

当表达式为真时，无变化；
当表达式为假时，强行终止程序，并给出错误提示。
