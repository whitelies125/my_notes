# Formatting Output

格式化输出。

## setw manipulator

"设置域宽" 控制符。

头文件

~~~C++
#include <iomanip>

setw(n);//设置域宽，即数据所占总字符数。
~~~

setw() 只对其后输出的第一个数据有效，其它控制符则对其后的所有输入输出产生影响。

~~~C++
std::cout << std::setw(5) << 'a' << 'b' << std::endl;
//默认为右对齐
//output:
//    ab
~~~

setw() 默认为 setw(0) ，即按实际输出。

如果输出的数值占用 的宽度超过了 setw(int n) 设置的宽度，则按实际宽度输出。

~~~C++
float f = 0.12345;
std::cout << std::setw(3) << f << std::endl;
//output:
//0.12345
~~~

## setprecision manipulator

"设置浮点精度" 控制符。

~~~C++
setprecision(int n)
~~~

控制显示浮点数的有效位。

n 代表数字总位数。

~~~C++
#include <iostream>
#include <iomanip>

int main(){
    float f = 17/7.0;
    std::cout << f << std::endl;
    std::cout <<  std::setprecision(0) << f << std::endl;
	//setprecision(0) 取决于编译器，不同编译器实现不同。
    std::cout <<  std::setprecision(1) << f << std::endl;
    std::cout <<  std::setprecision(2) << f << std::endl;
    std::cout <<  std::setprecision(3) << f << std::endl;
    std::cout <<  std::setprecision(4) << f << std::endl;
    std::cout <<  f << std::endl;
    std::cout <<  std::setprecision(5) << f << std::endl;
    std::cout <<  std::setprecision(6) << f << std::endl;
    std::cout <<  std::setprecision(7) << f << std::endl;

    return 0;
}
//output:
//2.42857
//2
//2
//2.4
//2.43
//2.429
//2.429//说明设置一次后，对后面的都生效。
//2.4286
//2.42857
//2.428571
~~~

## setfill manipulator

"设置填充字符" 控制符。

~~~C++
setfill(ch);
~~~

设置填充字符，即 << 后面的数据长度小于域宽时，使用填充字符进行填充。

~~~C++
#include <iostream>
#include <iomanip>

int main(){
    std::cout << std::setfill('*')
            << std::setw(5)
            << 'a'
            << std::endl;
    return 0;
}
//output:
//****a
~~~

| 控制符                | 用途                                                         |
| --------------------- | ------------------------------------------------------------ |
| setw(width)           | 设置输出字段的宽度(仅对其后第一个输出有效)                   |
| setprecision(n)       | 设置浮点数的输/入出精度(总有效数字个数等于 n)                |
| fixed                 | 将浮点数以定点数形式输入/出(小数点后有效数字个数等于 setprecision 指定的n) |
| showpoint             | 将浮点数以带小数点和结尾0的形式输入/出，即便该浮点数没有小数部分。 |
| left                  | 输出内容左对齐。                                             |
| right                 | 输出内容右对齐。                                             |
| hexfloat/defaultfloat | C++11新增；前者以定点科学记数法的形式输出十六进制浮点数，后者还原默认浮点格式。 |
| get_money(money)      | C++11新增；从流中读取货币值。支持不同语言和地区的货币格式。  |
| put_money(money)      | C++11新增；将货币值输出到流。支持不同语言和地区的货币格式。  |
| get_time(tm,  format) | C++11新增；从流中读取日期时间值。                            |
| put_time(tm,format)   | C++11新增；将日期时间值输出到流。                            |

## Formatting Output in File Operation

在文件操作中的格式化输入输出。

The stream manipulator also works to format output to a file.

流控制符同样可以用于文件输入输出。

