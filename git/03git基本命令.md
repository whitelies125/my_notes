# git 基本命令

## git 工程区域

- 版本库（Respository）/ .git 目录（.git directory）

  实际上为在工作区中存在的 .git 文件夹，里面存放了 git 用来管理该仓库的所有版本数据，也可以叫作本地仓库。

- 暂存区（Stageing Area）/ 索引（Index）

  一般存放在 .git 文件家中的 index 文件中，也可以叫索引。

- 工作区（Working Directory）

  日常工作的代码文件或者文档所在的文件夹。

## git 版本控制下的文件状态

- 已提交（committed）

  该文件已经安全地保存在本地数据库中。

- 已暂存（staged）

  把已修改的文件放在下次提交时要保存的清单中，但还没有提交。

- 已修改（modified）

  修改了文件，但还没有暂存。

## 工程准备

### git init

用于在本地目录下新建 git 项目仓库。

~~~git
git init
~~~

执行该命令后，会在当前目录下自动生成一个 .git 文件夹。

如果带上参数 file_name：

~~~git
git init <file_name>
~~~

则会创建名为 file_name 的文件夹，及在其下的 .git 文件夹。

### git  clone

用于克隆远端服务器上的工程到本地磁盘。

~~~git
git clone <url>
~~~

若项目所在 git 服务器支持 git-lfs，且对二进制文件进行了区别管理，则务必使用：

~~~git
git lfs clone <url>
~~~

否则无法下载到工程中的二进制文件，工程内容不完整。

这一点就需要询问工程管理相关人员了。

## 新增/删除/移动文件到暂存区

### git add

在提交修改文件之前，需要将修改文件添加到暂存区。

~~~git
git add <modified_file_name>
~~~

- 如果文件为新建文件，尚未被 git 追踪（untracked），则必须先执行 git add <file_name>，将文件添加到暂存区后，才可再执行提交操作。
- 如果文件已被 git 追踪（tracked），即曾提交过。在早期版本的 git 仍然需要 git add <modified_file_name> 后才可提交。在较新版本已不用，可直接提交。

便捷操作：

~~~git
git add .
~~~

添加当前工作区所有文件到暂存区。

### git rm

用于将指定文件彻底从当前分支的暂存区删除，该文件在之后不再受到 git 的管理追踪。

~~~git
git rm <file_name>
~~~

执行该命令后，该文件会从暂存区删除。

也可在硬盘上删除该文件，然后对该文件进行 git commit 操作，git 会自动将删除的文件从暂存区中移除，效果相同。

### git mv

用于移动文件，也可用于重命名文件。

- 移动文件

  ~~~git
  git mv <file_name> <folder_name>
  ~~~

  将 <file_name> 文件移动到 <folder_name> 文件夹下。

- 重命名文件

  ~~~git
  git mv <old_file_name> <new_file_name>
  ~~~

  将 <old_file_name> 文件重命名为 <new_file_name> 文件。

## 查看工作区

### git diff

用于比较项目中两个版本（分支）之间的差异，也可以用来比较当前的索引和上次提交之间的差异。

**待后续完善**

### git status

用于显示工作目录和暂存区的状态。

~~~git
git status
~~~

使用此命令能够看到当前各种文件的状态：未被追踪的文件，已修改未暂存，已暂存未提交。

## 提交文件

### git commit

用于将暂存区里的文件改动提交到本地的版本库。

这个提交是本地操作，即是本地仓库 .git 文件夹中提交改动，不会影响远端服务器。

通常 git commit 往往需要附带提交描述信息，用以说明本次提交的相关信息。

~~~git
git commit <file_name> -m "<commit_message>"
~~~

将 <file_name> 文件提交到本地仓库，本次提交的描述信息为 <commite_message>。

若要一次性提交所有暂存区中的文件到版本库，则可使用：

~~~git
git commit -a -m "<commit_message>"
~~~

或

~~~git
git commit -am "<commit_message>"
~~~

如果想要修改最近一次提交到本地仓库的 <commit_message>，可使用：

~~~git
git commit --amend
~~~

将会进入一个 vim 编辑器界面，其中内容为最近一次提交的 <commit_message>，修改后保存退出就好。
vim 的保存退出是输入 :wq 按回车。

注意，修改后再次使用 git log 查看日志，发现最近一次的 commit id 发生了变化，所以实际上是新生成了一个节点来代替原本的节点。

## 查看日志

### git log

用于查看提交历史。

