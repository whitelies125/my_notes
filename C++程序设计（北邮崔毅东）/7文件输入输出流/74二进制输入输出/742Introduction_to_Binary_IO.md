# Introduction to Binary IO

二进制输入输出简介。

## Text File vs Binary File

文本文件与二进制文件。

- Both stroes as a sequence of bits(in binary format)

  二者都按二进制格式存储比特

- test file: interpreted as a sequence of characters.

  文本文件：解释为一系列字符。

- binary file: interpreted as a sequence of bits.

  二进制文件：解释为一系列比特。

例如，对十进制整数 199。

文本文件将其存储为 '1', '9', '9' 三个 ASCII 码字符。

二进制文件将其存储为 11000111。

## Text I/O vs Binary I/O

文本读写与二进制读写。

文本读写：windows 文件的换行（CRLF）vs *nix 文件的换行（LF）

- 在 windows 上，'\n' 输出到文件中会自动编码为 '\r' '\n' 两个字符。
- 在 *nix 上，'\n' 字符输出到文件中不变。

Text I/O is built upon binary I/O to provide a level of abstraction for character encoding and decoding.

文本模式的读写是建立在二进制模式读写的基础上的，只不过是将二进制信息进行了字符解码。

## File Open Mode: ios::binary

文件打开模式：ios::binary

Binary I/O does not require conversions.

二进制读写无需信息转换。

How to perform binary I/O?

如何进行二进制读写。

- By default, a file is opened in text mode.

  文件默认以文本模式打开。

- open a file using the binary mode ios::binary.

  用 ios::binary 以二进制模式打开文件。

| Text  I/O (文本模式) | Binary  I/O function:(**二进制模式) |          |
| -------------------- | ----------------------------------- | -------- |
| 读                   | operator  >>; get(); getline();     | read();  |
| 写                   | operator  <<; put();                | write(); |