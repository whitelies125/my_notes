[TOC]

## Variables Part 2

### Flavors and modification

>There are different ways that a variable in GNU `make` can get a value; we call them the *flavors* of variables. The flavors are distinguished in how they handle the values they are assigned in the makefile, and in how those values are managed when the variable is later used and expanded.
>
>GNU make 中有两种方式来获取变量的值；我们将其称为变量的 flavors（口味）。不同的 flavors 在它们如何处理在 makfile 中被指派的值，以及当这些值之后被使用时如何管理是不同的。

#### Recursively Expanded

递归展开

https://www.gnu.org/software/make/manual/html_node/Recursive-Assignment.html

> The first flavor of variable is a *recursively expanded* variable. Variables of this sort are defined by lines using ‘=’ (see [Setting Variables](https://www.gnu.org/software/make/manual/html_node/Setting.html)) or by the `define` directive (see [Defining Multi-Line Variables](https://www.gnu.org/software/make/manual/html_node/Multi_002dLine.html)). The value you specify is installed verbatim; if it contains references to other variables, these references are expanded whenever this variable is substituted (in the course of expanding some other string). When this happens, it is called *recursive expansion*.
>
> 变量的第一种 flavor 是递归展开（Recursive expanded）变量。使用 = 或 define 命令创建的这类变量。你指定的值将会一字不差被设置；如果该变量的值中包含了对其他变量的引用，则在替换该变量时（在其他字符串的展开过程中），这些引用也将会被展开。当上述操作发生时，这就被称为递归展开 recursive expansion。
>
> This flavor of variable is the only sort supported by most other versions of make. 
>
> 对于 make 的大多数版本，recursive expanded variable 是其唯一支持的一类变量。

个人觉得就是 Recursive expanded 告诉 make 把值中的 $(...) 视为变量 展开。

 Recursive expanded 的一个缺点是，无法在变量值的尾部进行添加，因为会导致无限递归错误。

>Another disadvantage is that any functions (see [Functions for Transforming Text](https://www.gnu.org/software/make/manual/html_node/Functions.html)) referenced in the definition will be executed every time the variable is expanded. This makes `make` run slower; worse, it causes the `wildcard` and `shell` functions to give unpredictable results because you cannot easily control when they are called, or even how many times.
>
>另一个缺点是，recursively expanded variable 其定义中引用的任何函数，每次在该变量展开时，都会执行该被引用的函数。这也就让 make 运行得更慢了；更糟糕的是，由于递归展开的特性，它会造成定义中的 wildcard 和 shell 函数给出预期外的结果，因为你无法简单地控制什么时候它们被调用，甚至不知道会被调用多少次。

#### Simply Expanded

简单展开

https://www.gnu.org/software/make/manual/html_node/Simple-Assignment.html

> To avoid the problems and inconveniences of recursively expanded variables, there is another flavor: simply expanded variables.
>
> 为了避免递归展开变量的问题与不便，这里有另一种 flavor：简单展开 simply expanded 变量。
>
> *Simply expanded variables* are defined by lines using ‘:=’ or ‘::=’ (see [Setting Variables](https://www.gnu.org/software/make/manual/html_node/Setting.html)). Both forms are equivalent in GNU `make`; however only the ‘::=’ form is described by the POSIX standard (support for ‘::=’ is added to the POSIX standard for POSIX Issue 8).
>
> 简单展开变量由 := 或 ::= 定义。这两种形式在 GNU make 中是等价的。但是 POSIX 标准中只描述了 ::= 形式（在 POSIX Issue 8 中添加了对 ::= 的支持）。
>
> The value of a simply expanded variable is scanned once, expanding any references to other variables and functions, when the variable is defined. Once that expansion is complete the value of the variable is never expanded again: when the variable is used the value is copied verbatim as the expansion. If the value contained variable references the result of the expansion will contain their values *as of the time this variable was defined*.
>
> simply expanded variable 的值是只被扫描一次的，在变量被定义时，即展开其对其他任何变量、函数的引用。simply expanded variable 一旦展开完成，则变量的值不会再改变：当该 simply expanded variable 被使用时，其值将会被逐字拷贝作为其展开。如果 simply expanded variable 的值包含变量引用，则其展开结果将包含的是这些被引用的变量在该 simply expanded variable 定义时的值。

通过 Simply expanded variables，我们即可对一个变量基于其原本的值进行重新定义。

例如：

~~~Makefile
# Recursive variable. This will print "later" below
# one 使用 := 赋值，flavor 为 Recursive expanded，当执行到下面的 echo $(one) 时，对 $(one) 进行展开，而 $(one) 的值中包含其他变量 $(later_variable) 的引用，因此此时也会对 $(later_variable)进行展开，最终得到 one later
one = one ${later_variable}
# Simply expanded variable. This will not print "later" below
# two 使用 := 赋值，flavor 为 Simply expended variable，在定义时就对其及其引用变量、函数进行展开，而此时 later_variable 此时尚未定义，${later_variable} 展开为空，因此 two 的值为 two
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

#### Immediately Expanded Variable Assignment

立即展开变量指派

> Another form of assignment allows for immediate expansion, but unlike simple assignment the resulting variable is recursive: it will be re-expanded again on every use. In order to avoid unexpected results, after the value is immediately expanded it will automatically be quoted: all instances of $ in the value after expansion will be converted into $$. This type of assignment uses the :::= operator. 
>
> 立即展开的另一种形式，但同于简单指派，最终的变量是 recursive 递归的：它每次在使用时才展开。为了避免预期外的结果，在其值立即展开后，下述操作将自动被执行：将值中的所有 $ 都将被转换为 $$。这类指派使用 :::= 操作符。
>
> 例如，OUT 变量的结果为 first
>
> ~~~Makefile
> var = first
> OUT :::= $(var)
> var = second
> ~~~
>
> 而
>
> ~~~Makefile
> var = one$$two
> OUT :::= $(var)
> var = three$$four
> ~~~
>
> OUT 变量的结果为 one$$two。当 OUT 被指派时，其值被展开，因此其展开结果为 one$two（$$ 转义字符，实则为 $）；随后，该值将会被指派完成前重新转义（将值中的所有 $ 都将被转换为 $$），给出最终的结果为 one$$two
>
> The variable `OUT` is thereafter considered a recursive variable, so it will be re-expanded when it is used.
>
> 此后该变量 OUT 被视为递归变量，因此每当它被使用时，会重新展开
>
> This seems functionally equivalent to the ‘:=’ / ‘::=’ operators, but there are a few differences:
>
> 看起来在功能上等同于 := 或 ::=，但仍有一些不同：
>
> First, after assignment the variable is a normal recursive variable; when you append to it with ‘+=’ the value on the right-hand side is not expanded immediately. If you prefer the ‘+=’ operator to expand the right-hand side immediately you should use the ‘:=’ / ‘::=’ assignment instead.
>
> 首先，在指派后，变量是普通的递归变量；当你使用 += 在其值得右边添加值时，不会立即展开。如果你想要立即展开右手边的值，应当使用 := 或 ::=。
>
> Second, these variables are slightly less efficient than simply expanded variables since they do need to be re-expanded when they are used, rather than merely copied. However since all variable references are escaped this expansion simply un-escapes the value, it won’t expand any variables or run any functions.
>
> 其次，这类变量的效率要低于 simply expanded 变量，因为它们需要在被使用时重新展开，而不仅仅是复制。但由于所有变量引用被转义，这个展开只是简单的反转义其值，它并不会展开任何变量或运行任何函数。
>
> ~~~Makefile
> var = one$$two
> OUT :::= $(var)
> OUT += $(var)
> var = three$$four
> ~~~
>
> After this, the value of `OUT` is the text ‘one$$two $(var)’. When this variable is used it will be expanded and the result will be ‘one$two three$four’.
>
> OUT 的值为 one$$two $(var)，当 OUT 被使用时，它将展开且其展开结果为 one$two three$four。

个人理解，Immediately Expanded Variable Assignment 是在赋值时进行一次立即展开，随后就是 recursive expanded 的属性。之所以立即展开后要替换 $ 为 $$，是因为转义，替换才能保持原义（此处为 $ 字符），不替换的话含义就变了（此处为 $(...)的值）。

#### Conditional Variable Assignment

https://www.gnu.org/software/make/manual/html_node/Conditional-Assignment.html

条件变量指派

> There is another assignment operator for variables, ‘?=’. This is called a conditional variable assignment operator, because it only has an effect if the variable is not yet defined. 
>
> 变量的另一种指派操作符为 ?=，其称为 条件变量指派，因为其仅在变量未定义的情况下生效：
>
> ~~~Makefile
> FOO ?= bar
> ~~~
>
> 等价于：
>
> ~~~Makefile
> ifeq ($(origin FOO), undefined)
>   FOO = bar
> endif
> ~~~

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

#### 小结

GNU make 有两种 flavors：

- recursive expanded
- simply expanded

四种指派方式：

- Recursively Expanded Variable Assignment
- Simply Expanded Variable Assignment
- Immediately Expanded Variable Assignment
- Conditional Variable Assignment

#### 未定义的变量，其值为空

~~~Makefile
all: 
	# Undefined variables are just empty strings!
	echo $(nowhere)
~~~

#### 空格

前导空白会被丢弃，但后导空白不会被丢弃。
如果想要一个变量为空白字符，以下方法很实用：则需使用 $(nullstring)

https://www.gnu.org/software/make/manual/html_node/Simple-Assignment.html

> Leading whitespace characters are discarded from your input before substitution of variable references and function calls; this means you can include leading spaces in a variable value by protecting them with variable references, like this:
>
> 在替换变量引用和函数调用前，前导空白字符会被丢弃。因此你可以在变量的值中使用使用变量引用来保护前导空格使得其能包含进变量的值中

~~~Makefile
# 这里使用 "" 双引号中的内容表示变量的值（不含双引号）
# $(with_spaces) 的值为 "hello   "，注意包括了最后 3 个空白，但 = 等号与 h 之间的前导空格是忽略的。
with_spaces = hello   # with_spaces has many spaces after "hello"
# 因此 $(after) 的值为 "hello   there"
after = $(with_spaces)there

nullstring =
# 展开 space 前，" $(nullstring) " 前导空格被舍去，由于存在变量 $(nullstring) 因此其后的空格不会被舍去，
# 展开 space 时，是对 "$(nullstring) " 进行展开，又因 $(nullstring) 其值为空，因此使得 space 的值为一个空格
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

#### 使用 += 可给直接变量添加内容：

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

### Command line arguments and override

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

### List of commands and define

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

### Target-specific variables

指定目标的变量

可为指定 targets 设置变量，override 这类关键字也可使用：

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

### Pattern-specific variables

指定模式的变量

可为指定模式设置变量，override 这类关键字也可使用：

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