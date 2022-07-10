# Catch Multiple Unrelated Exceptions

捕获多种无关异常。

## Catch Various Exceptions

不同的异常的捕获。

The exception threw by a try block can be of differing types.

try 块可抛出不同类型的异常。

One catch block can catch only one type of exception.

一个 catch 块只能捕获一个异常。

所以，多写几个 catch，就能捕获不同类型的异常了：

~~~C++
try{
    /...
}catch(exception_A& e){
    //...
}catch(exception_B& e){
    //...
}
~~~

