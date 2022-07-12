# File Open Mode

文件的打开模式。

## fstream and File Open Modes

fstream 与文件打开模式。

之前使用 ofstream 写数据，ifstream 读数据。

现在使用 fstreaa 读写数据。

When opening an fstream object, a "file open mode" should be specified.

创建 fstream 对象时，应该指定文件打开模式。

| Mode(模式)  | Description(描述)                                            |
| ----------- | ------------------------------------------------------------ |
| ios::in     | 打开文件读数据                                               |
| ios::out    | 打开文件写数据                                               |
| ios::app    | 把输出追加到文件末尾。app = append                           |
| ios::ate    | 打开文件，把文件光标移到末尾。ate = at end                   |
| ios::trunc  | 若文件存在则舍弃其内容。这是ios::out的默认行为。trunc = truncate |
| ios::binary | 打开文件以二进制模式读写                                     |

## Combining Modes

模式组合。

Open Mode 的定义：

又编译器自己实现。

- // std::ios_base::openmode 被ios继承
- typedef */\*implementation defined\*/* openmode;
- static constexpr openmode app = */\*implementation defined\*/*

Combine several modes:

组合多种模式：

- using the | operator (bitwise inclusive OR) 

  使用 "|" 运算符。

To open a file "name.txt" for appending data:

例如，打开文件 name.txt 追加数据：

~~~C++
stream.open("name.txt", ios::out | ios::app);
//out,以写文件的形式打开文件
//app,追加形式
~~~



