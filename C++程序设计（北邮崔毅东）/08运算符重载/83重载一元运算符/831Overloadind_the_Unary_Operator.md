# Overloading the Unary Operator

重载一元运算符。

一元：一个操作数。

Let  @ be a unary operator

@ 代表单目运算符：--，++，-(符号)

## prepositive unary operators(前置单目运算符)：

-(负号)，*(dereference，解引用)。

当编译器遇到 @obj 时：

- 若 operator@ 是在 obj 的类中的成员，则调用 obj.operator@()
- 若 operator@ 是在 obj 的类的 friend 函数，则调用 operator@(obj)



