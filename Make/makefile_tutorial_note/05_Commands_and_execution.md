[TOC]

## Commands and Execution

#### Command Echoing/Silencing

在 commands 前添加 @ 使该 command 不打印出来

~~~Makefile
all: 
	@echo "This make line will not be printed"
	echo "But this will"
~~~

shell：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
This make line will not be printed
echo "But this will"
But this will
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### Command Execution

**每一行 command 各自在一个新的 shell 中运行**（或者说效果上等同于每行 command 各自单独在一个 shell 中运行）。

~~~Makefile
all: 
	cd ..
	# The cd above does not affect this line, because each command is effectively run in a new shell
	echo `pwd`

	# This cd command affects the next because they are on the same line
	cd ..;echo `pwd`

	# Same as above
	cd ..; \
	echo `pwd`
~~~

如上例中，第一个 echo 'pwd' 与 cmd 不在同一行，所以 echo 'pwd' 仍然输出的是当前目录，而后两个则在同一行，因此输出的是上一级目录。

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
cd ..
# The cd above does not affect this line, because each command is effectively run in a new shell
echo `pwd`
/home/whitelies125/code/makefile
# This cd command affects the next because they are on the same line
cd ..;echo `pwd`
/home/whitelies125/code
# Same as above
cd ..; \
echo `pwd`
/home/whitelies125/code
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ nvim
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### Default Shell

默认 shell 是 /bin/sh，可通过修改变量 SHELL 来修改 shell：

~~~Makefile
SHELL=/bin/bash

cool:
	echo "Hello from bash"
~~~

#### Default dollar sign

commands 中使用字符 $，需使用 $$ 表示。

毕竟 $ 被 make 用来表示变量了嘛，所以得转义一下，很好理解。

~~~Makefile
make_var = I am a make variable
all:
	# Same as running "sh_var='I am a shell variable'; echo $sh_var" in the shell
	sh_var='I am a shell variable'; echo $$sh_var

	# Same as running "echo I am a make variable" in the shell
	echo $(make_var)
~~~

#### Error handing with -k，-i and -

根据 https://www.gnu.org/software/make/manual/html_node/Errors.html

