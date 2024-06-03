[TOC]

## Function

#### First Functions

函数主要用于文本处理。使用 $(fn, arguments) 或 ${fn, arguments} 的形式来调用函数。

Make 有许多内置函数，参见：https://www.gnu.org/software/make/manual/html_node/Functions.html

举例：

如 subst 函数，$(subst from, to, text)：对 text 中的每一个 from 替换为 to。

~~~Makefile
bar := ${subst not,totally, "I am not superman"}
all:
	@echo $(bar)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
I am totally superman
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

如果想要将空格替换为逗号，则使用变量

~~~Makefile
comma := ,
empty:=
space := $(empty) $(empty) # 使用前面 Variables_part_2 一节的方法将变量赋值为空格
foo := a b c
bar := $(subst $(space),$(comma),$(foo))

all:
	@echo $(bar)
~~~

shell 结果：

~~~Makefile
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
a,b,c
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

**注意**，在第一个参数之后不要再包含空格， 否则会被视为字符串的一部分

~~~Makefile
comma := ,
empty:=
space := $(empty) $(empty)
foo := a b c
bar := $(subst $(space), $(comma) , $(foo)) # 将 " a b c" 中的 " " 替换为 " , "

all: 
	# Output is ", a , b , c". Notice the spaces introduced
	@echo $(bar)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# Output is ", a , b , c". Notice the spaces introduced
, a , b , c
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### String Substitution

$(patsubst pattern,replacement,text)

