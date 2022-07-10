# Function for I/O Stream

## getline()

When using (>>), data are delimited by whitespace. 

运算符 >> ，使用用空格分隔数据。

对于文件内容：

~~~C++
Li Lei#Han Meimei#Adam
~~~

如下代码只能读入 "Li"

~~~C++
ifstream input("name.txt");
std::string name;
input >> name;//L
~~~

Read in "Li Lei" with member function :

~~~C++
getline(char* buf, int size, char delimiter)
~~~

使用成员函数 getline() 读入 "Li Lei"：

~~~C++
constexpr int SIZE{ 40 };
std::array<char, SIZE> name{};
while (!input.eof()) {// not end of file
	input.getline(&name[ 0 ] , SIZE , '#');
	std::cout << &name[ 0 ] << std::endl;//Li Lei
}
~~~

Read in "Li Lei" with non-member function:

~~~C++
std::getline(istream& is, string& str, char delimiter)
~~~

使用非成员函数 getline() 读入 "Li Lei"：

~~~C++
std::string name2{};
while (!input.eof()) {
	std::getline(input, name2, '#');
	std::cout << name2 << std::endl;//Li Lei
}
~~~

## get() and put() 

get(): read a character.

读一个字符

~~~C++
int istream::get();//无参，返回的是 int 类型，往往需要对其转换为 char
istream& get (char& c);
~~~

put(): write a character.

输出一个字符

~~~C++
ostream& put (char c);
~~~

## flush()

Flush output stream buffer.

将输出流缓存中的数据写入目标文件。

~~~C++
ostream& flush();
~~~

用法

~~~C++
cout.flush(); //其它输出流对象也可以调用 flush()
cout << "Hello" << std::flush; // 与 endl 类似作为 manipulator 的调用方式
~~~