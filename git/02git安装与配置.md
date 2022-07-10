# git 安装与配置

## git 安装

git 官网：https://git-scm.com/

Downloads —— Windows —— 64-bit Git for Windows Setup.

下载好后安装运行，懒，一路点下一步 Next 就好。

安装完成后，在文件资源管理器中，鼠标右键菜单有 git bash here，点击打开，输入

~~~git
git --version
~~~

可查看 git 版本信息。

~~~cmd
git version 2.36.1.windows.1
~~~

能看到也就说明安装成功了。

## git 基本配置

### 配置文件

- 系统配置（对所有用户都适用）

  该配置文件为在 Git 安装目录下 ./etc/ 下的 gitconfig 文件。
  若使用 git config 时加上 --system 选项，读写的就是该文件。

- 用户配置（只适用于改用户）

  该配置文件为在用户目录下，即 C:/User/your_user_name 下的 .gitconfig 文件。
  若使用 git config 时加上 --global 选项，读写的就是该文件。

- 仓库配置（只对当前项目有效）

  该配置文件为在工作目录下，即工作目录下的 .git/ 下的 config 文件。
  若使用 git config 时加上 --local 选项，读写的就是该文件。

每级配置文件会覆盖上一级配置文件的对相同参数的设置。
（局部变量与全局变量的关系）

**后续皆以 github 为例**。

### 配置个人身份

通过鼠标右键菜单打开 git bash，其他方式打开也行。

#### 设置 user.name

在 git bash 中输入

~~~git
git config --global user.name "your_github_user_name"
~~~

设置用户名。也就是 github 的用户名。

#### 设置 user.email

输入

~~~git
git config --global user.email "your_github_register_email"
~~~

设置邮箱。也就是 github 的注册邮箱。

设置后，查看一下这两个变量，看看是否设置成功：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~
$ git config --global user.name
whitelies125

98099@LAPTOP-J528RFFT MINGW64 ~
$ git config --global user.email
lzb125125@gmail.com
~~~

### 连接到 github

#### 生成 ssh 密钥文件

输入：

~~~git
ssh-keygen -t rsa -C "你的GitHub注册邮箱"
~~~

然后对后续跳出来的 3 个选项，不输入任何东西，为空，直接回车。

第一个是

~~~git
Enter file in which to save the key (/c/Users/98099/.ssh/id_rsa):
~~~

创建保存 key 的文件，默认为保存在 /c/Users/98099/.ssh/id_rsa 文件中，直接回车。

后面两个是

~~~git
Enter passphrase (empty for no passphrase):
Enter same passphrase again:
~~~

也是直接回车，不输入东西。

然后就成功生成了 ssh 密钥文件。

#### github 公钥设置

找到 C:\Users\98099\.ssh\id_rsa.pub 文件，使用记事本打开，将其中内容复制。

在 github 中 settings —— SSH and GPG keys —— New SSH key。
然后出现的界面中 Title 一栏可以不写，或随便写。
将复制的内容粘贴到 Key 一栏中。

点击 Add SSH key，完成。

#### 查看是否连接成功

输入：

~~~git
ssh git@github.com
~~~

显示：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~
$ ssh  git@github.com
The authenticity of host 'github.com (20.205.243.166)' can't be established.
ED25519 key fingerprint is SHA256:+DiY3wvvV6TuJJhbpZisF/zLDA0zPMSvHdkr4UvCOqU.
This key is not known by any other names
Are you sure you want to continue connecting (yes/no/[fingerprint])?
~~~

输入 yes，显示：

~~~git
Warning: Permanently added 'github.com' (ED25519) to the list of known hosts.
PTY allocation request failed on channel 0
Hi whitelies125! You've successfully authenticated, but GitHub does not provide shell access.
Connection to github.com closed.
~~~

连接成功。

命令全部显示内容：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~
$ ssh  git@github.com
The authenticity of host 'github.com (20.205.243.166)' can't be established.
ED25519 key fingerprint is SHA256:+DiY3wvvV6TuJJhbpZisF/zLDA0zPMSvHdkr4UvCOqU.
This key is not known by any other names
Are you sure you want to continue connecting (yes/no/[fingerprint])? yes
Warning: Permanently added 'github.com' (ED25519) to the list of known hosts.
PTY allocation request failed on channel 0
Hi whitelies125! You've successfully authenticated, but GitHub does not provide shell access.
Connection to github.com closed.
~~~