>After each shell invocation returns, `make` looks at its exit status. If the shell completed successfully (the exit status is zero), the next line in the recipe is executed in a new shell; after the last line is finished, the rule is finished.
>
>在每次 shell 调用返回后，make 查看其退出状态。如果 shell 成功完成（退出状态为 0），则在新 shell 中执行 commands 中的下一行；最后一行 command 执行完成了，rule 就执行完成了。
>
>If there is an error (the exit status is nonzero), `make` gives up on the current rule, and perhaps on all rules.
>
>如果出现错误（退出状态非零），则make放弃当前 rule，并且也可能是放弃所有 rule。
>
>Sometimes the failure of a certain recipe line does not indicate a problem. For example, you may use the `mkdir` command to ensure that a directory exists. If the directory already exists, `mkdir` will report an error, but you probably want `make` to continue regardless.
>
>有时，某行 command 的失败并不意味着真的是个问题。例如，你可能使用 mkdir 命令来确保目录存在。而在目录已经存在的情况下，mkdir 将报告一个错误，但你可能是希望 make 继续执行下去。
>
>To ignore errors in a recipe line, write a ‘-’ at the beginning of the line’s text (after the initial tab). The ‘-’ is discarded before the line is passed to the shell for execution.
>
>要忽略配  command 返回的错误，请在该行文本的开头（在初始制表符之后）添上 -。在将 command 传递给 shell 执行之前，- 是会被丢弃，即 - 不会传递给 shell。
>
>For example,
>
>```
>clean:
>    -rm -f *.o
>```
>
>This causes `make` to continue even if `rm` is unable to remove a file.
>
>例如：这将使得 make 继续执行，即便 rm 命令并未能够移除文件。
>
>When you run `make` with the ‘-i’ or ‘--ignore-errors’ flag, errors are ignored in all recipes of all rules. A rule in the makefile for the special target `.IGNORE` has the same effect, if there are no prerequisites. This is less flexible but sometimes useful.
>
>当使用 -i 或 --ignore-errors 标志运行 make 时，所有 rules 的所有 command 中的错误都会被忽略。如果makefile 中针对特殊目标. IGNORE 的规则没有 prerequisites，则对该 rule 具有相同的效果。这种方法不太灵活，但有时很有用。
>
>When errors are to be ignored, because of either a ‘-’ or the ‘-i’ flag, `make` treats an error return just like success, except that it prints out a message that tells you the status code the shell exited with, and says that the error has been ignored.
>
>当错误因 - 或 -i 标识而被忽略时，make 会打印出 shell 退出时的状态码并表示错误已被忽略，除此之外像处理成功一样处理本次错误返回。
>
>When an error happens that `make` has not been told to ignore, it implies that the current target cannot be correctly remade, and neither can any other that depends on it either directly or indirectly. No further recipes will be executed for these targets, since their preconditions have not been achieved.
>
>当错误发生时，且 make 没有被告知忽略（无 -，-i 等忽略错误的标识），则意味着当前目标不能被正确地重新构建，其他直接或间接依赖于它的 targets 自然也不能重新构建。由于这些依赖于它的 targets 的 prerequisites 未完成，因此将不再执行这些 targets 的方案。
>
>Normally `make` gives up immediately in this circumstance, returning a nonzero status. However, if the ‘-k’ or ‘--keep-going’ flag is specified, `make` continues to consider the other prerequisites of the pending targets, remaking them if necessary, before it gives up and returns nonzero status. For example, after an error in compiling one object file, ‘make -k’ will continue compiling other object files even though it already knows that linking them will be impossible. See [Summary of Options](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html).
>
>通常在这种情况下 make 立即放弃，返回非零状态。但是，如果指明了 -k 或 --keep-going 标识，则在放弃并返回非零状态之前，make 将继续待处理 targets 的其他 prerequisites，必要时重新构建它们。例如，在编译一个目标文件出错后，make -k 将继续编译其他目标文件，即使它已经知道链接它们是不可能的。参见 Summary of Options。
>
>The usual behavior assumes that your purpose is to get the specified targets up to date; once `make` learns that this is impossible, it might as well report the failure immediately. The ‘-k’ option says that the real purpose is to test as many of the changes made in the program as possible, perhaps to find several independent problems so that you can correct them all before the next attempt to compile. This is why Emacs’ `compile` command passes the ‘-k’ flag by default.
>
>通常的行为假设您的目的是使得指定 targets 为最新；一旦 make 发觉这是不可能的做到的，它立即上报失败。-k 选项表示，真正的目的是测试程序中尽可能多的更改，也可能是为了找到几个独立的问题，以便你在下次尝试编译之前将它们全部纠正。这就是 Emacs 编译命令默认传递 -k 标志的原因。

make -k：https://www.gnu.org/software/make/manual/html_node/Testing.html

总结：

make -k：即便执行 make 中途遇到错误，也不中断，而是继续执行后续 command。

make -i：等同于在所有 command 行前添加 -。

添加 -：若该 command 行出现错误，上报错误但是忽略，继续执行下一行 command。

#### Recursive use of make

参考官方文档：https://www.gnu.org/software/make/manual/html_node/Recursion.html

使用 $(MAKE) 来递归地调用 makefile，而不是使用 make。因为 $(MAKE) 会传递构建的 flags。

~~~Makefile
new_contents = "hello:\n\ttouch inside_file"
all:
	mkdir -p subdir
	printf $(new_contents) | sed -e 's/^ //' > subdir/makefile
	cd subdir && $(MAKE)

clean:
	rm -rf subdir
~~~

> Recursive use of `make` means using `make` as a command in a makefile. This technique is useful when you want separate makefiles for various subsystems that compose a larger system. For example, suppose you have a sub-directory subdir which has its own makefile, and you would like the containing directory’s makefile to run `make` on the sub-directory. You can do it by writing this:
>
> 递归地使用 make 意指在 makefile 中使用 make 作为一个 commnad。当为由多个子系统组成的大型系统分离 makefiles 时，这个技术相当有用。例如，假定你有一个子目录，其有着自己的 makefile，你想要在其上级目录的 makefile 中执行子目录的 make，你可以这样写：
>
> ~~~Makefile
> subsystem:
> 	cd subdir && $(MAKE)
> ~~~
>
> or, equivalently, this (see [Summary of Options](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html)):
>
> 或等价于（详见 [Summary of Options](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html)):：
>
> ~~~Makefile
> subsystem:
> 	$(MAKE) -C subdir
> ~~~

