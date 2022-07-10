# More on Operator Overloading

运算符重载的更多说明。

## 能重载运算符操作基础数据类型吗

不能。

重载的运算符必须和用户定义的 class 类型一起使用。

重载的运算符的参数至少应有一个是类对象（或对象的引用）

- 参数不能全部是 C++ 的标准类型。
- 以防止用户修改用于标准类型数据的运算符性质。

## 以友元函数实现部分运算符重载

大部分运算符重载也都可以以友元函数的方式来实现，方便在于可以任意指定运算符左右参数的次序。

但赋值运算符 =，下标运算符 []，函数调用运算符 ()，箭头运算符 ->，都必须只能是类的成员函数，而不能使用友元函数实现。

~~~C++
class Vec2D {
public:
    friend Vec2D operator+(Vec2D &firstVec, Vec2D &secondVec);
	// friend double& operator[](Vec2D &v, const int &index);
	// operator[] 不能重载为友元函数，只能重载为成员函数
    friend Vec2D operator++(Vec2D &v);
    friend Vec2D operator++(Vec2D &v, int dummy);
}
~~~
