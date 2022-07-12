# 313 Dynamic Memory Allocation

使用 new 申请动态内存

~~~C++
new 类型名(初值);//申请一个变量的空间
new 类型名[常量表达式];//申请数组
~~~

若申请成功，返回指定类型内存地址；
若申请失败，抛出异常，或返回空指针 nullptr。

使用 delete 释放

~~~C++
delete 指针名;//释放一个变量/对象
delete[] 指针名;//释放数组空间
~~~

例：

~~~C++
char* s = new char('a');
delete s;
int* p = new int[10];
delete [] p;
~~~

