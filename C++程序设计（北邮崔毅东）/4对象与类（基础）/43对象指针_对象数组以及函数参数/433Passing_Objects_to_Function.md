# Pass Objects to Function

对象作为函数参数。

## Objects as Function arguments

对象作为函数参数。

You can pass Objects by value or by reference.

对象作为函数参数，可按值传递，也可按引用传递。

~~~C++
//pass by value
void print(Circle c){...}
int main(){
	Circle myCircle(5.0);
    print(myCircle);
}

//pass by reference
void print(Circle &c){...}
int main(){
	Circle myCircle(5.0);
    print(myCircle);
}

//pass by pointer
void print(Circle* c){...}
int main(){
	Circle myCircle(5.0);
    print(&myCircle);
}
~~~

## Objects as Function Return Value

对象作为函数返回值。

~~~C++
Object func(/*arguments*/){
    //do something
    return Object(/*arguments*/);//返回一个对象
}
int main(){
	Object o = func(/*arguments*/);//这样创建对象
    func(/*arguments*/).memberFunction();//可这样调用成员函数。
}
~~~

## Object Pointer as Function Return Value

对象指针作为函数返回值。

~~~C++
Object* func(/*arguments*/){
    Object* o = new Object
    return o//返回对象指针
}
int main(){
	Object* o = func(/*arguments*/);
    //这种在函数内部 new 返回其指针的写法不太好，
    //因为在函数内部创建对象，之后却需要在函数外的其它地方 delete o，而如果函数是别人提供或提供给别人的，容易导致内存泄漏。
}

//返回对象指针，更好的写法，更适合的情况是，传递一个对象指针进来，再返回回去。尽量避免上面在函数内部创建对象，在函数外部 delete 的情况。
Object* func(Object* p,/*other arguments*/){//传递一个对象指针进来
	//do something
    return p//返回对象指针
}
int main(){
	Object* o = func(Object* p,/*other arguments*/);
    func(/*arguments*/)->memberFunction();//因为是指针，所以用 -> 调用成员函数。
}
~~~

## Object Reference as Function Return Value

对象引用作为函数返回值。

~~~C++
//错误的用法，这里函数中创建的对象是临时变量，函数结束后就销毁了，返回的 o 根本没法用。
Object& func(/*arguments*/){
    Object o{/*arguments*/};
	//do something
    return o;
}

//正确的写法，但同时主要，要避免前文所说函数内创建，函数外销毁的情况。
Object& func(Object* p,/*other arguments*/){//传递一个对象指针进来
	//do something
    return p//返回对象指针
}
int main{
    auto& o = func(/*arguments*/);
    func(/*arguments*/).memberFunction();
}
~~~

## 一些高阶问题

传值，传地址，传指针，传引用都是骗初学者的。C++ 中有意义的概念是传值和传引用。

（这句话可能只是这个老师的看法啊）

自行参考：

differences between a pointer variable and a reference variable in C++.

https://stackoverflow.com/questions/57483/what-are-the-differences-between-a-pointer-variable-and-a-reference-variable-in

difference between passing by reference vs. passing by value.

https://stackoverflow.com/questions/373419/whats-the-difference-between-passing-by-reference-vs-passing-by-value

简而言之，这个老师说了我本身就猜到的问题，大意为：
虽然 C++ 的 reference，在 C++ 的标准中没有规定编译器如何实现，但目前为止，据他了解到编译器在实现 reference 的时候都是用的指针。本质上来说，不管是 pass by value, reference, pointer，对编译器来说都是 pass by value。

实际上讲这点东西，我认为他想说的就是值与指针，也就是内存单元里的二进制和内存单元地址的关系，学过内存就能理解。

在为函数传参时，一般来说，**能用引用尽量不用指针**。
因为引用更加直观，更少出现意外的疏忽导致的错误。例如指针可能为空，而引用是不能为空的。

当然，指针可以有二重、三重之分，比引用更加灵活。
在有些情况下，例如使用 new 运算符，只能用指针。
