# Meta-Programming and Generic Programming

元编程与泛型编程。

C++ 是一种多模式的编程语言，支持结构化编程，面向对象编程，函数式编程，模板元编程或泛型编程。

## Concepts of X-Programming

编程的概念。

**Programming（编程）**：Writing a program that creates, transforms, filters, aggregates and otherwise manipulates **data**.

写程序来操作数据。

**Metaprogramming（元编程）**：Writing a program that creates, tranforms, filters, aggregates and otherwise manipulates **programs**.

写程序来操作程序。

**Generic Programming（泛型编程）**： Writing a program that creates, transforms, filters, aggregates and otherwise manipulates data, **but** makes only the minimum assumptions about the structure of the data, thus maximizing reuse across a wide range of datatypes.

写程序来操作程序，但只对数据的结构做最小假设，以最大化该程序可重用于更广泛的数据类型的能力。

## C++: Meta/Generic Programming

C++ 的元编程和泛型编程。

C++ 的元编程和泛型编程，并不是区分开的。

C++ implements MetaProgramming with "template" to produce template instance, i.e. programs, in compiling time.

C++ 用模板实现元编程，由编译器，在编译期，根据模板生成模板示例，即生成程序。

Generic programming in C++(i.e. compile-time polymorphism) is accomplished by metaprogramming(i.e. code generation from templated code).

C++ 的泛型编程（即编译时多态），是籍由元编程实现的（即由代码模板生成代码）。