个人测试：

~~~Makefile
all:
	echo $(MAKE)
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
echo make
make
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make all
echo make
make
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

可见，$(MAKE) 包含了传递给 make 的参数。而直接使用 make 显然是不带这些参数的。

#### Export，environments，and recursive make

导出，环境，和递归 make

当 make 开始执行时，它自动地从它被执行时所设置的环境变量创建出其外的属于 Make 的变量。

（结合下面的例子，这里的意思是说 make 自己添加了一份与环境变量相同名字、相同值的拷贝作为自己程序内的变量。
该教程里使用的是 Make 变量这个词，其实也就是指通常 Makefile 里定义变量 var := 定义出来的变量。
这也就是为什么下例中直接使用 Makefile 中没有定义的 shell_env_var 也能输出环境变量相同的值。）

~~~Makefile
# Run this with "export shell_env_var='I am an environment variable'; make"
all:
	# Print out the Shell variable
	# 这里输出的是环境变量，因为这个 command 实际上是在 shell 中执行 "echo $shell_env_var"，因此是输出的环境变量
	echo $$shell_env_var


	# Print out the Make variable
    # 这里输出的是 make 根据环境变量拷贝了一份具有相同名字、相同值的自己的变量
    # 因此，这里直接 Makefile 中明明没有定义的 $(shell_env_var) 也是正确的值，而非空值。
	echo $(shell_env_var)
~~~

export 指令携带了一个变量，并且将该变量设为所有的 commands 中 shell command 的环境变量。

~~~Makefile
shell_env_var=Shell env var, created inside of Make
export shell_env_var
# 或者直接
# export shell_env_var=Shell env var, created inside of Make
all:
	echo $(shell_env_var) # 输出 make 自己的变量
	echo $$shell_env_var # 输出环境变量，因为 export 导出了 shell_env_var 到环境变量中，因此没问题
~~~

如此一来，当 make 执行过程中又执行 make 命令时，你可以使用 export 指令使得变量可被 子 make 命令访问。在该例中，cooly 被导出所以 subdir 中的 makefile 仍可以使用 cooly：
（子 make 启动时也会把启动时的环境变量拷一份嘛。）

~~~Makefile
new_contents = "hello:\n\techo \$$(cooly)"

all:
	mkdir -p subdir
	printf $(new_contents) | sed -e 's/^ //' > subdir/makefile
	@echo "---MAKEFILE CONTENTS---"
	@cd subdir && cat makefile
	@echo "---END MAKEFILE CONTENTS---"
	cd subdir && $(MAKE)

# Note that variables and exports. They are set/affected globally.
cooly = "The subdirectory can see me!"
export cooly
# This would nullify the line above: unexport cooly

clean:
	rm -rf subdir
~~~

你也可以 export 变量使得它们能够在 shell 中使用

~~~Makefile
one=this will only work locally
export two=we can run subcommands with this

all: 
	@echo $(one)
	@echo $$one
	@echo $(two)
	@echo $$two
~~~

因为 $(one)，$(two) 是 make 的变量，因此在传递给 shell 执行之前，就会被替换为变量的值，shell 接收到并执行的命令实为 @echo this will only work locally，@echo we can run subcommands with this。

而 $$one，$$two 则是传递给 shell 执行的命令是 @echo $one，@echo $two，此处是输出环境变量的 one two，而环境变量中并未定义 one two，且 make 只导出的 two 到环境变量，因此结果为：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
this will only work locally

we can run subcommands with this
we can run subcommands with this
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

.EXPORT_ALL_VARIABLES 可导出所有的变量：

~~~Makefile
.EXPORT_ALL_VARIABLES:
new_contents = "hello:\n\techo \$$(cooly)"

cooly = "The subdirectory can see me!"
# This would nullify the line above: unexport cooly

