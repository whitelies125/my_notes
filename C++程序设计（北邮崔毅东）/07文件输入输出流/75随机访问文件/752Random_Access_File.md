# Random Access File

随机访问文件。

## Random Access

随机访问。

Random Access means one can read/write anywhere inside a file.

随机访问意味着可以读写文件的任意位置。

 

### How to do this?

We are able to know where the file positioner is.

我们要能够知道文件定位器在什么位置。

We are able to move the file positioner inside the file.

我们要能够在文件中移动文件定位器。

Maybe we need two file positioners : one for reading, another for writing.

也许，我们需要两个 fp，一个用来读，一个用来写。

### 相关函数

| ·                        | For reading(读文件时用)               | For writing(写文件时用)               |
| ------------------------ | ------------------------------------- | ------------------------------------- |
| 获知文件定位器指到哪里   | tellg(); tell是获知，g是get表示读文件 | tellp(); tell是获知，p是put表示写文件 |
| 移动文件定位器到指定位置 | seekg(); seek是寻找，g是get表示读文件 | seekp(); seek是寻找，p是put表示写文件 |

## seek 的用法

seek 的原型

~~~C++
xxx_stream& seekg/seekp( pos_type pos );//fp 移动到 pos 位置
xxx_stream& seekg/seekp( off_type off, std::ios_base::seekdir dir);//fp 移动到相对 dir 的 off 距离的位置。
~~~

seekdir 为文件定位方向类型，只有三种：

| **seekdir** 文件定位方向类型 | 解释                                  |
| ---------------------------- | ------------------------------------- |
| std::ios_base::beg           | 流的开始；beg = begin                 |
| std::ios_base::end           | 流的结尾                              |
| std::ios_base::cur           | 流位置指示器的当前位置；cur = current |

举例：

| 例子                        | 解释                                                 |
| --------------------------- | ---------------------------------------------------- |
| seekg(42L);                 | 将文件位置指示器移动到文件的第42字节处               |
| seekg(10L, std::ios::beg);  | 将文件位置指示器移动到从文件开头算起的第10字节处     |
| seekp(-20L, std::ios::end); | 将文件位置指示器移动到从文件末尾开始，倒数第20字节处 |
| seekp(-36L, std::ios::cur); | 将文件位置指示器移动到从当前位置开始，倒数第36字节处 |

## Example

~~~C++
#include <iostream>
#include <filesystem>
#include <fstream>

int main(){
	//创建路径
    std::filesystem::path p {"test.txt"};
	//以 写|读|舍弃原有内容 方式打开 test.txt
    std::fstream file {p,std::ios::out | std::ios::in | std::ios::trunc};
	//向 test.txt 输入两个 long long int 和 "Hello world"
    auto x{12LL};//long long int
    auto y{24LL};
    char str[] {"Hello,world"};
    file.write(reinterpret_cast<char*>(&x), sizeof(x));
    file.write(reinterpret_cast<char*>(&y), sizeof(y));
    file.write(str,sizeof(str));

    //随机读取 Hello 字符串
    char buf[100] {0};
    file.seekg(2*sizeof(long long int), std::ios::beg);
    file.read(buf, 5);
    std::cout << buf << std::endl;
    
    return 0;
}
//output:
//Hello
~~~

