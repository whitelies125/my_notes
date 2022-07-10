# Overview of Exception Handling

异常处理概览。

## Necessity of Exception Handling

异常处理的必要性。

~~~C++
#include <iostream>
int main() {
	int v1, v2;
	std::cin >> v1 >> v2;
	std::cout << (v1 / v2) << std::endl;//v2 不能为 0
	return 0;
}
~~~

例如，当两数相除时，0 不能作为除数，若作为除数的变量值为 0，就会导致出现运行时错误，程序报错终止。

## Exception Handling Overview

异常处理概览。

有两种办法来处理除数为 0 的情况：

1. use **if** statement. 使用 if 语句。
2. use **exception handling**. 使用异常处理。

使用 if 的很简单，就在之前判断一下，这里就不提了。

使用异常处理的：

~~~C++
#include <iostream>
int main() {
	// Read two intergers
	int v1, v2;
	std::cin >> v1 >> v2;
	try{
        if(v2 == 0) throw v1;//如果 v2==0，则抛出异常,直接跳转到 catch 开始执行
		std::cout << (v1 / v2) << std::endl;//v2 不能为 0
    }catch(int e){
        std::cout << "Exception: an integer " << e << " cannot be divided by zero" << std::endl;
    }

	return 0;
}
~~~

## Example Snippet for try-throw-catch

try-throw-catch 的代码块示例。

~~~C++
try {
	Code to try;
	throw an exception
	//(1) with a throw statement(直接是一个 throw) 
	//(2) or from function(或者是一个函数,throw 在这个函数中);
	More code to try;
}catch (type e) {//需要 throw 出的类型与 catch() 括号中的类型匹配才会进入 catch 中执行代码
	Code to process the exception;
}
~~~

