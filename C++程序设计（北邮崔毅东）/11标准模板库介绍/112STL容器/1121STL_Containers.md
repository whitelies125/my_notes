# STL Containers

STL 容器。

这里至少简单介绍 STL 容器及其通用的一些操作方法。

## Container Classes

容器类。

| STL Container             | Header     | Applications                                                 |
| ------------------------- | ---------- | ------------------------------------------------------------ |
| vector                    | \<vector\> | 直接访问任意元素，快速插入、删除尾部元素                     |
| deque(double-ended queue) | \<deque\>  | 直接访问任意元素，快速插入、删除头部和尾部元素               |
| list                      | \<list\>   | 快速插入、删除任意位置元素                                   |
| set                       | \<set\>    | 快速查询元素，无重复关键字                                   |
| multiset                  | \<set\>    | 与set相同，但允许重复关键字                                  |
| map                       | \<map\>    | Key/value pair mappint（键值对映射）。不允许重复关键字，使用关键字快速查询元素 |
| multimap                  | \<map\>    | 与map相同，但允许重复关键字                                  |
| stack                     | \<stack\>  | 后进先出容器                                                 |
| queue                     | \<queue\>  | 先进后出容器                                                 |
| priority_queue            | \<queue\>  | 高优先级元素先删除                                           |

## Common Functions to All Containers

所有容器共同函数。

| Functions                             | Description                                    |
| ------------------------------------- | ---------------------------------------------- |
| non-arg constructor（无参构造函数）   | 构造一个空容器                                 |
| constructor with args（带参构造函数） | 每个容器都有多个带参数的构造函数               |
| copy constructor（拷贝构造函数）      | 创建一个容器，从一个已有的同类型容器中复制元素 |
| destructor（析构函数）                | 容器销毁后执行清理工作                         |
| empty()                               | 若容器中没有元素则返回空                       |
| size()                                | 返回容器中的元素数目                           |
| operator=                             | 将容器内容复制到另一个容器                     |
| Relational operators(<.<=,>,>=,!=)    | 顺序比较两个容器中的对应元素，来确定大小关系   |

## Common Function to First-Class Containers

一级容器的通用函数。

一级容器，指顺序容器和关联容器。
而容器适配器，就是二级容器。

| Functions        | Descripton                                             |
| ---------------- | ------------------------------------------------------ |
| c1.swap(c2)      | 交换两个容器c1和c2的内容                               |
| c1.max_size()    | 返回一个容器可以容纳的最大元素数量                     |
| c.clear()        | 删除容器中所有元素                                     |
| c.begin()        | 返回容器首元素的迭代器                                 |
| c.end()          | 返回容器尾元素之后位置的迭代器                         |
| c.rbegin()       | 返回容器尾元素的迭代器，用于逆序遍历                   |
| c.rend()         | 返回容器首元素之前位置的迭代器，用于逆序遍历           |
| c.erase(beg,end) | 删除容器中从beg到end-1之间的元素，beg和end都是迭代器。 |

