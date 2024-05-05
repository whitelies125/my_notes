[TOC]

## Fancy Rules

#### Implicit Rules

make 对于 C 和 C++ 语言存在一些隐式规则（当然不止下面这些隐式规则）

1. 编译 C 程序时：make 会使用 $(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@ 命令，从 n.c 文件自动生成 n.o 文件
2. 编译 C++ 程序时：make 会使用 $(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $^ -o $@ 命令，从 n.cc 或 n.cpp 文件自动地生成 n.o 文件
3. 链接单个目标文件时：make 会使用 $(CC) -c $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@ 命令自动地生成 n。

上面这些被隐式规则使用变量，含义为（当然不止这些变量）：

- CC：用于编译 C 语言的程序名，默认是 cc
- CXX：用于编译 C++ 的程序名，默认是 g++
- CFLAGS：提供给 C 编译器的额外标识
- CXXFLAGS：提供给 C++ 编译器的额外标识
- CPPFLAGS：提供给 C 预处理器的额外标识
- LDFLAGS：提供给编译器，用于其调用链接器时传递的标识

由于隐式规则的存在，因此对于 Makefile 可以写成：

~~~Makefile
CC = gcc # Flag for implicit rules
CFLAGS = -g # Flag for implicit rules. Turn on debug info

# Implicit rule #1: blah is built via the C linker implicit rule
# Implicit rule #2: blah.o is built via the C compilation implicit rule, because blah.c exists
blah: blah.o

blah.c:
	echo "int main() { return 0; }" > blah.c

clean:
	rm -f blah*
~~~

下面为个人理解，实际上我个人理解为，当满足条件时，make 会为 C 和 C++ 程序自动补充或补全 rule。不建议使用隐式规则。

执行 make，过程：

1. 开始

2. blah 作为目标，其 prerequisites 中 blah.o 不存在

3. 虽然并没有写 blah.o 的 rule，但由于 Makefile 中有名为 blah.c 的 targets，因此 Makefile 的隐式规则会自动地从 blah.c 生成 blah.o，即隐式存在着一个 rule 为

   ~~~Makefile
   blah.o: blah.c
   	$(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@
   ~~~

4. 由于 blah.c 不存在，执行 blah.c 的 commands，生成了 blah.c

5. 随后执行隐式规则的 $(CC) -c $(CPPFLAGS) $(CFLAGS) $^ -o $@ 生成 blah.o

6. 注意到 blah 并没有写 commands，同样由隐式规则得出，隐式存在着一条 commands 为 $(CC) -c $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@，由此生成 blah

7. 结束

#### Static Pattern Rules

另一种少费笔墨的方式。

其语法格式为：

~~~Makefile
targets...: target-pattern: prereq-patterns ...
   commands
~~~

含义为：根据 target-pattern 从 targets 中匹配字符串，被匹配到的字符串被称为 stem。随后 stem 会替换  prereq-patterns 中的占位符，以生成 prerequisites。

例如，一个通常的从 foo.c，bar.c，all.c 生成 all 的 Makefile 如下：

~~~Makefile
objects = foo.o bar.o all.o
all: $(objects)

# These files compile via implicit rules
foo.o: foo.c
bar.o: bar.c
all.o: all.c

all.c:
	echo "int main() { return 0; }" > all.c

%.c:
	touch $@

clean:
	rm -f *.c *.o all
~~~

通过使用 static pattern rule 可写为：

~~~Makefile
objects = foo.o bar.o all.o
all: $(objects)

# These files compile via implicit rules
# Syntax - targets ...: target-pattern: prereq-patterns ...
# In the case of the first target, foo.o, the target-pattern matches foo.o and sets the "stem" to be "foo".
# It then replaces the '%' in prereq-patterns with that stem
$(objects): %.o: %.c

all.c:
	echo "int main() { return 0; }" > all.c

%.c:
	touch $@

clean:
	rm -f *.c *.o all
~~~

隐式规则的部分不谈。

首先前文关于通配符的小节已说明，通配符 % 在 static pattern rule 中代表着匹配到的字符串，即 stem。

这里的逻辑为， 

1. $(objects) 的值为 foo.o bar.o all.o，有三个，根据 multiple targets 的语法，实际上等价于给三个 targets 分开写。

2. 对于第一个 foo.o 而言，其 rule 为：

   ~~~Makefile
   foo.o: %.o : %.c
   ~~~

3. 因此 %.o（target-pattern ），从 foo.o（targets）匹配到 foo（stem），然后将 %.c（prereq-patterns）替换为 foo.c（prerequisites）


关于 %.c: 在下面文末解释。

#### Static Pattern Rules and Filter

关于函数的部分后续讲解。

这里讲解 Filter 函数是因为该函数可用于 Static Pattern Rules 中来使得我们得到正确的文件名。

~~~Makefile
obj_files = foo.result bar.o lose.o
src_files = foo.raw bar.c lose.c

all: $(obj_files)
# Note: PHONY is important here. Without it, implicit rules will try to build the executable "all", since the prereqs are ".o" files.
.PHONY: all 

# Ex 1: .o files depend on .c files. Though we don't actually make the .o file.
$(filter %.o,$(obj_files)): %.o: %.c
	echo "target: $@ prereq: $<"

# Ex 2: .result files depend on .raw files. Though we don't actually make the .result file.
$(filter %.result,$(obj_files)): %.result: %.raw
	echo "target: $@ prereq: $<" 

%.c %.raw:
	touch $@

clean:
	rm -f $(src_files)
~~~

#### Pattern Rules

模式规则虽然常用但是是否令人不解，可从两个角度看待 Patter Rules：

- 自定义 implicit rule 的方式
- static pattern rules 的简化形式

如上述例子中的 %.c:，%.c %.raw，以及下面的这个例子 %.o: %.c 都是 pattern rules：

~~~Makefile
# Define a pattern rule that compiles every .c file into a .o file
%.o : %.c
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@
~~~

在 target 中含有通配符 % 的才是 pattern rules，注意这里未要求 prerequisites。
target 字符串中，% 将会匹配任何非空字符串，其他字符则是完全匹配。% 匹配到的字符串仍称为 stem。随后 prerequisite 中的 % 则被 stem 替换。

#### Double-Colon Rules

双冒号规则很少使用，它允许对同一个 target 定义多个 rule。

~~~makefile
all: blah

blah::
	echo "hello"

blah::
	echo "hello again"
~~~

如果使用单冒号定义了多个 rule，那么 make 将会报出告警，并且只会使用最后一个 rule。

关于双冒号规则：https://www.gnu.org/software/make/manual/html_node/Multiple-Rules.html#Multiple-Rules

### 个人补充：

#### 关于 pattern rules

https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html

虽然本教程中写的是两种看待方式，但根据 make 官方文档来说

> You define an implicit rule by writing a pattern rule.
> 你可以使用 pattern rule 来定义一个隐式规则
>
> A pattern rule looks like an ordinary rule, except that its target contains the character ‘%’ (exactly one of them).
>
> 模式规则看起来与普通规则类似，只是它的目标包含字符%(正好是其中之一)。
>
> Thus, a pattern rule ‘%.o : %.c’ says how to make any file stem.o from another file stem.c.
> 因此，一个模式规则 %.o : %.c 说明了对任意的 stem.o 如何从 stem.c 生成

根据第二句话，只提及了 targets，但并未要求 prerequisites，因此 pattern rules 的语法格式为：

~~~Makefile
# targets 包含 %
targets...: prerequisites ...
   commands
~~~

根据第一、三句话，以及 pattern rule 一节在 Using Implicit Rules 中，和后续在 Canceling Implicit Rules 一节中，也指出了可以定义一个与 make 内置隐式规则具有相同 target、prerequisites 相同的 rule，来重定义或取消（不写 commands）这个隐式规则。

所以我个人跟从官方文档，认为 pattern rules 是定义或重定义的隐式规则。

于是可以解释上文的 Static Pattern Rules 一节例子中的 %c:，Static Pattern Rules and Filter 一节例子红的 %c %raw:，其作用是定义了隐式规则，告诉 make 对于 .c，.raw 文件使用该规则进行生成。

继续完成 Static Pattern Rules 一节中的执行流程：

4. 对 foo.c 进行检查与操作，发现不存在 foo.c 文件
5. 使用隐式规则 %c: 进行生成，该 rule 没有 prerequisites，因此递归检查操作结束，执行该 rule 的 commands
6. foo.o 结束。

bar.o 与 foo.o 相同不赘述。那么 all.o 呢。

注意到，例子中明确定义了生成 all.c 的 rule。

根据官方文档 Multiple Rules for One Target 一节：https://www.gnu.org/software/make/manual/html_node/Multiple-Rules.html#Multiple-Rules

> If none of the explicit rules for a target has a recipe, then `make` searches for an applicable implicit rule to find one see [Using Implicit Rules](https://www.gnu.org/software/make/manual/html_node/Implicit-Rules.html)).
> 如果 target 的所有显式规则都没有 commands，那么在隐式规则中搜寻一个可应用的 rule

我对这句话的理解是，如果有显式规则，就不用隐式规则了（大概就像 C++ 的函数重载决议，优先匹配最精确的）。

因此 all.c 使用的是显式规则 all.c: 生成的，而非使用 %.c: 隐式规则生成。

更进一步地，如果没有显式规则，文件匹配上了多个隐式规则呢？

参见官方文档 How Patterns Match 一节： https://www.gnu.org/software/make/manual/html_node/Pattern-Match.html

个人总结：

1. 若显式规则有多个，单冒号，报错且只执行最后一个；双冒号，不报错，从前往后执行所有 rules
2. 显式规则优先级高于隐式规则，若找到了显式规则，则不使用隐式规则
3. 可使用 pattern rules 定义或重定义隐式规则。
4. 若无显式规则，且文件匹配上多个隐式规则，则执行拥有最短的 stem 的 rule。

2024.05.02

