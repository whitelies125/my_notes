# string.h

[TOC]

### strlen()

~~~C
int strlen(const char *str)
~~~

返回字符串的长度。

下面是 stelen() 函数里的一部分

```c
for (char_ptr = str; ((unsigned long int) char_ptr
                      & (sizeof (longword) - 1)) != 0;
	++char_ptr)
	if (*char_ptr == '\0')//看这个判断条件
	return char_ptr - str;
```

显然，从 strlen() 中的这个判断条件可以看出，实际上返回的是第一个 '\0' 前的字符个数（不包含 '\0' ）。

顺带对比一下 sizeof()，sizeof() **不是函数**，是什么我现在的水平也不知道，反正不是函数。
sizeof() 返回的是类型或对象所占内存空间的大小，单位字节。
比如 sizeof(int)，返回 int 类型所占内存空间大小；sizeof(a)，返回对象 a 所占内存空间的大小。

So，如果是 char array[10]=“\0123456768\0”，那么，strlen() 返回 0，而 sizeof() 返回10。

### memset()

~~~C
void *memset(void *str, int c, size_t n)
~~~

复制字符 c（一个无符号字符）到参数 str 所指向的字符串的前 n 个字符。

例如，用于数组清零：

~~~c
#include <stdio.h>
#include <string.h>

#define maxn 1010

int a[maxn];

int main()
{

    memset( a, 0, sizeof(a) );
    ...
    return 0;
}
~~~

虽然也能用 for 循环来清零，不过 memset 就一句话更方便呗。

### strchr()

~~~c
char *strchr(const char *str, int c)
~~~

在参数 str 所指向的字符串中，搜索第一次出现字符 c（一个无符号字符）的位置。

返回在字符串 str 中第一次出现字符 c 的位置，如果未找到该字符则返回 NULL。

~~~c
#include <stdio.h>
#include <string.h>

int main ()
{
   char str[] = "http://www.baidu.com";
   char ch = '.';
   char * ret;
   ret = strchr(str, ch);
   printf("%c\n", *ret);
   printf("%s", ret);
   return 0;
}
//运行结果：
//.
//.baidu.com
~~~

