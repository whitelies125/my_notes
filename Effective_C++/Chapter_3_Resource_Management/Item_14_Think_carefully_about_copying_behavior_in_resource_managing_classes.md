条款 14：在资源管理类中小心 coping 行为

[[Item_13_Use_objects_to_manage_resources]] 中主要讲述的是对 heap-based 的资源管理，然而并非所有资源都是 heap-based 的。
对于这类非 heap-based 资源，如 auto_ptr 或 tr1::shared_ptr 的智能指针就不再适合作为资源处理者（handler）（个人注：毕竟栈变量，超出作用域就自动析构了，而堆变量因为需要手动调用析构才释放因此需要绑定一个智能指针，依赖智能指针的析构调用析构函数）
在这种情况下，你可能需要建立自己的资源管理类。

# 引子
这里假定使用 C 的 API 操作类型为 Mutex 的互斥对象，提供了 lock 和 unlock 函数：
```cpp
void lock(Mutex *pm); // 锁定 pm 所指的 Mutex
void unlock(Mutex *pm); // 解锁 pm 所指的 Mutex
```
为了确保不好忘记将已加锁的 Mutex 解锁，你可能会遵循 RAII 原则，创建一个如下所示的类来管理锁，即构造时申请资源，析构时释放资源：
```cpp
class Lock {
public:
    explicit Lock(Mutex *pm) : mutexPtr(pm) {
        lock(mutextPtr); // 申请资源
    }
    ~Lock() {
        unlock(mutexPtr); // 释放资源
    }
private:
    Mutex *mutexPtr;
}
```

使用者则以常规的 RAII 方式使用 Lock 类：
```cpp
Mutex m;
{ // 创建一个 block
    Lock(m); // 加锁
} // 在 block 结束时，Lock 自动析构，解锁
```

到此为止，表现良好。

接下来考虑，如果使用者进行了拷贝操作的场景。
**这同时也是每一个 RAII class 作者必定要面对的问题：当 RAII 对象进行拷贝时应该发生什么？**

```cpp
Lock ml1(&m);
Lock ml2(ml1); // 发生拷贝，预期会发生什么？
```

# 对 RAII 对象发生拷贝的处理方法
通常而言，对于 RAII 对象的拷贝行为，我们有以下预期及处理方法

## Prohibit copying 禁止拷贝
大多数场景，对 RAII 对象进行拷贝操作是不合理的。
因此当拷贝操作对 RAII 类而言是无意义、不合理时，如上述的 Lock 类，应直接禁止该操作。
如何禁止可参见[[Item_6_Explicitly_disallow_the_use_of_compiler_generated_functions_you_do_not_want]]

## Reference-count the underlying resource 对底层资源使用引用计数
假若希望保有资源，并且直到最后一个使用者销毁，则在这种情况下，应将该资源的被引用数递增。

## Copy the underlying resource 拷贝底层资源

## Transfer ownership of the underlying resource 转移底层资源所有权