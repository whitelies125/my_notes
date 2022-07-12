# When Do We Use Exceptions?

何时应该使用异常？

## When to Use Exceptions

何时使用异常机制

Throw an exception when your program can identify an external problem that prevents execution.

当一个外部的问题阻止你的程序运行时，抛异常。

-  从服务器接收到非法数据。
- 磁盘满了。
- 宇宙射线阻止你查询数据库。

Throw an exception if the function couldn’t do what it advertised, and establish its postconditions.

如果函数无法完成它所告知的功能并建立其正常的后置状态，抛异常。

- 构造函数失败。

  例如vector的构造函数应创建一个对象，但对象占内存太大导致无法构建，那么应该抛异常。

## Not to Use Exceptions

何时不用异常。

Simple errors that may occur in individual functions are best handled locally without throwing exceptions.

只发生在单独函数中的简单错误不要用异常处理。

Do not use throw to indicate a coding error in usage of a function.

不要用异常处理编码错误。

- 可以用 assert() 中断程序执行然后调试。

Do not use exceptions for control flow.

不要用异常来控制程序流程。

- 不要用throw来结束循环。

适度：不可不用，不可多用。

- 实时系统中不用异常航天飞机控制程序、生命维持系统等