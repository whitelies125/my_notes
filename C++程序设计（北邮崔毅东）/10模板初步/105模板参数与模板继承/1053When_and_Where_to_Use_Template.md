# When and  Where to Use Template

何时何地使用模板编程？

#### 使用别人写好的模板库时

- STL (Standard Template Library) 
- Boost

#### 对不同类型的数据做类似处理 (算法、容器、遍历等)

#### 过量使用模板不好(不要模板原教旨主义)

When you want to write code that nobody else on your team can understand

When you want code that you won't be able to understand 7 days after you wrote it

When code performance is more important to you than maintainability

When you want to be able to list "Template Metaprogramming" as a skill on your resumé.

When you need to write code that's unlikely to work on many real-world compilers

## 2. OOP or GP?

 面向对象编程 or 泛型编程？

C++: Generic programming is very widely useful, and can often to a large extent replace OOP：

泛型编程在C++中广泛使用，经常可以取代面向对象编程。

- Almost the entire standard library relies on generic programming.

  几乎整个C++标准库都依赖于泛型编程。

- There is little inheritance or polymorphism in the standard library (not/incl: exception, string and stream class)

  在C++标准库较少使用继承和运行时多态（除了异常、字符串和IO流中使用了较多的继承）。