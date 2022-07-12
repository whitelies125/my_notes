# 322 List Initialization

C++11 之前的初始化方式：

~~~C++
    int x = 0;
    int y(1);
    char c('a');
    int a[] = {1,2,3};
    char s[] = "hello,world!";
~~~

等。

C++11 后推荐列表初始化，当然，仍然支持原本的初始化方式。

## List Initialization 列表初始化

也叫做 unified initialization 统一初始化方法，毕竟把变量和数组的初始化都统一使用花括号了。

### Direct list initialization 直接列表初始化

~~~C++
/* Variable initialization */ 
int x{}; // x is 0; 
int y{ 1 }; // y is 1; 

/* Array initialization */ 
int array1[]{ 1,2,3 }; 
char s1[ 3 ] { 'o', 'k' }; 
char s3[]{ "Hello" }; 
~~~

就是使用大括号。

### Copy list initialization 拷贝列表初始化

~~~C++
/* Variable initialization */ 
int z = { 2 }; 

/* Array initialization */ 
int array2[] = { 4,5,6 }; 
char s2[] = { 'y','e','s' }; 
char s4[] = { "World" };
char s5[] = "Aloha"; // Omit curly braces (省略花括号)
~~~

多个了等号，仍然有花括号。

---

目前，对于何时使用列表初始化仍然有争论。

这个课的崔毅东的观点是：

>尽量使用列表初始化，除非你有很好的理由不使用它。
>
>理由：列表初始化不允许 " 窄化 "，即不允许丢失数据精度的隐式类型转换
>
>如 int x = 1.1; 会进行隐式转换使得 x = 1，不会有提示信息。
>而 int x{1,1}; 则不会通过编译器的检查。

测试了一下，我用的 Code::Blocks 使用 int x{1,1} 是有提示信息，但也能运行。
看视频，崔毅东用 Visual Studio  使用 int x{1,1} 是报错，不运行的。