all:
	mkdir -p subdir
	printf $(new_contents) | sed -e 's/^ //' > subdir/makefile
	@echo "---MAKEFILE CONTENTS---"
	@cd subdir && cat makefile
	@echo "---END MAKEFILE CONTENTS---"
	cd subdir && $(MAKE)

clean:
	rm -rf subdir
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
mkdir -p subdir
printf "hello:\n\techo \$(cooly)" | sed -e 's/^ //' > subdir/makefile
---MAKEFILE CONTENTS---
hello:
        echo $(cooly)---END MAKEFILE CONTENTS---
cd subdir && make
make[1]: Entering directory '/home/whitelies125/code/makefile/subdir'
echo "The subdirectory can see me!"
The subdirectory can see me!
make[1]: Leaving directory '/home/whitelies125/code/makefile/subdir'
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

#### Arguments to make

所有选项参见：https://www.gnu.org/software/make/manual/html_node/Options-Summary.html

可以一次传递多个 targets 给 make，如 make clean run test，会先后执行 clean、然后是 run，最后是 test。

其他可以看一下的选项：

make -n：打印出 make 要执行的 commands，但只打印并不真的执行。

make -t：将文件标记为最新，但只标记为最新，实际上并不重新构建。

make -o file：不重新构建 file，即便 file 比它的 prerequisite 要老也不要重新构建。

### 个人补充

#### linux 中的 export

根据网络自己整理：

用户登录到 linux 系统后，系统将启动一个用户 shell。

执行 shell 脚本时，实际上是创建一个子 shell，由子 shell 来执行该 shell 脚本，完成后返回结果，子 shell 终止，回到父 shell。

注意，子 shell 在创建时，只会继承（或称为拷贝）父 shell 的环境变量，不会拷贝父 shell 中定义的局部变量（没有 export 导出到环境变量的变量）。

因此，使用 export 将变量添加到环境变量，创建子 shell 时才会拷贝到子 shell 的环境变量中去。子 shell 才可获取到该变量。

例如我写了两个shell 脚本：

~~~shell
# father.sh
var=123

echo $var
echo call child.sh start
./child.sh #执行 child.sh 脚本，将会创建一个新的 shell 来执行 child.sh
echo call child.sh end

# child.sh
echo $var
~~~

执行 ./father.sh 结果为：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ ./father.sh
123
call child.sh start

call child.sh end
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

child.sh 中并没有输出 var 的值 123。

这是因为，一开始，我们处在一个 shell 中。

执行 father.sh 时，新开了一个 shell_f 用来执行 father.sh。此时 father.sh 定义的变量，只在 shell_f 中，属于 shell_f 的局部变量。

执行 child.sh 时，又新开了一个 shell_c，因此 shell_c 中不存在名为 var 的变量，自然 child_sh 在 shell_c 中执行 echo $var 时输出为空。

而使用 export 命令对 shell 中的变量进行导出，使得系统在创建新 shell 时，会对 export 导出的变量创建在新 shell 中创建一份拷贝。这样子 shell 中才能获取到父 shell 中的变量。

因此，father.sh 需添加 export 导出 var：

~~~shell
# father.sh
export var=123
# 或：
# var=123
# export var

echo $var
echo call child.sh start
./child.sh
echo call child.sh end

# child.sh
echo $var
~~~

另，在前文 Command Execution 一节中有写到：

> **每一行 command 各自在一个新的 shell 中运行**（或者说效果上等同于每行 command 各自单独在一个 shell 中运行）。

shell 中有环境变量 $，其值为该 shell 的 pid（每个程序唯一）。
可通过下述 Makefile 验证不同行是不同shell，同行是同一个shell：

~~~Makefile
all:
	echo $$$$
	echo $$$$; echo $$$$; echo $$$$
	echo $$$$; echo $$$$; echo $$$$
~~~

shell 结果：

~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$ make
echo $$
204954
echo $$; echo $$; echo $$
204955
204955
204955
echo $$; echo $$; echo $$
204956
204956
204956
whitelies125@DESKTOP-H47PT8Q:~/code/makefile$
~~~

2024.05.02