[TOC]

## Other Features

#### Include Makefiles

参考：https://www.gnu.org/software/make/manual/html_node/Include.html

include 指令告诉 make 去读其他 makefile，

~~~Makefile
include filenames...
~~~

例如：

如果你有三个 .mk 文件，a.mk、b.mk 和 c.mk，并且 $(bar) 展开为 bish bash，则以下表达式等价

~~~Makefile
include foo.make *.mk $(bar)
# 等价于
# include foo.make a.mk b.mk c.mk e.mk f.mk  
~~~

类似于 C/C++ 的 include，原封不动地把 include 的文件拷贝过来。

#### The vpath Directive

使用 vpath 来指明一些 prerequisites 在哪里。

格式为

~~~Makefile
vpath <pattern> <directories, space/colon separated>
~~~

其中 <pattern>  可以包含 %，其将匹配零个或多个字符。

含义为，对于符合 pattern 的文件，去以空格或冒号分隔的 directories 目录下寻找。

也可以使用环境变量 VPATH 进行全局操作。

辨别：VPATH 是环境变量，而 vpath 是关键字。

~~~Makefile
# 对于 %.h 的文件，从 ../headers ../other-directory 目录中寻找
vpath %.h ../headers ../other-directory

# 尽管 some_binary 依赖的 blah.h 在当前目录并不存在，但可在 vpath 中对 %.h 文件指出的寻找目录中找到
# Note: vpath allows blah.h to be found even though blah.h is never in the current directory
some_binary: ../headers blah.h
	touch some_binary

../headers:
	mkdir ../headers

# We call the target blah.h instead of ../headers/blah.h, because that's the prereq that some_binary is looking for
# Typically, blah.h would already exist and you wouldn't need this.
blah.h:
	touch ../headers/blah.h

clean:
	rm -rf ../headers
	rm -f some_binary

~~~

shell 结果：

~~~Makefile
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
mkdir ../headers
touch ../headers/blah.h
touch some_binary
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### Multiline

使用反斜线 \ 进行多行编辑

~~~Makefile
some_file: 
	echo This line is too long, so \
		it is broken up into multiple lines
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
echo This line is too long, so \
it is broken up into multiple lines
This line is too long, so it is broken up into multiple lines
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### .phony

使用 .PHONY 防止 make 将伪目标与文件名混淆。如下例，即便在当前目录创建了一个名为  clean  的文件，但执行 make clean 时，clean 目标仍然会执行。

这是因为 .PHONY 告知了 make ，clean 是伪目标，而非文件名。因此 make 并不会按照普通 target 的逻辑：因为当前目录已存在一个最新的名为 clean 的文件，就不执行 commands。

~~~Makefile
some_file:
	touch some_file
	touch clean

.PHONY: clean
clean:
	rm -f some_file
	rm -f clean
~~~

#### .delete_on_error

参考：https://www.gnu.org/software/make/manual/html_node/Special-Targets.html

只要 .DELETE_ON_ERROR 在 Makefile 中出现，则当 rule 的 target 已被修改且该 rule 的 commands 以非零值退出时，make 将会删除改 rule 的 target。

其将作用于所有的 targets，而不同于 .PHONY 仅作用于指出的 target。

~~~Makefile
.DELETE_ON_ERROR:
all: one two

one:
	touch one
	false
# 尝试了把 .DELETE_ON_ERROR: 放在这里也是一样的效果
two:
	touch two
	false
# 尝试了把 .DELETE_ON_ERROR: 放在末尾也是一样的效果
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
touch one
false
make: *** [Makefile:6: one] Error 1
make: *** Deleting file 'one'
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ ls
Makefile
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

如果把 one 修改为返回零：

~~~Makefile
.DELETE_ON_ERROR:
all: one two

one:
	touch one # 创建了文件 one
	true # 没有返回非零值，因此不删除

two:
	touch two # 创建了文件 two
	false # 由于返回了非零值，且 target 已被修改（新建了 two）因此删除 target
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
touch two
false
make: *** [Makefile:10: two] Error 1
make: *** Deleting file 'two'
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ ls
Makefile  one
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

2024.06.05