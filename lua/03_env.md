### 前置：

#### upvalue

《Lua 5.4 Reference Manual》3.5 – Visibility Rules

> A local variable used by an inner function is called an *upvalue* (or *external local variable*, or simply *external variable*) inside the inner function.

一个被用于内层函数的局部变量，在内层函数中被称为 **upvalue** 或**外部局部变量**或**外部变量**。

#### chunk

《Lua 5.4 Reference Manual》 9 – The Complete Syntax of Lua

> ### 3.3.2 – Chunks
>
> The unit of compilation of Lua is called a *chunk*. Syntactically, a chunk is simply a block:
>
> ```
> 	chunk ::= block
> ```
>
> Lua handles a chunk as the body of an anonymous function with a variable number of arguments (see §3.4.11). As such, chunks can define local variables, receive arguments, and return values. Moreover, such anonymous function is compiled as in the scope of an external local variable called `_ENV` (see §2.2). The resulting function always has `_ENV` as its only external variable, even if it does not use that variable.
>
> A chunk can be stored in a file or in a string inside the host program. To execute a chunk, Lua first *loads* it, precompiling the chunk's code into instructions for a virtual machine, and then Lua executes the compiled code with an interpreter for the virtual machine.

Lua 中一个编译单元被称为一个 chunk。

Lua 将一个 chunk 当作一个可变参数的匿名函数的函数体来处理。例如，chunks 可以定义局部变量，接收参数，和返回值。此外，匿名函数在名为 `_ENV` 的外部局部变量范围中被编译。这个函数总是有着 `_ENV` 作为它唯一的外部变量，即便它并不使用 `_ENV` 。

一个 chunk 可以被保存在一个文件或宿主程序内的一个字符串中。为了执行一个 chunk，Lua 首先加载它，预编译该 chunk 的代码为虚拟机的指令，然后 Lua 使用虚拟机的解释器执行编译好的代码。

---

简而言之，在我的工作场景中，一个 .lua 文件，就是一个 chunk。
在编译时，一个chunk（在我的工作场景中是一个 .lua 文件中的代码）作为一个可变参数匿名函数的函数体来处理，并且其存在一个名为 `_ENV` 的 upvalue。

### 2.2 – Environments and the Global Environment

> As we will discuss further in §3.2 and §3.3.3, any reference to a free name (that is, a name not bound to any declaration) var is syntactically translated to _ENV.var. Moreover, every chunk is compiled in the scope of an external local variable named _ENV (see §3.3.2), so _ENV itself is never a free name in a chunk.

任何**自由名字**（此处意为，未绑定任何声明的名字）`var`，在语法上被翻译为 `_ENV.var`。此外，每个 chunk 都在一个名为 `_ENV` 的外部的局部变量的范围中被编译，所以在一个 chunk 中， `_ENV` 它自身永远不会是一个自由变量。

> Despite the existence of this external _ENV variable and the translation of free names, _ENV is a completely regular name. In particular, you can define new variables and parameters with that name. Each reference to a free name uses the _ENV that is visible at that point in the program, following the usual visibility rules of Lua (see §3.5).

尽管有着外部 `_ENV` 变量的存在和自由名字的翻译，但 `_ENV` 仍然是一个完全规则的名字。特别地，你可以使用这个名字 `_ENV` 定义新变量和参数。每一个对自由名字的引用使用的 `_ENV` 是在程序中可见的 `_ENV` ，关于可见性规则详见 §3.5。

> Any table used as the value of _ENV is called an environment.

任何用于 `_ENV` 的值的 table 被称为一个**环境**

> Lua keeps a distinguished environment called the global environment. This value is kept at a special index in the C registry (see §4.3). In Lua, the global variable _G is initialized with this same value. (_G is never used internally, so changing its value will affect only your own code.)

Lua 维持一个称为**全局环境**的特殊环境。这个值被维持在 C 注册表中的一个特殊索引中（参见 §4.3）。 在 Lua 中，全局变量 `_G` 被初始化为相同的值。（`_G` 从不在内部使用，所以更改它的值只会影响您自己的代码。）

> When Lua loads a chunk, the default value for its _ENV variable is the global environment (see load). Therefore, by default, free names in Lua code refer to entries in the global environment and, therefore, they are also called global variables. Moreover, all standard libraries are loaded in the global environment and some functions there operate on that environment. You can use load (or loadfile) to load a chunk with a different environment. (In C, you have to load the chunk and then change the value of its first upvalue; see lua_setupvalue.)

当 Lua 加载一个 chunk 时，它的 `_ENV` 变量的默认值是这个全局环境 `_G` （参见 load 一节）。 因此，默认情况下，Lua 代码中的自由名字指的是全局环境 `_G` 中的条目，因此也称为**全局变量**。 此外，所有标准库都加载到全局环境 `_G` 中，并且其中的一些函数在该环境中运行。 您可以使用 load（或 loadfile）加载具有不同环境的 chunk。（在 C 中，您必须加载 chunk 然后更改其第一个 upvalue 的值；请参阅 lua_setupvalue。）