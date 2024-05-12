[TOC]

## Conditional Part of Makefiles

#### Conditional if/else

~~~Makefile
foo = ok

all:
ifeq ($(foo), ok)
	echo "foo equals ok"
else
	echo "nope"
endif
~~~

#### Check if a variable is empty

~~~Makefile
nullstring =
foo = $(nullstring) # end of line; there is a space here

all:
ifeq ($(strip $(foo)),)
	echo "foo is empty after being stripped"
endif
ifeq ($(nullstring),)
	echo "nullstring doesn't even have spaces"
endif
~~~

#### Check if a variable is defined

ifdef 只查看变量是否已定义，并不展开变量。

~~~Makefile
bar =
foo = $(bar)

all:
ifdef foo
	echo "foo is defined"
endif
ifndef bar
	echo "but bar is not"
endif
~~~

#### $(MAKEFLAGS)

下例展示了如何使用 findstring 和 MAKEFLAGS 来测试 make flags。使用 make -i 来观察其输出

~~~Makefile
all:
# MAKEFLAGS 由make自动设置，包含 make 接收到的标志字母。例如，执行 make -ik 或 make -k -s 或者 make --ignore-errors --keep-going，那么 MAKEFLAGS 的值都为 ik。
# 注意，MAKEFLAGS 的值是没有短横线 -，并且只有对应选项的单字母的）
	echo $(MAKEFLAGS)
# Search for the "-i" flag. MAKEFLAGS is just a list of single characters, one per flag. So look for "i" in this case.
# MAKEFLAGS 是单个字符的列表，每个字符表示一个flag
ifneq (,$(findstring i, $(MAKEFLAGS)))
	echo "i was passed to MAKEFLAGS"
endif
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
echo

whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make -i
echo i
i
echo "i was passed to MAKEFLAGS"
i was passed to MAKEFLAGS
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make -ik
echo ik
ik
echo "i was passed to MAKEFLAGS"
i was passed to MAKEFLAGS
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make --ignore-errors --keep-going
echo ik
ik
echo "i was passed to MAKEFLAGS"
i was passed to MAKEFLAGS
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

2024.05.12