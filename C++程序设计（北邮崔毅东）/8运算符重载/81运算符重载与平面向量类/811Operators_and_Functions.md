# Operators and Functions

运算符和函数。

## Special Operators Usage with Objects

与对象一起用的运算符。

string 类：使用 + 连接两个字符串。

array 与 vector 类：使用 [] 访问元素。

path 类：使用 / 连接路径元素。

实际上就是对运算符进行了重载，重新赋予了含义。

## The operator vs function

运算符与函数的异同。

- 运算符可以看做是函数。

- 不同之处

  - 语法上有区别

    运算符是 2*3
    函数是 multiply(2,3)，实际上就类似前缀表达式。

  - 不能自定义新的运算符（只能重载）

    3 ** 2，在 C/C++ 中是错误的。
    只能用 pow(3,2)。

  - 函数可 overload，override 产生任何想要的结果，但运算符作用于**内置类型**的行为不能修改。

    函数 multiply(2,3) 可以做很多事情，也可以返回任何值。
    运算符 3*2 结果必须是 6。

- 函数时编程语言的观念

  - 一切皆是函数。
  - Haskell 中可以定义新的运算符。

