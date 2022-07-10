# SQLite

这是当时在学校用 R 语言时稍微了解了一点的。

#### 安装。

官网下载安装。

#### 创建.db文件

首先运行cmd，然后跳到想要创建数据库的目录，输入sqlite3 student.db，就会在当前目录创建student.db文件。

#### 创建表 table

~~~sqlite
Create table student(id integer, name varchar(20), score integer);
~~~

创建名为student的表，第1列为ID，整型；第2列名字，字符串型，长度最长不超过20；第3列分数，整型。

#### 向表中写入数据

~~~sqlite
Insert into student values (101,'zhangsan'.90);
~~~

向student表中写入数据。

#### 删除表

~~~sqlite
drop table student
~~~

删除student表。

#### 从表中选取数据并显示

~~~sqlite
select name, score from student
~~~

从student表中选择name，score。

#### 选择所有数据并显示

~~~sqlite
Select * from student
~~~

选择student中所有数据并显示

#### .databases

~~~sqlite
.darabases
~~~

显示打开的.db文件

#### .table

~~~sqlite
.tables
~~~

显示当前数据库中的表名。

#### .shell cd

~~~sqlite
.shell cd
~~~

显示sqlite3当前工作目录。

实际上.shell 后面加其他，意思是执行cmd里的指令，这里其实就是执行的cmd里的cd。

#### .cd E:/sqlite

~~~sqlite
.cd E:/sqlite
~~~

工作目录改为E:/sqlite

#### .show

~~~sqlite
.show
~~~

显示当前各种设置的值。

#### .separator “,”

~~~sqlite
.separator “,”
~~~

设置输出和.import使用的分隔符为，逗号。比如输入输出文件是.csv文件就设置成逗号。

#### import E:/sqlite/teacher.csv teacher

~~~sqlite
import E:/sqlite/teacher.csv teacher
~~~

将 E:/sqlite/teacher.csv 里的数据读入 tercher 表中。

#### 选择表输出到scv

~~~sqlite
.output test.csv //选择输出到的文件，这里是当前目录下的test.csv
select * from teacher//选择要输出的表。这里选择的是输出teacher表。
.output stdout//输出。
~~~

#### 运算符

自己要用的时候查表吧，比如逻辑运算符这些。

还有就是AND/OR运算符。

#### where语句

感觉就是 C 语言啊其他语言里的 if。

Select id,name from student where 这里写条件。

比如条件写id=1001之类的。

~~~sqlite
updata student set score=100 where id=1;
~~~

更新student表，score改为100，id=1这行的。

没有where就全改100了，人人都是100分。

~~~sqlite
delete from student where id=2;
~~~

删除student表，id=2这行。

#### .schema student

~~~sqlite
.schema student
~~~

显示student表的结构。就是我创建student表的时候输入的结构。

2019.09.02