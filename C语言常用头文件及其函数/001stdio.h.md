# stdio.h

[TOC]

### scanf()

~~~c
int scanf(const char *format, ...)
~~~

从标准输入 stdin 读取格式化输入。

常用的循环输入：

~~~c
#include <stdio.h>

int main()
{
    int x = 0;
    while( scanf("%d",&x) == 1){
        ...
    }
    return 0;
}
~~~

因为 scanf() 的返回值，是成功输入的变量个数。当输入结束时，则返回 0。

例如：

~~~c
#include <stdio.h>

int main()
{
    int x = 0;
    int y = 0;
    printf("%d\n", scanf("%d",&x) );
    printf("%d", scanf("%d %d",&x,&y) );
    return 0;
}
//输入内容：
//1 2 3
//运行结果：
//1
//2
~~~

### freopen()

~~~C
FILE *freopen(const char *filename, const char *mode, FILE *stream)

filename:要打开的文件名称。
mode:文件访问模式，r，w，a，r+，w+，a+
~~~

把一个新的文件名 filename 与给定的打开的流 stream 关联，同时关闭流中的旧文件。

例如：

~~~c
#include <stdio.h>
int main ()
{
    FILE *fp;
    printf("该文本重定向到 stdout\n");
    fp = freopen("file.txt", "w+", stdout);
    printf("该文本重定向到 file.txt\n");
    fclose(fp);
    return 0;
}
//输出结果：
//控制台中显示：
//    该文本重定向到 stdout
//文件 file.txt 有内容：
//    该文本重定向到 file.txt
~~~

例如：

~~~c
#include <stdio.h>
int main ()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
    //这样，在之后的 scanf(), 就会是从文件 input.txt 读入，printf() 向 output.txt 写入
    //不仅 scanf()、printf()，之后的所有读键盘输入、写屏幕输出的函数都将该为这俩文件
    //就是 freopen 将 stdin 关联到了 input.txt 并关闭了 stdin 了嘛。
    ...
    return 0;
}
~~~

### fscanf()

~~~c
int fscanf(FILE *stream, const char *format, ...)
~~~

从流 stream 读取格式化输入。

例如：

~~~c
#include <stdio.h>

int main()
{
    FILE * fin = fopen("data.in","rb");
    FILE * fout = fopen("data.out","wb");
    while( fscanf( fin, "%d", &x) == 1){
        ...
    }
	fprintf( fout, "%d", x);
    fclose(fin);
    fclose(fout);
}
~~~

### sprintf()

```c
int sprintf(char *str, const char *format, ...)
```

发送格式化输出到 str 所指向的字符串。

如果成功，则返回写入的字符总数，不包括字符串追加在字符串末尾的空字符。如果失败，则返回一个负数。

例如：

~~~c
#include <stdio.h>

int main()
{
	char str[80];
    char a = 'a';
	int b = 123;
    char c = 'c';
    sprintf(str, "%c%d%c", a,b,c);
    puts(str);
}
//运行结果：
//a123c
~~~

### fgetc()

~~~c
int fgetc(FILE *stream)
~~~

从指定的流 stream 获取下一个字符（一个无符号字符），并把位置标识符往前移动。

该函数把读取的字符 char 强制转换为 int 的形式返回，如果到达文件末尾或发生读错误，则返回 EOF。

注意返回值的数据类型是 int。
<算法竞赛入门经典（第 2 版）> 解释为：

> 如果把 fgetc() 的返回值强制转换为 char，将无法把特殊的 EOF 和普通字符区分开。
>
> 提示：
> 一般情况下应当在检查它不是 EOF 后再将其转换为 char 值。
> 从标准输入读取一个字符可以用 getchar()，它等价于 fgetc(stdin)。

### getchar()

```
int getchar(void)
```

从标准输入 stdin 获取一个字符（一个无符号字符）。这等同于 getc 带有 stdin 作为参数。

该函数把读取的字符 char 强制转换为 int 的形式返回，如果到达文件末尾或发生读错误，则返回 EOF。

### fgets()

~~~c
char *fgets(char *str, int n, FILE *stream)
~~~

从指定的流 stream 读取一行，并把它存储在 str 所指向的字符串内。
当读取 n-1 个字符时，或者读取到换行符时，或者到达文件末尾时，它会停止，具体视情况而定。

如果成功，该函数返回相同的 str 参数。
如果到达文件末尾或者没有读取到任何字符，str 的内容保持不变，并返回一个空指针。
如果发生错误，返回一个空指针。

~~~c
#include <stdio.h>

int main()
{
	char str[60];
	fgets (str, 60, stdin);
    puts(str);
    fclose(fp);
    return 0;
}
//运行结果：
//123abc123abc
//123abc123abc
~~~
