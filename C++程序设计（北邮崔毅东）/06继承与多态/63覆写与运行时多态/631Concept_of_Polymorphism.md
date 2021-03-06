# Concept of Polymorphism

多态的概念。

## What is Polymorphism?

广义的多态：不同类型的实体/对象对于同一消息有不同的响应，就是 OOP 中的多态性。

截至目前，多态性有两种表现的方式。

翻了一下，慕课网上这个课程第八次开课时间为 2022.02.21 到 2022.06.30，至少这第八次开课里的视频内容，此处仍然是 " 截至目前，多态性有两种表现的方式 "。

截至目前，多态性有两种表现的方式：

**重载多态**：调用同名重载函数，而因为参数不同，得到不同的响应，也就是调用不同的重载函数。

**子类型多态**：不同对象调用同名重定义函数，表现出不同的行为。也就是调用的是子类各自的重定义函数。

使用 Binding（联编）来实现多态。

## Binding

**Binding（联编）**：确定具有多态性的语句调用哪个函数的过程。

- **Static Binding（静态联编）**：

  在程序编译时（Compile-time）确定调用哪个函数。

  例如：函数重载就是静态联编。

  模板多态也是编译时多态，也是静态联编。

- **Dynamic Binding（动态联编）**：

  在程序运行时（Run-time），才能够确定调用哪个函数。

  用动态联编实现的多态，也称为**Run-time Polymorphism（运行时多态）**。

C++ 中说多态，多是指运行时多态。

