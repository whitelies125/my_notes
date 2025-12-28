# lua 环境-windows

从官网下载源文件压缩包。

解压后，命令行切换到 Makefile 所在目录

~~~shell
E:\lua\lua-5.4.4>dir
 驱动器 E 中的卷是 DATA
 卷的序列号是 3455-1790

 E:\lua\lua-5.4.4 的目录

2022/11/18  00:23    <DIR>          .
2022/11/18  00:23    <DIR>          ..
2022/11/18  00:23    <DIR>          doc
2021/11/05  00:25             3,146 Makefile
2022/01/14  00:15               151 README
2022/11/18  00:36    <DIR>          src
               2 个文件          3,297 字节
               4 个目录 276,264,620,032 可用字节

E:\lua\lua-5.4.4>
~~~

执行

~~~shell
make mingw
~~~

顺利编译后，在 src 目录中可找到编译出来的 lua.exe

~~~shell
E:\lua\lua-5.4.4>cd src

E:\lua\lua-5.4.4\src>dir | find ".exe"
2022/11/18  00:25            55,822 lua.exe
2022/11/18  00:25           312,661 luac.exe

E:\lua\lua-5.4.4\src>
~~~

把 lua 添加进环境变量里去这种事情就不用说的了吧。

然后使用 lua -v 查看版本，如下：

~~~shell
C:\Users\98099>lua -v
Lua 5.4.4  Copyright (C) 1994-2022 Lua.org, PUC-Rio

C:\Users\98099>
~~~

2022.11.16