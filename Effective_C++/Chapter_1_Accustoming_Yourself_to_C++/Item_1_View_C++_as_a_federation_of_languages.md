# 条款 1：视 C++ 为一个语言联邦

C++ 大致上可分为：
- C：主要针对 C++ 中关于 C 语言的部分，包括：区块（blocks）、语句（statements）、预处理器（preprocessor）、内置数据类型（built-in data types）、数组（arrays）、指针（pointers）等。而这部分语言也有局限：没有模板、异常以及重载等。
- Object-Oriented C++：C with Classes 部分，包括：类（classes，构造函数和析构函数）、封装（encapsulation）、继承（inheritance）、多态（polymorphism）、虚函数（virtual function、动态绑定）等。
- Template C++：C++ 的泛型编程部分（generic programming）。良好的编程守则中，仅对 template 适用的特殊条款并不罕见。template 中的相关考虑和设计遍布整个 C++，甚至由于其功能强大，已经衍生出新的编程范型（programming paradigm），即模板元编程（template metaprogramming， TML）
- STL： C++ 中的一个 template 程序库，主要由容器（container）、迭代器（iterator）、算法（algorithm）以及函数对象（function obejcts）组成，相互直接紧密配合和协调。相对于其他 template 程序库， STL 有自己的特点。所以我们在使用 STL 的时候，需要遵守他的一些规约，这会在后面的条款中介绍。

> Things to Remember:
> - Rules for effective C++ programming vary, depending on the part of C++ you are using.
>   C++ 高效编程守则是变化的，取决于你正使用 C++ 的那一部分。
