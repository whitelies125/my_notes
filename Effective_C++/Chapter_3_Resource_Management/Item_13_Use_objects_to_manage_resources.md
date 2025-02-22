资源就是指，一旦你使用了它，你就需要将它归还给系统。
如果没有这样做，就会发生糟糕的事情。

Item 13：以对象管理资源

# 引子
假设在 func 函数中通过工厂模式[[Item_7_Declare_destructor_virtual_in_polymorphic_base_classes]]由 createInvestment 函数返回基类指针，随后在 func 函数末尾释放它：

~~~cpp
Investment * createInvestment(); // fa
void f() {
    Investment *pInv = createInvestment();
    //...
    delete pInv;
}
~~~
这种写法看似没有问题，实则：
在中间代码，可能存在有 return 语句并被指向，导致函数提前退出。
又或者在循环中使用 createInvestment 及 delete 时，中间存在 break 或 goto 语句导致 delete 未执行。
又如，中间代码可能抛出异常。

# 使用对象管理资源

通过将资源放在对象中，我们可以依赖 C++ 自动调用 destructor 来确保资源被释放。

使用对象来管理资源的两个关键点在于：
- 获得请求的资源后，立即将其放入管理它对象中：
    In fact，这种理念也被称为 **Resource Acquisition Is Initialization（RAII）**，因为请求资源与初始化资源管理对象往往在相同的语句中。
    有时也会是先请求资源，随后指派给资源管理对象。
    但无论哪种，二者都是在获得资源的同时，立即转给了资源管理对象。
- 管理资源的对象使用 destructor 来确保资源被释放：
    因为 destrctor 会在对象析构时自动调用，因此无论控制流如何离开区块，资源总是会被正确地释放。
    尽管当释放资源时抛出异常，事情可能变得棘手，但这是[[Item_8_Prevent_exceptions_from_leaving_destructors]]解决的问题，这里我们无需再多考虑。

## 使用 std::auto_ptr （c++11 起已废弃）
大多数场景为，资源从 heap 申请，但仅在一个小区块或函数中使用，当控制流在离开该区块或函数时应释放这些资源。

标准库的 auto_ptr 就是为这种场景定制的。
auto_ptr 是一个 pointer-like 对象，是一个智能指针，其 destructor 将会自动调用它所指的对象。

~~~cpp
void func() {
    std::auto_ptr<Investment> pInv(createInvestment());
    // ...
    // 函数退出时，会通过 auto_ptr 的 dtor 自动调用 delete 其所指对象，
}
~~~

但正因 auto_ptr 的特性，需注意，永远不要使得多个 auto_ptr 指向同一个对象，否则，对象会被多次 delete，进而导致 undefined behavior。
因此，为了避免该问题，auto_ptr 具有一个不寻常的特性：copy auto_ptr（通过 copy constructor 或 copy assignment operaotr）将会使得源对象（拷贝出的对象）指向 null，目标对象（拷贝到的对象）获得资源的唯一拥有权：

~~~cpp
std::auto_ptr<Investment> pInv1(createInvestment()); // pInv1 指向 createInvestment 返回的实例
std::auto_ptr<Investment> pInv2(pInv1); // pInv1 指向 null，pInv2 指向该实例
pInv1 = pInv2; // pInv2 指向 null，pInv1 指向该实例
~~~
由于这个奇特的 copy 行为，以及 auto_ptr 所管理的资源只能有一个 auto_ptr 指向它的底层要求，因此 auto_ptr 并不是管理所有动态申请资源的最好方式。
例如，STL 容器要求其元素是具有普通 copy 行为的，所以这些 stl 容器不能使用 auto_ptr 作为元素。

### 个人注：

>[^1]
>Because of these unusual copy semantics, auto_ptr may not be placed in standard containers
>由于这些不普通的 copy 语义，auto_ptr 不可用于标准容器中

实际上，我个人在 c++98 尝试了一下，没法向 STL 容器中加入 auto_ptr 元素，会报编译错误。

此外，即便在 STL 容器中加入的 auto_ptr 元素（虽然我没有成功），STL 中可能会存在 copy 操作，例如 std::sort 排序时存在将容器元素赋值给临时变量的 copy 操作，因此若使用 auto_ptr 作为元素的 STL 容器调用 std:sort，那么在赋值给临时变量时，容器中元素的资源所有权转移给了临时变量，即容器中的元素 auto_ptr 被置为了 null，随后临时变量超出作用域时会析构，资源被释放。

甚至只需要一个简单的访问容器元素语句 std::auto_ptr\<T\> ptr = vec\[index\]; 就能使容器中的 auto_ptr 置为 null。


## 使用 std::str1::shared_ptr

## 使用 std::unique_ptr, std::shared_ptr，std::weak_ptr（c++11 起）

[^1]: https://en.cppreference.com/w/cpp/memory/auto_ptr