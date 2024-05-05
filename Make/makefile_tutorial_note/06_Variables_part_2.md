[TOC]

## Variables Part 2

#### Flavors and modification

变量有两种类型：

- 简单展开（Simply expanded）（使用 := 赋值）：类似于通常的命令式编程——只有当前已定义的变量才会被展开
- 递归（Recursive）（使用 = 赋值）：仅在 commands 使用到该变量时，才查找该变量展开。而非定义时。

例如：

~~~Makefile
# Recursive variable. This will print "later" below
# one 使用 := 赋值，属于 Recursive variable，在 commands 中使用到该变量时才对其进行展开，而执行到下面的 echo $(one) 时，one 使用的 later_variable 此时已定义，${later_variable} 为 later，因此执行 commands 时，得到 one 的值为 one later
one = one ${later_variable}
# Simply expanded variable. This will not print "later" below
# two 使用 := 赋值，属于 Simply expended variable，在定义时就对其进行展开，而其使用的 later_variable 此时尚未定义，${later_variable} 为空，因此 two 的值为 two
two := two ${later_variable}

later_variable = later

all: 
	echo $(one)
	echo $(two)
~~~

使用 simply expended 可以给变量添加内容，而这种场景下如果使用 recursive 则会导致无限循环错误：

~~~Makefile
one = hello
# one gets defined as a simply expanded variable (:=) and thus can handle appending
one := ${one} there

all: 
	echo $(one)
# shell 结果:
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# echo hello there
# hello there
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

未定义的变量，其值为空

~~~Makefile
all: 
	# Undefined variables are just empty strings!
	echo $(nowhere)
~~~

?= 仅当变量未定义时才定义变量

~~~Makefile
one = hello
# 由于 one 已定义，因此这句 one ?= 不会再次定义
one ?= will not be set
two ?= will be set

all: 
	echo $(one)
	echo $(two)

# shell 结果
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# echo hello
# hello
# echo will be set
# will be set
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

前导空白会被截去，但后导空白不会被截去。
如果想要一个变量为空白字符，则需使用 $(nullstring)

~~~Makefile
# 这里使用 "" 双引号中的内容表示变量的值（不含双引号）
# $(with_spaces) 的值为 "hello   "，注意包括了最后 3 个空白，但 = 等号与 h 之间的前导空格是忽略的。
with_spaces = hello   # with_spaces has many spaces after "hello"
# 因此 $(after) 的值为 "hello   there"
after = $(with_spaces)there

nullstring =
space = $(nullstring) # Make a variable with a single space.

all: 
	echo "$(after)"
	echo start"$(space)"end

# shell 结果：
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# echo "hello   there"
# hello   there
# echo " "
# 
# echo ""
# 
# echo start" "end
# start end
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

使用 += 可给直接变量添加内容：

~~~Makefile
foo := start
# 注意 shell 结果中的输出，说明使用 += 实际上是自动添加了一个空格的
foo += more

all: 
	echo $(foo)
	
# shell 结果：
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# echo start more
# start more
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

后续的 String Substitution 一节，也是一个用于修改变量的非常普遍与有用的方式。

#### Command line arguments and override

使用 override 覆写命令行传参：

~~~Makefile
# Overrides command line arguments
override option_one = did_override
# Does not override command line arguments
option_two = not_override
all: 
	echo $(option_one)
	echo $(option_two)

# shell 结果：
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make option_one=hi option_two=hi
# echo did_override
# did_override
# echo hi
# hi
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

注意到，命令行传参了 option_one 和 option_two 都为 hi，但 override 修饰的 option_one 使得其 override 了命令行参数，而 option_two 未使用 override 则没有 override 命令行参数。

#### List of commands and define

命令列表与定义

定义指令 并不是一个函数，尽管它看起来像。它主要用于 定义封装好的 commnads（canned recipes） 、与 eval 函数（eval funciont）配合很好。

define/endef 简单地创建一个变量，其值设为 commands 组成的列表。
注意，define/endef 创建的变量与分号分隔但不换行的 commands 有存在区别，它的每一行仍然是运行在不同的 shell 中的。

~~~Makefile
one = export blah="I was set!"; echo $$blah

# 由于第二行 echo $$blah 在另一个新的 shell 中执行，因此该新 shell 中 blah 未定义，输出为空
define two
export blah="I was set!"
echo $$blah
endef

all: 
	@echo "This prints 'I was set'"
	@$(one)
	@echo "This does not print 'I was set' because each command runs in a separate shell"
	@$(two)
~~~

个人感觉其实就类似于 C/C++ 里面的宏。

为了证实我的猜想，试一下：

~~~Makefile
one = export blah="I was set!"; echo $$blah

define two
export blah="I was set!"
echo $$blah
endef

# three 把命令写成一行了，shell 结果与 one 相同
define three
export blah="I was set!"; echo $$blah
endef

# four 把命令写一半，看看是否是如我猜想一般是像C/C++ 的宏一样的替换
define four
ho "for"
endef

all:
	@echo "This prints 'I was set'"
	@$(one)
	@echo "This does not print 'I was set' because each command runs in a separate shell"
	@$(two)
	@echo $(three)
	@ec$(four)

# shell 结果：
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# This prints 'I was set'
# I was set!
# This does not print 'I was set' because each command runs in a separate shell
# 
# I was set!
# four
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### Target-specific variables

指定目标的变量

可为指定 targets 设置变量：

~~~Makefile
all: one = cool

all: 
	echo one is defined: $(one)
other:
	echo one is nothing: $(one)

# shell 结果：
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make all
# echo one is defined: cool
# one is defined: cool
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make other
# echo one is nothing:
# one is nothing:
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

自己再试了一下：

~~~Makefile
all: one = cool
other: one = not cool

all:
	echo one is defined: $(one)
other:
	echo one is nothing: $(one)

# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make all
# echo one is defined: cool
# one is defined: cool
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make other
# echo one is nothing: not cool
# one is nothing: not cool
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### Pattern-specific variables

指定模式的变量

可为指定模式设置变量：

~~~Makefile
%.c: one = cool

blah.c: 
	echo one is defined: $(one)
other:
	echo one is nothing: $(one)

# shell 结果：
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make blah.c
# echo one is defined: cool
# one is defined: cool
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make other
# echo one is nothing:
# one is nothing:
# whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

2024.05.05