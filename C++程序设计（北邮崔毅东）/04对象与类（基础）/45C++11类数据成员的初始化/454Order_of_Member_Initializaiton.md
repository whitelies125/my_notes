# Order of Member Initialization

成员的初始化次序。

## How to Initialize the Object/Class Members?

如何初始化对象/类成员？

- Default Members Initialization. 就地初始化。
- Constructor Initialization List. 构造函数初始化列表。
- Assign Values to the members in Ctor Body. 在构造函数函数体中为成员赋值。

执行顺序：

就地初始化 -> Ctor 初始化列表 -> 在 Ctor 函数体中为成员赋值。

最终哪个起作用（初始化/赋值优先级）：

在 Ctor 函数体中为成员赋值 -> Ctor 初始化列表 -> 就地初始化。

## Default Member Initializer is Ignored

就地初始化被忽略。

If a member has a default member initializer and also appears in the member initialization list in a constructor, the default member initializaer isignored.

若一个成员同时有就地初始化和构造函数列表初始化，则就地初始化语句被忽略不执行。

~~~C++
#include <iostream>
using std::cout;
using std::endl;
int x = 0;
struct S{
	int n = ++x;//default initializer
	S(){}//会使用 default initializer
	S(int arg) : n(arg){}//initializer list
};
int main(){
    cout << x << endl;//x=0
    S s1;//调用默认构造函数，使用就地初始化，x+1
    cout << x << endl;
    S s2(1);//有初始化列表的构造函数，使得就地初始化语句被忽略不执行，x不变。
    cout << x << endl;
}
//output result:
//0
//1
//1
~~~

