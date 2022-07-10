# Write Data to a File

向文件写入数据。

ofstream class can be used to write data to a text file.

ofstream 类用于向文本文件写入数据。

What if the file exists? the contents of the file will be destroyed without warning.

如果文件已经存在，文件内容将会在没有警告的情况下被清除。

<img src="722-1.png" alt="722-1" style="zoom: 67%;" />

<center>图1.输出数据流程图</center>



## Auto type recognition

自动类型识别。

<img src="722-2.png" alt="722-2" style="zoom:80%;" />

<center>图2.自动类型识别</center>

如图 2 所示，在输出时

~~~C++
output << "LiLei" << " " << 90.5 << endl;
output << "HanMeimei" << " " << 85 << endl;
~~~

其中 90.5 为 double 类型，85 为 int 类型，但在输出文件中可见，输出时自动转换为了 4 个 ASCII 码 90.5，和 2 个 ASCII 码 85 的形式来输出。

### Example

~~~C++
#include <iostream>
#include <filesystem>
#include <fstream>

int main(){
    namespace fs = std::filesystem;
    fs::path p {"test.txt"};//创建路径
    std::ofstream output {p};//以路径为参数，创建文件输出流//{直接写路径也行}
    double LiLei_score {90.5};
    int HanMeimei_score {80};
    output << "LiLei" << " " << LiLei_score << std::endl;
    output << "HanMeimei" << " " << HanMeimei_score << std::endl;
    output.close();//关闭文件输出流。
    return 0;
}
~~~

运行后：

~~~C++
//test.txt
LiLei 90.5
HanMeimei 80

~~~

