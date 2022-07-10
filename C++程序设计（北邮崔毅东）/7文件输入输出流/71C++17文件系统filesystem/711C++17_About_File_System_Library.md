# C++17: About File System Library

C++17 的文件系统简介。

## About std::filesystem

std::filesystem 简介。

C++17 std::filesystem provides facilities for performing operations on file systems and their components, such as paths, regular files, and directories.

C++17 标准库的 filesystem 提供在文件系统与其组件，例如路径、常规文件与目录上进行操作的方法。

**File（文件）**：持有数据的文件系统对象，能被写入或读取。文件有名称和属性，属性之一是文件类型。

**Path（路径）**：标识文件所处位置的一系列元素，可能包含文件名。

## 路径类

~~~C++
namespace fs = std::filesystem;
fs::path p{"test.cpp"}
~~~

## Path

**Absolute Path(platform dependent)**: An absolute path contains a file name with its complete path and drive letter.

绝对路径：包含完整的路径和驱动器符号。

**Relative Path**:

相对路径

- Contains NO drive letter or leading "/"

  不包含驱动器及开头的 "/" 符号。

- The file stores in the path Relative to "Current Path"

  文件存在相对于 "当前路径" 的位置。

| OS Type                    | Absolute path         | Directory path |
| -------------------------- | --------------------- | -------------- |
| Windows(case insensitive)  | c:\example\scores.txt | c:\example     |
| Unix/Linux(case sensitive) | /home/cyd/scores.txt  | /home/cyd      |

## Differences between Windows and Linux

两种操作系统的不同。

|              | Windows               | Linux  | C++                                        |
| ------------ | --------------------- | ------ | ------------------------------------------ |
| 行结束符     | \r\n                  | \n     | -                                          |
| 路径名结束符 | \                     | /      | std::filesystem::path::preferred_separator |
| 路径名       | a:\b\c or \\\host\b\c | /a/b/c | std::filesystem::path                      |

为名字空间起一个别名：

~~~C++
namespace fs = std::filesystem;//注意这里 std::filesystem 是名字空间而不是类。
//如果写成 namespace fs = std::filesystem::path; 就错了，因为 std::filesystem::path 是一个类了，就不能用 namespace 了
~~~

举例：

~~~C++
namespace fs = std::filesystem;
fs::path p1("d:\\cpp\\hi.txt");//字符串中的反斜杠要被转义
fs::path p2("d:/cpp/hi.txt");//windows 也支持正斜杠
fs::path p3(R("d:\cpp\hi.txt"));//使用原始字符串字面量，如果路径带有特殊字符可以用这个方式
~~~

