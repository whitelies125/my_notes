# path class

路径类及操作。

## Members functions of path class

path 类的成员函数。

|        | 部分重要的成员函数                                 | 说明                                                         |
| ------ | -------------------------------------------------- | ------------------------------------------------------------ |
|        | +path(string)                                      | 构造函数                                                     |
|        | +assign(string): path&                             | 为路径对象赋值                                               |
| 连接   | +append(type p): path&                             | 将p追加到路径后。type是string、path或const char*。等价于  /= 运算符；自动添加目录分隔符 |
|        | +concat(type p): path&                             | 将p追加到路径后。type是string、path或const char*。等价于+=运算符；不自动添加目录分隔符 |
| 修改器 | +clear(): void                                     | 清空存储的路径名                                             |
|        | +remove_filename(): path&                          | 从给定的路径中移除文件名                                     |
|        | +replace_filename(const path&  replacement): path& | 以 replacement 替换文件名                                    |
| 分解   | +root_name(): path                                 | 返回通用格式路径的根名                                       |
|        | +root_directory(): path                            | 返回通用格式路径的根目录                                     |
|        | +root_path(): path                                 | 返回路径的根路径，等价于  root_name() / root_directory()，即“路径的根名 / 路径的根目录” |
|        | +relative_path(): path                             | 返回相对于 root-path 的路径                                  |
|        | +parent_path(): path                               | 返回到父目录的路径                                           |
|        | +filename(): path                                  | 返回路径中包含的文件名                                       |
|        | +stem(): path                                      | 返回路径中包含的文件名，不包括文件的扩展名                   |
|        | +extension(): path                                 | 返回路径中包含的文件名的扩展名                               |
| 查询   | +empty(): bool                                     | 检查路径是否为空                                             |
|        | +has_xxx(): bool                                   | 其中“xxx”是上面“分解”类别中的函数名。这些函数检查路径是否含有相应路径元素 |

## Non-members functions

非成员函数。

|          | **部分重要的非成员函数**                                     | **说明**                                                     |
| -------- | ------------------------------------------------------------ | ------------------------------------------------------------ |
|          | operator/( const path& lhs, const  path& rhs )               | 以偏好目录分隔符连接二个路径成分  lhs 和  rhs。比如  path p{"C:"}; p = p / "Users" /  "batman"; |
|          | operator <<, >> (path p)                                     | 进行路径 p 上的流输入或输出                                  |
| 文件类型 | s_regular_file( const path& p ): bool                        | 检查路径是否是常规文件                                       |
|          | is_directory( const path& p ): bool                          | 检查路径是否是目录                                           |
|          | is_empty( const path& p ): bool                              | 检查给定路径是否指代一个空文件或目录                         |
| 查询     | current_path(): pathcurrent_path( const path& p ): void      | 返回当前工作目录的绝对路径（类似linux指令 pwd）更改当前路径为p （类似linux指令 cd） |
|          | file_size( const path& p ): uintmax_t                        | 对于常规文件 p ，返回其大小；尝试确定目录(以及其他非常规文件)的大小的结果是由编译器决定的 |
|          | space(const path& p): space_info                             | 返回路径名 p 定位于其上的文件系统信息。space_info中有三个成员：capacity ——文件系统的总大小(字节)，free ——文件系统的空闲空间(字节)，available ——普通进程可用的空闲空间（小于或等于  free ） |
|          | status(const path& p): file_status                           | 返回 p 所标识的文件系统对象的类型与属性。返回的file_status是一个类，其中包含文件的类型(type)和权限(permissions) |
| 修改     | remove(const path& p): boolremove_all(const path& p): uintmax_t | 删除路径 p 所标识的文件或空目录递归删除 p 的内容（若它是目录）及其子目录的内容，然后删除  p 自身，返回被删文件及目录数量 |
|          | rename(const path& old_p,      const path& new_p): void      | 移动或重命名 old_p 所标识的文件系统对象到  new_p(类似linux指令mv) |
|          | copy( const path& from, const  path& to ): void              | 复制文件与目录。另外一个函数  bool copy_file(from, to) 拷贝单个文件 |
|          | create_directory( const path& p ):  boolcreate_directories( const path& p ):  bool | 创建目录 p （父目录必须已经存在）,若 p 已经存在，则函数无操作创建目录 p （父目录不一定存在）,若 p 已经存在，则函数无操作 |

