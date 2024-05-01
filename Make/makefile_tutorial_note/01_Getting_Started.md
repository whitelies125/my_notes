[TOC]

## Getting_Started

#### Makefile Syntax

Makefile 包含了一系列 rule 规则。一个常见 rule 如下：

~~~Makefile
targets: prerequisites
	command
	command
	command
~~~

targets：文件名，以空格分隔。一个 rule 中 targets 通常只有一个文件名

commands：一系列用于制作 targets 的步骤。需以 TAB 开头而不能是空格。

prerequisites：也是文件名，以空格分隔。在执行 commands 前，这些文件需已存在。同时也可称之为依赖（dependencies）

#### The essence of Make

如果没有给 make 传递任何参数，则默认执行第一个 target。

对于所有 targets ，如果其文件不存在，或 prerequisites 中有文件 A 不存在，或文件 A 的修改时间晚于它，则对文件 A 进行同样的检查与操作，随后执行它的 commands。
这个检查和操作是递归的。

~~~C++
// 伪代码逻辑：
// 假定 targets 只写了一个文件
void make(target){
	for(auto& file : target.prerequisites){
        if(target.noExist() || file.noExist() || target.data() < file.date()){
            make(file);
			for(auto& cmd : target.command){
            execute(cmd);
        }
    }
}
~~~

#### More quick examples

~~~Makefile
some_file: other_file
	echo "This will always run, and runs second"
	touch some_file

other_file:
	echo "This will always run, and runs first"
~~~

如上例，第一次执行 make 时

1. 开始
2. some_file 作为默认目标
3. some_file 文件不存在，对 other_file 进行相同递归检查与操作
4. other_file 文件不存在，other_file 已没有 prerequisites，执行 other_file 的 commands
5. 执行 some_file 的 commands
6. 结束

注意到，some_file 的 commands 是会创建 some_file 文件的，而 other_file 的 commands 并没有创建 other_file 文件，因此第二次执行 make 时 other_file、some_file 的 commands 总是会执行：

1. 开始
2. some_file 作为默认目标
3. some_file 文件存在，但 other_file 不存在，对 other_file 进行相同递归检查与操作
4. other_file 文件不存在，other_file 已没有 prerequisites，执行 other_file 的 commands
5. some_file 没有其他 prerequisites，执行 some_file 的 commands
6. 结束

从这个例子注意到一点：make 并不会自动地帮我们创建文件，实际上，make 只是在检查文件是否存在以及时间关系，满足条件则执行相应的 commands，文件是需要在 commands 中我们自己写命令创建的。

当然，也存在有些  targets 本身只是用于执行一些 commands 而不创建文件的。如下例中的 clean 以及后续会讲到到的 .PHONY 伪目标。

#### Make clean

clean 是常用于删除其他 target 的输出。

~~~Makefile
some_file: 
	touch some_file

clean:
	rm -f some_file
~~~

若直接执行 make，则 some_file 则会作为默认目标，因此需执行 make clean，指明将 clean 作为目标。

注意，若不将 clean 指明为 .PHONY 伪目标，则 make 认为其是一个正常的 targets，因此如果当前目录下恰好存在有名为 clean 的文件，则 make clean 不会执行 commands：

1. 开始
2. clean 作为目标
3. clean 文件已存在，other_file 已没有 prerequisites，不执行 other_file 的 commands
4. 结束

#### Variables

Makefile 的变量只有字符串。

Makefile 中符合正常直觉的变量赋值其实是 :=，而不是 =，这一点在后续变量的一节中讲述。

~~~Makefile
files := file1 file2
some_file: $(files)
	echo "Look at this variable: " $(files)
	touch some_file

file1:
	touch file1
file2:
	touch file2

clean:
	rm -f file1 file2 some_file
~~~

使用 $(变量名) 或 ${变量名} 来展开变量（虽然直接使用 $变量名 也行，但不推荐，容易出错）。

上述示例中，files 的值为 file1 file2，是会忽略了 := 与 file1 之间的前导空格的。

~~~Makefile
a := one two # a is set to the string "one two"
b := 'one two' # Not recommended. b is set to the string "'one two'"
all:
	printf '$a'
	printf $b
~~~

注意，Makefile 中的引号，无论单双引号，都是没有特殊含义的，都当作通常的字符。而非像其他编程语言，引号内的才是字符。

如以上示例，a 的值是 one two，而 b 的值是 'one two'，是带单引号的。

执行的两条 commands 最终是一样的

~~~shell
printf 'one two'
~~~

2024.05.01