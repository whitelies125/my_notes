资源就是指，一旦你使用了它，你就需要将它归还给系统。
如果没有这样做，就会发生糟糕的事情。

Item 13：以对象管理资源

# 引子
假设在 func 函数中通过工厂模式[[Item_7_Declare_destructor_virtual_in_polymorphic_base_classes]]由 createInvestment 函数返回基类指针，随后在 func 函数末尾释放它：

~~~cpp
Investment * createInvestment(); // factory function 返回实例指针
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

auto_ptr 的替代方案是 reference-counting smart pointer（RCSP，引用计数的智能指针）。
RCSP 是能够持续追踪有多少个对象指向同一特殊资源的智能指针，并且当不再有对象指向它时自动 delete 该资源。
如此，RCSP 提供了类似 garbage collection（GC，垃圾回收）的行为。
~~~cpp
void f() {
    std::tr1::shared_ptr<Investment> pInv1(createInvestment()); // pInv1 指向 createInvestment 返回的实例
    std::tr1::shared_ptr<Investment> pInv2(pInv1); // pInv1 和 pInv2 都指向同一个实例
    pInv1 = pInv2; // pInv1 和 pInv2 都指向同一个实例，无变化
    // pInv1，和 pInv2 析构，无任何对象指向该实例时，该实例也销毁
}
~~~

因此，std::str1::shared_ptr 的 copy 行为是寻常的，即可用于 STL 容器，和其他不适用于使用不寻常拷贝行为的 auto_ptr 的场景。

但是 RCSP 无法打破 cycles of references（环状引用），例如两个对象虽已不使用但二者互相指向，则不满足 "不再有对象指向它" 的条件而导致无法自动释放。

Note，
auto_ptr 和 std::str1::shared_ptr 二者的析构函数中，都使用的是 delete 而非 delete\[\] 进行资源释放。
因此，二者不适用于数组类型：
~~~cpp
void func() {
    std::auto_ptr<int> api (new int[1024]); // 错误，析构时使用的是 delete 而非 delete[]
    std::tr1::shared_ptr<std::string> sps(new std::string[10]); // 同样会造成资源泄漏
}
~~~

## 使用 std::unique_ptr, std::shared_ptr，std::weak_ptr（c++11 起）
自 C++11 起，标准库提供了三种智能指针来管理动态内存。
但这里就不详细叙述了。

## Things to Remember
- To prevent resource leaks, use RAII objects that acquire resources in their constructors and release them in their destructors.
    为了避免资源泄漏，使用在其构造函数中申请资源、析构函数中释放资源的 RAII 对象。
- Two commonly useful RAII classes are tr1::shared_ptr and auto_ptr. tr1::shared_ptr is usualy the better choice, because its behavior when copied is intuitive. Copying an auto_ptr sets is to null.
    两个常见有用的 RAII 类是 tr1::shared_ptr 和 auto_ptr。tr1::shared_ptr 通常是更佳选择，因为其拷贝行为是符合直觉的。而拷贝 auto_ptr 会使其置为 null。

[^1]: https://en.cppreference.com/w/cpp/memory/auto_ptr

2025.02.26