## Example

这里的代码，在编译时 g++ -std=c++17 test.cpp -o test.exe 编译报错，不通过。

然后搜了一下，发现这是编译器的问题，我用 8.1.0 版本，而正好这个版本使用 filesystem，编译时会有问题。

我换成了 11.3.0 版本，就可以了。
而且换了之后，g++ test.cpp -o test.exe 就能通过编译，不用指定 -std=c++17 了。
测试了一下 C++17 才支持的结构化绑定：

~~~C++
int a[3]{1,2,3};
auto [a1,a2,a3]{a};
~~~

直接使用 g++ test.cpp -o test.exe 也没有报错。

说明 11.3.0 版本默认的 C++ 标准应该是高于等于 C++17 了。

顺带，这一节之后我默认的 g++ test.cpp -o test.exe，也就为 C++17 标准或以上了。

#### 例一 path 对象用法

在当前目录创建一个 test.txt 文件：

~~~C++
hello,world!
~~~

执行代码：

~~~C++
#include <iostream>
#include <string>
#include <filesystem>

int main(){
    
    namespace fs = std::filesystem;
    //Relative path
    fs::path p{"test.txt"};
    //Absolute path
    fs::path p1{"E:\\Visual_Studio_Code\\VSCode_mywork\\test\\test.txt"};
    //also Absolute path but "/"
    fs::path p2{"E:/Visual_Studio_Code/VSCode_mywork/test/test.txt"};
    //also Absolute path but Raw String literals
    fs::path p3{R"(E:\Visual_Studio_Code\VSCode_mywork\test\test.txt)"};

    //输出默认文件分隔符
    std::cout << "file separator is " << fs::path::preferred_separator << std::endl;
    //判断是否是常规文件，如果是，输出文件大小
    //判断是否是目录，如果是，列出其子目录
    if(fs::is_regular_file(p)){
        std::cout << p << "is a regular file, size: ";
        std::cout << fs::file_size(p) << std::endl;
    }
    else if(fs::is_directory(p)){
        std::cout << p << "is a directory, include: ";
        for(auto& e : fs::directory_iterator(p)){
            std::cout << e.path() << std::endl;
        }
    }
    //判断路径是否存在
    else if(fs::exists(p)) std::cout << p << "is a special file" << std::endl;
    else std::cout << p << "does not exist" << std::endl;
    return 0;
}
//output:
//file separator is 92// 92 也就是 ascii 码的 '\'
//"test.txt" is a regular file, size: 12
~~~

把 test.txt 文件删掉试试：

~~~C++
//output:
//file separator is 92
//"test.txt" does not exist
~~~

把

~~~C++
fs::path p{"test.txt"};
//改为
fs::path p{"E:/Visual_Studio_Code/VSCode_mywork/test"};
//改成一个路径的名字，而一个文件。

//output:
//file separator is 92
//"E:/Visual_Studio_Code/VSCode_mywork/test" is a directory, include: //"E:/Visual_Studio_Code/VSCode_mywork/test\\.vscode"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\Circle.cpp"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\Circle.h"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\Rectangle.cpp"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\Rectangle.h"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\Shape.h"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\test.cpp"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\test.exe"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\test.h"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\test.txt"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\test_1.cpp"
//"E:/Visual_Studio_Code/VSCode_mywork/test\\test_1.exe"
~~~

#### 例二 Path 类中用于分解路径成分的函数

~~~c++
#include <iostream>
#include <filesystem>

