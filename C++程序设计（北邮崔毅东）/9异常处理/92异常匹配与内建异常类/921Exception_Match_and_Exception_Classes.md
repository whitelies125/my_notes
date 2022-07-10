# Exception Match and Exception Classes

异常匹配与异常类。

## catch: Match with Exception Type

catch: 按照异常类型匹配。

~~~C++
catch(ExceptionType& parameter){
    //...
}
~~~

若 try{} 中所抛异常类型与 catch() 的参数类型 ExceptionType 匹配，则进入 catch 块。

若对异常对象的内容不感兴趣，则可省略 catch 参数，只保留类型。

例如：

~~~C++
void f1(){ throw 100; }

try{
    f1();
}catch(int& e){//类型+参数
    std::cout << "Error : " << e << std::endl;
}

try{
    f1();
}catch(int){//仅类型
    std::cout << "Error" << std::endl;
}
~~~

有些代码失败时会抛出已定义好的异常类型：

~~~C++
void f2(){
	for(int i = 1; i < 100; i++)
        new int[1000000000];
}

try{
    f1();
}catch(bad_alloc& e){//类型+参数
    std::cout << "Error : " << e.what() << std::endl;
}

try{
    f2();
}catch(bad_alloc){//仅类型
    std::cout << "new Error" << std::endl;
}
~~~

可见，不就可以使用 throw 出 int 类型，也可以 throw 出类的对象。

## Why Exception class

为何要使用异常类。

显然，throw 出 int 类型，我们仅仅能获得一个值的信息，而 throw 出类的对象，可以获得更多的信息，比如上述代码中的 e.what() 就是一段描述这个异常信息的字符串。