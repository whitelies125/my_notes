[TOC]

## Targets

#### The all target

~~~Makefile
all: one two three

one:
	touch one
two:
	touch two
three:
	touch three

clean:
	rm -f one two three
~~~

将名为 all 的 target 放在第一个规则，并将想要 make 的所有 targets 都作为 all 的 prerequisites，即可执行 make 便能够方便的 make 所有 targets。

#### multiple targets

~~~Makefile
all: f1.o f2.o

f1.o f2.o:
	echo $@
# Equivalent to:
# f1.o:
#	 echo f1.o
# f2.o:
#	 echo f2.o
~~~

当一个 rule 有多个 targets 时，将会为每个 target 执行检查与操作，等价于分开写。

提供了一种可用的、方便的语法

$@ 是自动变量 automatic variable，在此处其值是 target 的字符串。
通过使用 $@，在合并写成多个 targets 时，也能够实现在 command 中使用 target 各自的名字。

2024.05.01