int main(){
    namespace fs = std::filesystem;
    fs::path p{"E:/Visual_Studio_Code/VSCode_mywork/test/test.cpp"};
    
    //判空
    if(p.empty()) std::cout << "Path " << p << " is empty." << std::endl;
    //判断路径是否存在
    if(!fs::exists(p)){
        std::cout << "Path" << p << " does not exist." << std::endl;
        std::exit(0);
    }
    //输出根名，根路径，相对路径
    std::cout << "root.name(): " << p.root_name() << "\n"
            << "root.path(): " << p.root_path() << "\n"
            << "relative path(): " << p.relative_path() << std::endl;
    //输出父路径，文件名，文件名主干，扩展名
    std::cout << "parent.name(): " << p.parent_path() << "\n"
            << "filename(): " << p.filename() << "\n"
            << "stem(): " << p.stem() << "\n"
            << "extension(): " << p.extension() << std::endl;
    return 0;
}
//output:
//root.name(): "E:"
//root.path(): "E:/"
//relative path(): "Visual_Studio_Code/VSCode_mywork/test/test.cpp"
//parent.name(): "E:/Visual_Studio_Code/VSCode_mywork/test"
//filename(): "test.cpp"
//stem(): "test"
//extension(): ".cpp"
~~~

#### 例三 展示 path 相关一些特殊运算符用法 /=,+=,/

~~~C++
#include <iostream>
#include <filesystem>

int main(){
    namespace fs = std::filesystem;
    //定义路径对象
    fs::path p1{"E:/Visual_Studio_Code"};
    fs::path p2{"E:/Visual_Studio_Code/"};
    
    //append 和 /= 会自动加上路径分隔符
    p1.append("VSCode_mywork");
    p1 /= R"(test)";
    std::cout << p1 << std::endl;
    p2.append("VSCode_mywork");
    p2 /= R"(test)";
    std::cout << p2 << std::endl;
    
    //concat 和 += 不会自动加上路径分隔符
    p1 = "E:/Visual_Studio_Code";
    p2 = "E:/Visual_Studio_Code/";
    
    p1.concat(R"(VSCode_mywork)");
    p1 += R"(test)";
    std::cout << p1 << std::endl;
    p2.concat(R"(VSCode_mywork)");
    p2 += R"(test)";
    std::cout << p2 << std::endl;
    
    //用 / 拼凑一个新路径
    fs::path p3{""};
    //这里不能去掉等号后的 p3 / ，而直接为 p3 = "E" /....
    //因为斜杠 / 要表示拼接路径的含义，必须要求前面的是 path 对象。
    p3 = p3 / "E:" / R"(Visual_Studio_Code)" / "VSCode_mywork" / "test" / "test.cpp";
    //写成下面这样是会报错的：
    //p3 = "E:" / "Visual_Studio_Code" / "VSCode_mywork" / "test" / "test.cpp";
    std::cout << p3 << std::endl;
    
    return 0;
}
//output:
//"E:/Visual_Studio_Code\\VSCode_mywork\\test"//自动补上路径分隔符
//"E:/Visual_Studio_Code/VSCode_mywork\\test"//自动补上路径分隔符，有了就不补
//"E:/Visual_Studio_CodeVSCode_myworktest"//单纯加一起
//"E:/Visual_Studio_Code/VSCode_myworktest"//单词加一起
//"E:Visual_Studio_Code\\VSCode_mywork\\test\\test.cpp"
~~~

#### 例四 获取磁盘空间信息

~~~C++
#include <iostream>
#include <filesystem>

int main(){
    namespace fs = std::filesystem;
    //定义路径对象
    fs::path p{"E:/"};
    
    //展示磁盘的总大小和剩余大小
    std::cout << "E: total space: " << fs::space(p).capacity << std::endl;
    std::cout << "E: free space: " << fs::space(p).free << std::endl;
    
    return 0;
}
//output:
//E: total space: 1000068870144//单位为字节
//E: free space: 358208004096//单位为字节
~~~