### 查看配置信息

通过命令：

~~~git
git config --list
~~~

查看当前生效的配置信息。

~~~C++
98099@LAPTOP-J528RFFT MINGW64 ~
$ git config --list
diff.astextplain.textconv=astextplain
filter.lfs.clean=git-lfs clean -- %f
filter.lfs.smudge=git-lfs smudge -- %f
filter.lfs.process=git-lfs filter-process
filter.lfs.required=true
http.sslbackend=openssl
http.sslcainfo=E:/Git/Git/mingw64/ssl/certs/ca-bundle.crt
core.autocrlf=true
core.fscache=true
core.symlinks=false
pull.rebase=false
credential.helper=manager-core
credential.https://dev.azure.com.usehttppath=true
init.defaultbranch=master
user.name=whitelies125
user.email=lzb125125@gmail.com
~~~

可在此命令后跟 --system，--global，--local 查看系统配置文件，用户配置文件，仓库配置文件中设定的配置信息。

如

~~~git
git config --system --list
~~~

显示：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~
$ git config --system --list
diff.astextplain.textconv=astextplain
filter.lfs.clean=git-lfs clean -- %f
filter.lfs.smudge=git-lfs smudge -- %f
filter.lfs.process=git-lfs filter-process
filter.lfs.required=true
http.sslbackend=openssl
http.sslcainfo=E:/Git/Git/mingw64/ssl/certs/ca-bundle.crt
core.autocrlf=true
core.fscache=true
core.symlinks=false
pull.rebase=false
credential.helper=manager-core
credential.https://dev.azure.com.usehttppath=true
init.defaultbranch=master
~~~

可见，系统配置文件中的初始默认设定（我还没改过 --system 的设定，这当然就是初始默认设定啦）

显然，之前我们设置的 user.name，user.email，是在 --global 中设定的，所以这里是没有这两个参数的。

~~~git
git config --global --list
~~~

显示用户配置文件中的参数设定：

~~~git
98099@LAPTOP-J528RFFT MINGW64 ~
$ git config --global --list
user.name=whitelies125
user.email=lzb125125@gmail.com
~~~

### 配置文本换行符

Windows 换行符为 CRLF，而 Linux 换行符为 LF。

参数 core.autocrlf 表示是否在提交时自动将 CRLF 转换为 LF，在签出代码时自动将 LF 转换为 CRLF。
core.autocrlf 为 true 则自动转换，反之则不进行。

由前文可知，core.autocrlf 默认值为 true。

命令：

~~~git
git config --global core.autocrlf true
~~~

将 core.autocrlf 设置为 true。

如果在 Windows 上开发仅运行在 Windows 上的项目，则将其设置为 false，关闭此功能。

~~~git
git config --global core.autocrlf false
~~~

### 文件编码配置

在 Linux 及托管网站上默认的字符编码均是 utf-8，而 Windows 系统默认的编码不是采用 utf-8。
如果不设置 Git 字符编码为 utf-8，git 客户端在 Windows 下的中文内容可能会出现乱码。

在设置字符编码为 utf-8 后，可以解决中文显示的问题。

~~~git
git config --global gui.encoding utf-8
git config --global i18.commit.encodeing utf-8
git config --global i18n.logoutputencoding utf-8
git config --global core.quotepath false
export LESSCHARSET=utf-8
~~~

分别为：

设置 git 的图像界面，采用 utf-8 编码；

设置 git commite log 存储时，采用 utf-8 编码；

设置查看 git log时，采用 utf-8 编码。

设置 git 中显示中文路径时，可能以转义字符 \347\211 的形式显示，通过这条语句使其不转义而显示中文。

由于 git log 并不是将所有 log 一次性全部显示，而是恰当的停在第一页，按 space 键再往后翻页。这是通过 将 log 送给 less 处理实现的，所以设置 less 的字符编码为 utf-8，使得 git log 可以正常显示中文。
