# Introduce to STL

标准模板库（STL）介绍。

## What is Standard Template Library?

#### 来源

- STL 之父 Stepanov（合作者 Meng Lee）
- STL 于 1994 年 2 月正式称为 ANSI/ISO C++ 的一部分。

#### Components of STL（STL 的组成部分）

Containers（容器）、Iterators（迭代器）、Algorithms（算法）、Function Objects（函数对象）、Memory Allocation（空间分配器）。

主要介绍前三个组件：

Containers: store a collection of data, often referred to as elements.

容器：用于保持一组数据，数据个体被称为元素。

Iterators: facilitate traversing through the elements in a container.

迭代器：用于遍历容器中的元素。

容器都有自己专有的迭代器，只有容器才知道如何遍历自己的元素。

Algorithms: manipulate data such as sorting, searching...

算法：用于操作容器中的数据。

算符独立于容器。

## Categories of Containers

容器分类。

**Sequence containers（顺序容器）** represent linear data structures（线性数据结构：多个元素的有序集合，有头有尾有前有后）.

- vector，list，deque

**Associative containers（关联容器）** are non-linear containers that can locate elements stroed in the container quickly（可快速定位元素的非线性数据结构）.

- Such containers can store sets of values or **key/value pairs**（这类容器可以存储值的集合或键值对）.
- set，multiset，map，multimap（multi- 表示其中元素可重复）

**Container adapters（容器适配器）** are constrained version of sequence containers, aiming at handling special cases（是顺序容器的受限版本，用于处理特殊情况）.

- stack，queue，priority_queue

## Iterators

迭代器。

Iterators are a generalization of pointers.

迭代器是一种泛型指针。

The array pointers can be treated as iterators.

数组指针可被看作迭代器。

Usage:

- Iterators are often used with containers.

  迭代器一般与容器共同使用。

- but  some iterators are NOT associated with containers.

  有些迭代器与容器无关。

  - istream_iterator,ostream_iterator.

## Algorithm

算法。

The terms operations, algorithms, and functions are interchangeable.

术语：操作、算法、函数经常可以相互替代。
