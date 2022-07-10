# Catch Derived Exceptions

捕获派生异常。

## catch: Derive Exception Class

catch 块中的派生异常类。

Derived exception classes 派生异常类：

~~~C++
class my_exception : public logic_error{};
~~~

catch(logic_error) will match:

catch 参数类型为基类异常类型，则可以匹配：

- cati exception objects of a base class.

  能捕获基类对象（logic_error）

- catch exception objects of the derived classes. 

  也能捕获其派生类对象（my_exception）

~~~C++
try{
    throw my_exception();
}catch(logic_error& e){//参数为基类 logic_error，可捕获本身及其派生类。
    my_exception* p = dynamic_cast<my_exception*>(&e);
    if(p!=nullptr) std:cout << p->what() << std::endl;//my_exception.what()
    else std:cout << e.what() << std::endl;//logic_error.what()
}
//不够上述只是展示一下，
//毕竟 throw 的就是 my_exception, 在 catch 中直接 e.what() 就是 my_exception.what() 就行了的
~~~

## Order of exception handlers

异常处理的次序。

The correct order when catching exceptions:

捕获异常的正确次序：

1. first appear: A catch block for a derived calss type.

   派生类的 catch 块在前。

2. seconde appear: A catch block for a base class type.

   基类的 catch 块在后。

也就是把更细分的异常放在前面优先捕获。
如果将基类放在前面，则后续派生类异常捕获就会无作用，因为牌神类派生类异常都会被前面的基类捕获住，进入基类的 catch 块。

~~~C++
try{
    //...
}catch(my_exception& e){
    //...
}catch(logic_error& e){
    //...
}
//上面这种写法显然是更好的
try{
    //...
}catch(logic_error& e){//会导致下面的 catch(my_exception& e) 根本不会起作用
    //...
}catch(my_exception& e){
    //...
}
~~~

