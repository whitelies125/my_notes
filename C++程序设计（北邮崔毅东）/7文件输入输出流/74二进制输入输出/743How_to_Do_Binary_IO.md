# How to Do Binary I/O

如何实现二进制读写。

## The write() Function

write() 函数。

prototype（函数原型）

~~~C++
ostream& write(const char* s, std::streamsize count)
~~~

 可直接将字符串写入文件

~~~C++
fstream fs("GreatWall.dat", ios::binary|ios::trunc);
char s[] = "ShanHaiGuan\nJuYongGuan";
fs.write(s, sizeof(s));
~~~

如何将非字符数据写入文件

- Convert any data into a sequence of bytes (byte stream) 

  先将数据转换为字节序列，即字节流。

- Write the sequence of bytes to file with write()

  再用 write() 函数将字节序列写入文件。

## How to convert any data into byte stream?

如何将信息转换为字节流

### reinterpret_cast 运算符

该运算符有两种用途：

1. cast the address of a type to another type.

   将一种类型的地址转为另一种类型的地址。

2. cast the address to a number, i.e. integer

   将地址转换为数值，比如转换为整数。

语法: 

~~~C++
reinterpret_cast*<dataType>(address) 
~~~

- address is the starting address of the data.

  address 是待转换的数据的起始地址。

- dataType is the data type you are converting to.

  dataType 是要转至的目标类型。

For binary I/O, dataType is char *.

对于二进制 I/O 来说，dataType 为 char*。

例如：

~~~C++
long int x {0};
int a[3] {21,42,63};
std::string str{"Hello"};
char* p1 = reinterpret_cast<char*>(&x);//variable address
char* p2 = reinterpret_cast<char*>(a);//array address
char* p3 = reinterpret_cast<char*>(&str);//object address
~~~

## 3. The read() Function

read() 成员函数。

prototype (函数原型)

~~~C++
istream& read ( char* s, std::streamsize count );
~~~

例子

读字符串：

~~~C++
fstream bio("GreatWall.dat", ios::in | ios::binary);
char s[10];
bio.read(s, 5);
s[5] = '\0';
cout << s;
bio.close();
~~~

读其它类型数据（整数），需要使用 reinterpret_cast：

~~~C++
fstream bio("temp.dat", ios::in | ios::binary);
int value;
bio.read(reinterpret_cast<char*>(&value), sizeof(value));
cout << value;
~~~

