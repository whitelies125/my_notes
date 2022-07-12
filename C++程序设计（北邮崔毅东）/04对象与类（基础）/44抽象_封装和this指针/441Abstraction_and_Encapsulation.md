# Abstraction and Encapsulation

抽象与封装

## Data Field Encapsulation

数据域封装。

~~~C++
class Circle{
public:
    double radius;
    //...
};
int main(){
    //...
    circle1.radius = 5;//类中数据在类外被直接修改。
}
~~~

数据域采用 public 形式有 2 个问题。

- First, data may be tampered.

  数据会被类外的方法篡改。

- Second, it makes the class difficult to maintain and vulnerable to bugs.

  使得类难以维护，易出现 bug。

更好的方式：

~~~C++
class Circle{
private:
    double radius;//放在 private 中，使得 radius 只能被类中的方法访问。
public:
    Circle();
    //...
};
~~~

讲 radius 放在 private 中，使得 radius 只能被类中的方法访问，这就将 private 封装了起来。

## Accessor and Mutator

访问器与更改器。

To read/write private data, we need get/set function.

为了读写私有数据，我们需要 get/set 函数。

- get function is refered to as a getter(or accessor).

  get 函数被称为 getter（获取器，or 访问器）（称为 getter 更为常见）。

- set function is refered to as a setter(or mutator).

  set 函数被称为 setter（设置器，or 更改器）（称为 setter 更为常见）。

Signature of get function(General form)

get 函数的一般原型。

- returnType getPropertyName()

Signature of get function(Bool type)

布尔型数据的 get 函数的原型。

- bool isPropertyName()

Signature of set function

set 函数的原型。

- void setPropertyName(dataType propertyValue)

显然，访问器与修改器应该放在 public 内。

## Class Abstraction and Encapsulation

类抽象与封装。

### Class Abstraction 类抽象

The process of removing physical, spatial, or temporal details or attributes in the study objects or systems in order to more closely attend to other details of interest. 

在研究对象或系统时，为了更加专注于有趣的细节，去除对象或系统的物理或时空细节/属性的过程叫作抽象。

### Class encapsulation 类封装

A language mechanism for restricting direct access to some of the object's components.

一种限制直接访问对象组成部分的语言机制。

A language constrct that facilitates the bundling of data with the methods(or other functions) operating on that data.

一种实现数据和函数绑定的语言构造块。