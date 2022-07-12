# if and switch Statement with an Initializer

带有初始化器的 if 和 switch 语句。

需要 C++17。

## if statement with an initializer

if statement **without** an initializer:

~~~C++
int foo(int arg){
	...//do something
    return arg;
}
int main(){
    auto x = foo(42);
    if (x > 40){
		...//do something with x
    }else{
		...//do something with x
    }
    // auto x = 3; 这句不可以，因为 x 变量名在这个作用域的前面已经使用过了。
}
~~~

if statement **with** an initializer:

~~~C++
int foo(int arg){
	...//do something
    return arg;
}
int main(){
    if (auto x = foo(42); x > 40){// x 的声明与定义放在了 if 中
		...//do something with x
    }else{
		...//do something with x // x 在 else 中同样有效
    }
    auto x = 3;//正确，因为前面的 x 变量名作用域范围只在 if 及其后的 else if、else 的作用域里
}
~~~

### why we need if with an initializer?

The variable, which ought to be limited in if block, leaks into the surrounding scope.

变量本应该限制在 if 块内，却侵入了周边的作用域。

The compiler can better optimize the code if it knows explicitly the scope of the variable is only in one if block. 

若编译器确切地知道变量作用域限制于 if 块内，则可更好地优化代码。

## switch statement with an initializer

类似地：

~~~C++
switch(int i = rand()%100; i){
    case 1:
        ...//do something
	...
	default:
        ...//do something  
}
~~~

