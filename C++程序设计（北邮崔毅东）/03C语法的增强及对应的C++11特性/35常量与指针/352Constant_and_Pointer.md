# Constant and Pointer

const + pointer

指针与被指数据可能的关系（常量/变量）：

| 指针     | 被指数据 |
| -------- | -------- |
| variable | variable |
| variable | constant |
| constant | variable |
| constant | constant |

**指针变量**：指针为变量，即指针中的地址为变量，可以改变。

**指针常量**：指针为常量，即指针中的地址为常量，不可以改变。

**常量指针**：指针所指数据为常量，不可以通过指针来改变所指数据。

**常量指针常量**：指针为常量，指针所指数据为常量。

例子：

~~~C++
int x = 5;
int* p = &x;

int x = 5;
int* const p = &x;
//这里的 const 修饰 p，意指 p 是 constant 不能修改，
//即 p 中指向 x 的地址不能被改变，但 p 指向的 x 的内容可以被修改。

const int x = 5;
const int* p = &x;
//此处 const 表明我们认为 p 中地址指向的 x 为常量，不能被改变。
//但实际上 x 为 const int 或 int 都行，仅仅只是我们写 const int* p = &x; 时，认为 p 中地址指向的 x 为常量，即 " 不能通过 p 来修改 p 指向的 x 的值 " 罢。这与 x 是否为 const，其实是没有关系的。
//意指 p 中地址指向的 x 不能被改变，但 p 中的地址可以被修改（即可指向其他变量）。

//注意，虽然在上一节 351const关键字 中说到：const int x = 5 与 int const x = 5 没有区别。
//但涉及到指针 const int* p = &x 与 int* const p = &x 是有区别的。

const int x = 5;
const int* const p = &x;
//const p 表明 p 为常量，不能修改
//int* 表明 p 为 int 指针
//最前面的 const 表明我们认为该指针 p 指向的地址的内容为 const，
//实际上，最前面的 const 仅仅是我们认为 " 不能通过 p 来修改 p 指向的 x 的值 " 的意思。
//至于 x 是 const int 还是 int 都行。只是我们不能通过指针 p 来改变 x。
~~~
