# Exception Handling Advantages

异常处理机制的优点。

在上一节的除数为 0 的例子中，实际上用 if 来解决，比使用异常处理来解决更加简洁。

不过，只是因为这个例子太过简单和粗糙。

实际上，对于简单的情况，那么就用简单的方法，比如用 if 来处理，没有问题。但是对于复杂的情况，则使用异常处理更好。

例如，异常处理可将异常信息从被调函数带回给主调函数。

~~~C++
#include <iostream>
int quotient(int x, int y){
    if(y == 0) throw x;
    return x/y;
}
int main() {
	// Read two intergers
	int v1, v2;
	std::cin >> v1 >> v2;
	try{
        int temp = quotient(v1,v2);
		std::cout << temp << std::endl;//v2 不能为 0
    }catch(int e){
        std::cout << "Exception: an integer " << e << " cannot be divided by zero" << std::endl;
    }

	return 0;
}
~~~

而如果不使用异常处理，想要将出错信息，例如将 "是 v2 导致的异常" 这个信息，返回给主调函数，就很麻烦。