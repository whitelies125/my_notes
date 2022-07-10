# 与 github 相关操作

## 建立远程仓库 

在 github 上建立一个仓库，我勾选了创建仓库时创建 README.md 文件。
创建后，在该仓库中 Code 下拉菜单中，获得该仓库的 SSH 的 url：

我创建了一个名为 notes 的仓库，其 SSH 的 url 为：

~~~
git@github.com:whitelies125/notes.git
~~~

## 在本地工作目录创建本地仓库

我在 test 文件夹中放了 3 个文件用以测试。

~~~cmd
C:\Users\98099\Desktop\test>dir
 驱动器 C 中的卷是 Windows
 卷的序列号是 1481-AFE4

 C:\Users\98099\Desktop\test 的目录

2022/06/26  22:41    <DIR>          .
2022/06/26  22:41    <DIR>          ..
2022/06/26  11:17                93 01版本控制工具简介.md
2022/06/26  17:17             8,098 02git安装与配置.md
2022/06/26  17:41             1,273 03git基本命令.md
               3 个文件          9,464 字节
               2 个目录 14,928,691,200 可用字节

~~~

打开 git bash，执行

~~~git
git init
~~~

创建本地仓库。

## 添加远程仓库变量

执行：

~~~git
git remote add <origin> <url>
~~~

\<origin\>  名往往都会设置为 origin，\<url\> 即是 github 上 note 仓库的 SSH  的 url。

即：

~~~git
git remote add origin git@github.com:whitelies125/notes.git
~~~

创建了一个变量，名为 origin，内容为 git@github.com:whitelies125/notes.git。

使用：

~~~git
git remote
~~~

查看已指定的远端服务器的简写（变量名）。

~~~git
$ git remote -v
~~~

查看需要读写远程仓库使用的 git 保存的简写（变量名）与其对应的 URL。

~~~C++
origin  git@github.com:whitelies125/notes.git (fetch)
origin  git@github.com:whitelies125/notes.git (push)
~~~

这两个操作在 git bash 中显示为：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~/Desktop/test (master)
$ git remote
origin

98099@LAPTOP-J528RFFT MINGW64 ~/Desktop/test (master)
$ git remote -v
origin  git@github.com:whitelies125/notes.git (fetch)
origin  git@github.com:whitelies125/notes.git (push)
~~~

## 提交到本地仓库

将本地工作目录文加提交到本地仓库。

~~~git
git add .
git commit -am "firts add files"
~~~

## 将远程仓库内容更新到本地仓库

由于我在 github 创建 notes 仓库时，勾选了同时创建 README.md 文件，所以远程仓库一开始就存在 README.md，即本地仓库并非远程仓库的最新内容。

同时，由于本地仓库与远程仓库此时毫无联系，直接使用：

~~~git
git pull origin master
~~~

会被拒绝执行：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~/Desktop/test (master)
$ git pull origin master
From github.com:whitelies125/notes
 * branch            master     -> FETCH_HEAD
fatal: refusing to merge unrelated histories
~~~

所以，使用：

~~~git
git pull origin master --allow-unrelated-histories
~~~

参数表示允许两个不同项目的无关联提交进行拉取合并。

## 将本地仓库推送到远程仓库

然后执行：

~~~git
git push origin master
~~~

进入 github 网页查看，提交成功。