~~~git
git log
~~~

执行后会按提交时间由近到远列出历史提交日志。

每个日志包含了提交节点（commit id），作者信息（Author），提交时间（Date），提交说明（commit message）等内容。

git log 的可选参数非常多，功能也非常丰富。

~~~git
git log -<n>
~~~

只查看最近 \<n\> 次 commit 记录。

~~~git
git log --name-status
~~~

除了上述基本的 git log 信息外，还会显示每次变动的文件及其操作类型。

## 推送远端

### git push

在使用 git commit 将自己的修改从暂存区提交到本地仓库后，可以使用

~~~git
git push
~~~

将本地仓库的分支推送到远程服务器上对应的分支。

成功推送到远程仓库后，其它开发人员可以获取到新提交的内容。

常用的 git push 命令：

~~~git
git push <origin> <branch_name>
~~~

将本地 <branch_name> 分支推送到 \<origin\> 服务器。

~~~git
git push <origin> <local_branch_name>:<remote_branch_name>
~~~

将本地 <local_branch_name> 分支以 <remote_branch_name> 名推送到 \<origin\> 服务器。

## 分支管理

### 查看分支

#### git brach

用于查看所有 git 分支名称。

~~~git
git branch
~~~

查看本地所有分支，其中高亮，带星号 * 前缀的分支为当前工作分支。

~~~git
git branch-r
~~~

查看远端服务器上的所有分支。

~~~git
git branch -a
~~~

查看本地和远端服务器上的所有分支。

### 新建分支

git branch 和 git checkout -b 都可用于基于当前分支节点创建新分支。

#### git branch

~~~git
git branch <new_branch_name>
~~~

基于当前工作分支节点创建 <new_branch_name> 分支，但并不切换，工作分支不变。

#### git checkout -b

~~~git
git checkout -b <new_branch_name>
~~~

基于当前分支节点创建 <new_branch_name> 分支，并切换到新分支，工作分支变为新分支。

### 删除分支

#### git branch -d/-D

git branch -d 和 git branch -D 都可用于删除分支。

~~~git
git branch -d <branch_name>
~~~

删除 <branch_name> 分支。

~~~git
git branch -D <branch_name>
~~~

强制删除 <branch_name> 分支。

有时分支上包含路未合并的改动，或者要删除分支是当前工作分支，则 -d 无法删除，需要使用 -D 强制删除。

### 切换分支

#### git checkout 

用于切换工作分支。

~~~git
git checkout <branch_name>
~~~

切换到 <branch_name> 分支。

### 更新分支

#### git pull

~~~git
git pull <origin> <remote_branch>:<local_branch>
~~~

用于从远端服务器 \<origin\> 中获取某个分支 <remote_branch> 的更新，再与本地指定的分支 <local_branch> 进行自动合并。

如果远程分支与本地分支同名 ，则可以直接使用：

~~~git
git pull <origin> <remote_branch>
~~~

#### git fetch

用于从远端服务器中获取某个分支的更新到本地仓库，但不进行自动合并。
从而留给用户一个操作空间，确定 git  fetch 的内容符合预期后，再决定是否进行手动合并。

常用的命令：

~~~git
git fetch <origin> <remote_branch>:<local_branch>
~~~

同样的，如果远程分支与本地分支同名 ，则可以直接使用：

~~~git
git fetch <origin> <remote>:<local_branch>
~~~

### 分支合并

#### git merge

用于将指定分支合并到当前工作分支。

~~~git
git merge <branch_name>
~~~

将 <branch_name> 合并到当前工作分支。

~~~git
git merge <branch_name_1> <branch_name_2>
~~~

将 <branch_name_1> 合并到 <branch_name_2>。

#### git rebase

用于合并目标分支内容到当前分支。

~~~git
git rebase <branch_name>
~~~

将 <branch_name> 合并到当前工作分支。

## 撤销操作

### git reset

用于撤销当前工作区中的某些  git add，git commit 操作，可将工作区内容回退到某个历史提交节点。

~~~git
git reset <commit_id>
~~~

回退到指定的  <commit_id> 节点。

### git checkout

用于回退本地所有修改而未提交的内容。

~~~git
git checkout <file_name>
~~~

取消对 <file_name> 文件的未提交修改，回退到上一次暂存区中的文件内容。

~~~git
git checkout .
~~~

对所有文件回退，用暂存区的所有文件直接覆盖本地文件，从而实现取消对本地工作区的修改。