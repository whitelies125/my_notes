# Constructor Initializer Lists

构造函数初始化列表。

注意，此前学过列表初始化 List Initialization，和这个初始化列表 Initializer List 不是一个东西。

## Constructor Initializer

构造函数初始化。

在构造函数中使用列表初始化数据域。

~~~C++
ClassName(parameterList)
    : datafield1{value1}, datafield2{value2}
{
    //Something to do
}
~~~

使用冒号 : 分隔开之后，函数体花括号 {} 之前的，叫初始化列表。

对于**基础数据类型**，二者效果相同：

~~~C++
//初始化列表方式
Circle::Circle() : radius{1}{
    
}
//通常的方式
Circle::Circle(){
    radius = 1;
}
~~~

既然二者效果相同，那么为什么要多此一举搞一个初始化列表出来呢。

初始化列表主要是为了使用于内嵌对象成员，而非基础数据类型。

## Why we need a Constructor Initializer Lists?

为什么需要构造函数初始化列表？

A data field is an object type(Object in Object/Embedded Object).

类的数据成员是一个对象类型（对象中的对象，或内嵌对象）。

The embedded object must be constrcuted before the body of ctor is executed.

内嵌对象必须在被嵌对象的构造函数体执行前就构造完成。

~~~C++
class Time{/*code omitted*/};
class Action{
private:
    Time time;
public:
    Action(int hour, int minute, int second){
        time = Time(hour,minut,second);
        //一个赋值语句，给 time 赋值，应当先已有了 time 对象，再是对其赋值。
        //但此处使用了 time 对象，但实际上 time 对象尚未构造。
        //所以要在之前就构造 time
        //显然，留下的位置只有 ...int second)这里的空间来写点东西了{...
    }
};
int main(){
    Action a(11,59,30);
}
~~~

在这小节后的选择题中，做到一道判断题：

构造函数的函数体执行之前，类中的数据成员必须完成初始化。

答案是，正确。

翻了翻 C++ Primer 中，写道：对于对象的数据成员而言，如果没有在构造函数的初始值列表中显式地初始化成员，则该成员将在构造函数体之前执行默认初始化。

这里的初始值列表，也就是我文中所说的初始化列表。

想了想，这也是理所当然的，构造函数里其实就是对数据域赋值，显然赋值之前需要先分配内存，自然，在进入构造函数之前，需要所有的数据域已经分配了内存。

所以当进入构造函数的函数体前，会进行列表初始化，数据域显式初始化和默认初始化。

顺带再说一下：

~~~C++
int a;//默认初始化 or 隐式初始化，声明时不赋值。
int b = 1;//初始化 or 显式初始化，声明时赋值。
~~~

