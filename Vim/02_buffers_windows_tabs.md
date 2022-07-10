# buffers windows tabs

## buffers

每个文件对应一个 buffer。
当打开多个文件，就有多个 buffer。

buffer 也就是该文件对应的内存空间。

例如：

~~~C++
vim test.cpp test_1.cpp
~~~

就打开了两个文件，这两个文件有各自的 buffer。

#### :buffers 查看 buffer

~~~C++
:buffers
:ls
:files
~~~

### 遍历 buffer

#### :bnext 切换到后一个 buffer

~~~C++
:bnext
~~~

#### :bprevious 切换到前一个 buffer

~~~C++
:bprevious
~~~

#### :buffer test.cpp 切换到该文件 buffer

~~~C++
:buffer test.cpp
~~~

使用 tab 自动补全。

#### :buffer 2 切换到编号为 2 的 buffer

~~~C++
:buffer 2
~~~

#### ctrl+^  跳转至先前编辑过的 buffer

~~~C++
ctrl+^//实际我按的是 ctrl+6，没按 shift，试了一下按不按都一样。
~~~

#### ctrl+O 跳转到列表中旧的位置，ctrl+I 跳转到列表中新的位置

~~~C++
ctrl+O//实际上我按的是 ctrl+o，没按大写。
ctrl+I//同上，ctrl+i,没按大写。
~~~

但这 ctrl+O 和 ctrl+I 并不是属于 buffer 的独有方法。

### 退出 vim

#### :qall 关闭所有 buffers，退出 vim

~~~C++
:qall
:quitall
~~~

#### :wqall 保存并关闭所有 buffers，退出 vim

~~~C++
:wqall
~~~

#### :qall! 关闭所有 buffers 但不保存，退出 vim

~~~C++
:qall!
:quitall!
~~~

## windows

我们通过 window 查看文件的 buffer。

例如，在前面切换 buffer 的操作中，我们通过一个 window 显示不同 buffer。

#### :split test_1.cpp 新建水平分割的 window 查看 test_1.cpp

~~~C++
:split test_1.cpp//此时有两个窗口，分别展示 test.cpp test_1.cpp
~~~

#### :vsplit test.c 新建垂直分割 window 查看 test.c

~~~C++
:split test.c//此时有三个窗口，分别展示 test.cpp test_1.cpp test.c
~~~

#### :new filename 创建一个新窗口并打开 filename 文件

~~~C++
:new//创建一个新窗口
:new filename//创建一个新窗口并打开 filename 文件
~~~

### ctrl+W

#### ctrl+W H/J/K/L 移动光标到左/下/上/右窗口

~~~c++
ctrl+W H//按 ctrl+W 再按 H，移动光标到左边窗口
ctrl+W J//移动光标到下边窗口
ctrl+W K//移动光标到上边窗口
ctrl+W L//移动光标到右边窗口
~~~

#### ctrl+W C 或 :quit 关闭窗口

在对应窗口输入

~~~C++
ctrl+W C
:quit
~~~

则退出该窗口。

#### ctrl+W S/V/C/O

~~~c++
ctrl+W V//打开一个新的垂直分割窗口
ctrl+W S//打开一个新的水平分割窗口
ctrl+W C//关闭该窗口
ctrl+W O//改变该窗口外其它所有窗口
~~~

## tabs

#### :tabnew test_1.cpp 在新的 tab 打开 test.1.cpp

~~~C++
:tabnew test_1.cpp
~~~

#### :tab...

~~~C++
:tabnew file.txt    //在tab中打开一个文件
:tabnext            //切换至下一个tab
:tabprevious        //切换至前一个tab
:tablast            //切换至最后一个tab
:tabfirst           //切换至第一个tab
:tabclose           //关闭当前tab
~~~

#### gt 切换到后一个tab,gT 切换到前一个 tab

~~~C++
gt
gT
~~~

#### vim -p test.cpp test_1.cpp 以多个 tab 方式打开文件

~~~C++
vim -p test.cpp test_1.cpp//打开后 tab1 为 test.cpp,tab2 为 test_1.cpp
~~~

