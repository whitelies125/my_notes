# 362 Overloading Functions

## 一. overloading functions 重载函数

重载函数是在同一个名字空间中存在两个或者多个具有相同名字的函数所构成的语法现象。

调用重载函数的语句，是由编译器在编译期确定的。

编译器判断某个函数调用语句所对应的重载函数时，判断依据是函数参数的**类型、个数和次序**，注意是不看函数返回值的。

如果编译器无法判定，就会报告二义性错误。

~~~C++
int max(int num1, int num2){
    if( num1 > num2 )
        return num1;
    else
        return num2;
}

double max(double num1, double num2){//同名，但参数不同
    if( num1 > num2 )
        return num1;
    else
        return num2;
}

int main(){
    max(1, 2);//会依据函数参数的类型、个数和次序，调用 int max(int num1, int num2)
    max(1.1, 1.2)//调用的会是 double max(double num1, double num2)

}
~~~

## Ambiguous Invocation 二义调用

Ambiguous Invocation: There may be two or more possible matches for an invocation of a function, but the compiler cannot determine the most specific match.

二义调用：某函数调用有多个匹配项，但编译器无法确定正确的项。

Ambiguous invocation is a compilation error.

二义调用会导致编译错误。

~~~C++
int maxNumber(int num1, double num2){
    
}

double maxNumber(double num1, int num2){
    
}

int main(){
    maxNumber(1,2);//无法判断，会出错。
    //在 C 语言中，如果只存在 int maxNumber(int num1, double num2)，则会隐式地将 2 转换为 double
    //但现在编译器无法确定是调用 int maxNumber(int num1, double num2)，转换 2 为 double
    //还是调用 double maxNumber(double num1, int num2)，将 1 隐式地转换为 double
}
~~~

