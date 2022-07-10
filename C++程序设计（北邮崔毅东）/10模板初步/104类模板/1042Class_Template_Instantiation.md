# Class Template Instantiation

类模板实例化。

## 显式实例化

~~~C++
template class Stack<int>; // 将类模板实例化为一个处理 int 类型的 Stack类
//实际上编译器会按照自己的规则，生成一个类，比如这里可能就叫作 IntStack 类。为了避免重名，生成的名字可能更复杂
~~~

## 隐式实例化

~~~C++
Stack<char> charStack;
//会先实例化一个 CharStack 类：(名字由编译器按规则生成，为了避免重名，名字可能更复杂)
//class CharStack { … char elements[100]; … };
//然后使用该类创建名为 charStack 的对象：
//CharStack charStack; 

Stack<int> intStack;
//实例化一个 IntStack 类：(名字由编译器按规则生成，为了避免重名，名字可能更复杂)
//class IntStack { … int elements[100]; … };
//然后使用该类创建名为 intStack 的对象：
//IntStack intStack;

vector intVector{1, 2, 3};
//C++17，模板类型参数可根据初始化语句自动推导
//自动推导，实例化为 vector<int>
~~~

