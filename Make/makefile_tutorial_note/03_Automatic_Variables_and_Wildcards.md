[TOC]

## Automatic Variables and Wildcards

#### * Wildcard

通配符 * 会在你的**文件系统中**匹配所有满足条件的文件名。

下例中，$(wildcard *.c) 就代表了目录中所有的 .c 文件的文件名 a.c b.c

~~~Makefile
# Print out file information about every .c file
print: $(wildcard *.c)
	ls -la  $?
~~~

运行结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ ls
Makefile  a.c  b.c
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
ls -la  a.c b.c
-rw-r--r-- 1 whitelies125 whitelies125 0 May  1 07:47 a.c
-rw-r--r-- 1 whitelies125 whitelies125 0 May  1 07:47 b.c
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

危险：不要在变量定义中使用 *。

危险：当 * 没有匹配到任何文件时，它将保持原样（除非被 wildcard 函数包裹，则为空）

建议：使用 * 通配符时，应该一直使用 wildcard 函数包裹住它。

~~~Makefile
thing_wrong := *.o # Don't do this! '*' will not get expanded
thing_right := $(wildcard *.o)

all: one two three four

# Fails, because $(thing_wrong) is the string "*.o"
one: $(thing_wrong)

# Stays as *.o if there are no files that match this pattern :(
two: *.o 

# Works as you would expect! In this case, it does nothing.
three: $(thing_right)

# Same as rule three
four: $(wildcard *.o)
~~~

#### % Wildcard

通配符 % 虽然确实很有用，但是由于它的可用场景多种多样，着实有点让人摸不着头脑。

- 在 "匹配" 模式下使用时，它匹配字符串中的一个或多个字符，这种匹配被称为词干（stem）匹配。
- 在 "替换" 模式下使用时，它会替换匹配到的词干。
- % 大多用在规则定义以及一些特定函数中。

有关它的用法示例可参阅以下章节：

- 静态模式规则
- 模式规则
- 字符串替换
- vpath 指令

#### Automatic Variables

有很多自动变量，但是经常使用的就几个：

~~~Makefile
hey: one two
	# Outputs "hey", since this is the target name
	echo $@ # 值为 target 名

	# Outputs all prerequisites newer than the target
	echo $? # 值为所有比 target 新的 prerequisites 名

	# Outputs all prerequisites
	echo $^ # 值为所有 prerequisites 名

	touch hey

one:
	touch one

two:
	touch two

clean:
	rm -f hey one two
~~~

2024.05.01