> "Finds whitespace-separated words in text that match pattern and replaces them with replacement. Here pattern may contain a ‘%’ which acts as a wildcard, matching any number of any characters within a word. If replacement also contains a ‘%’, the ‘%’ is replaced by the text that matched the ‘%’ in pattern. Only the first ‘%’ in the pattern and replacement is treated this way; any subsequent ‘%’ is unchanged." ([GNU docs](https://www.gnu.org/software/make/manual/html_node/Text-Functions.html#Text-Functions))
>
> 在 text 中以空格分隔进行查找匹配 word，若 word 匹配 pattern，则替换 word 为 replacement。
>
> pattern 可能包含 %，其行为则与 wildcard 一致，在 word 中匹配任意数量的字符。
>
> 如果 replacement 也包含 %，则 % 被替换为 pattern 中 % 匹配到的字符。
>
> 只有 pattern 和 replacement 中的第一个 % 以这种方式进行处理，随后的任何 % 则保持不变。

$(text:pattern=replacement) 是其简写形式，等价于 $(patsubst pattern,replacement,text)

$(text:suffix=replacement) 是另一种简写形式，等价于 $(patsubst %suffix,%replacement,text)，用于替换后缀。

~~~Makefile
foo := a.o b.o l.a c.o
# 标准的函数调用形式 $(patsubst pattern,replacement,text)
one := $(patsubst %.o,%.c,$(foo))
# 简写：$(text:pattern=replacement) 形式
# This is a shorthand for the above
two := $(foo:%.o=%.c)
# 简写 $(text:suffix=replacement) 形式，只替换后缀的场景，"%.后缀名" 可省略为 ".后缀名"
# This is the suffix-only shorthand, and is also equivalent to the above.
three := $(foo:.o=.c)

all:
	echo $(one)
	echo $(two)
	echo $(three)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
echo a.c b.c l.a c.c
a.c b.c l.a c.c
echo a.c b.c l.a c.c
a.c b.c l.a c.c
echo a.c b.c l.a c.c
a.c b.c l.a c.c
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### The foreach function

$(foreach var,list,text)，用于将一个空格分隔的单词（word）列表 list 转换为另一个列表。var 表示对 list 进行每次操作时的 word，text 则表示对 var 的展开（进行的操作）。

~~~Makefile
foo := who are you
# 对于 foo 中的每个 word，进行 wrd -> $(wrd)! 的展开。
# 即，每次循环时，变量 wrd 赋值为该次循环的 word，然后进行 text 的展开
# For each "word" in foo, output that same word with an exclamation after
bar := $(foreach wrd,$(foo),$(wrd)!)

all:
	# Output is "who! are! you!"
	@echo $(bar)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# Output is "who! are! you!"
who! are! you!
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### The if function

关于条件判断函数，参见：https://www.gnu.org/software/make/manual/html_node/Conditional-Functions.html

> ```
> $(if condition,then-part[,else-part])
> ```
>
> The `if` function provides support for conditional expansion in a functional context (as opposed to the GNU `make` makefile conditionals such as `ifeq` (see [Syntax of Conditionals](https://www.gnu.org/software/make/manual/html_node/Conditional-Syntax.html))).
>
> The first argument, condition, first has all preceding and trailing whitespace stripped, then is expanded. If it expands to any non-empty string, then the condition is considered to be true. If it expands to an empty string, the condition is considered to be false.
>
> 第一个参数，condition，首先去除所有前导和尾随空白字符，然后进行展开。如果展开结果为任意非空字符串，则结果为 true；如果展开结果为空字符串，则结果为 false。
>
> If the condition is true then the second argument, then-part, is evaluated and this is used as the result of the evaluation of the entire `if` function.
>
> 如果 condition 结果为 true，则求解第二个参数，then-part，并将其结果作为整个 if 函数的求解结果。
>
> If the condition is false then the third argument, else-part, is evaluated and this is the result of the `if` function. If there is no third argument, the `if` function evaluates to nothing (the empty string).
>
> 如果 condition  结果为 false，则求解第三个参数，else-part，并将其结果作为整个 if 函数的求解结果。如果没有第三个参数，则 if 函数的求解结果为空字符串。
>
> Note that only one of the then-part or the else-part will be evaluated, never both. Thus, either can contain side-effects (such as `shell` function calls, etc.)
>
> 注意，then-part 和 else-part 只有一个会被求解，而不会都被求解。因此二者可能包含副作用（例如在 shell 函数中调用）

~~~Makefile
foo := $(if this-is-not-empty,then!,else!) # condition 结果为非空字符串，true，then!
empty :=
bar := $(if $(empty),then!,else!) # $(empty) 结果为空字符串，false，else!

all:
	@echo $(foo)
	@echo $(bar)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
then!
else!
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### The call funciton

Make 支持创建基础函数。你可以通过创建变量的方式来定义一个函数，只需用到参数 $(0)，$(1) 等。

然后你就可以使用内置函数 call 来调用你定义的函数。

语法为 $(call variable,param,param)，其中 $(0) 为该变量名，$(1)，$(2) 等则是传参。

~~~Makefile
sweet_new_fn = Variable Name: $(0) First: $(1) Second: $(2) Empty Variable: $(3)

all:
	# 调用自定义函数 sweet_new_fn，
	# $(0) = sweet_new_fn 为该变量的名字
	# $(1)，$(2) 等则是传参： $(1) = go，$(1) = tigers, $(3) 为空
	# Outputs "Variable Name: sweet_new_fn First: go Second: tigers Empty Variable:"
	@echo $(call sweet_new_fn, go, tigers)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
# 调用自定义函数 sweet_new_fn，
#  = sweet_new_fn 为该变量的名字
# ， 等则是传参：  = go， = tigers,  为空
# Outputs "Variable Name: sweet_new_fn First: go Second: tigers Empty Variable:"
Variable Name: sweet_new_fn First: go Second: tigers Empty Variable:
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### The shell function

shell 函数，调用 shell，但会将终端输出中的换行符替换为空格

~~~Makefile
all: 
    @echo $(shell ls -la) # Very ugly because the newlines are gone!
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ ls -la
total 12
drwxr-xr-x 2 whitelies125 whitelies125 4096 Jun  4 05:42 .
drwxr-xr-x 5 whitelies125 whitelies125 4096 May  1 06:54 ..
-rw-r--r-- 1 whitelies125 whitelies125   73 Jun  4 05:42 Makefile
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
total 12 drwxr-xr-x 2 whitelies125 whitelies125 4096 Jun 4 05:42 . drwxr-xr-x 5 whitelies125 whitelies125 4096 May 1 06:54 .. -rw-r--r-- 1 whitelies125 whitelies125 73 Jun 4 05:42 Makefile
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

注意终端的 ls -la 是有换行的，但 Makefile 中通过 shell 函数执行，则对其返回结果中的换行进行了处理，替换为空格，因此只有一行。

2024.06.04
