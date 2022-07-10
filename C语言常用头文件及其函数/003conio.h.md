# conio.h

[TOC]

### kbhit(void)

~~~c
int kbhit(void)
~~~

如果有键盘输入，返回非 0；没有键盘输入，返回 0。

但并不会读走这个输入。

```c
//举例：每秒检测一次是否有键盘输入。有键盘输入则跳出循环；如果超过 15 分钟没有则弹出提示框。
int no_input_time = 0;
while(!kbhit()){
    Sleep(1000);
    no_input_time ++;
    if (no_input_time >= 900){
        MessageBox(0,"已有 15 分钟未进行选择","Notification",0);
    }
}
```

