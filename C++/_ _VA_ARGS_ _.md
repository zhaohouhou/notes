ref:
http://www.cnblogs.com/zhujudah/archive/2012/03/22/2411240.html

## 1. `#`

假如希望在字符串中包含宏参数，ANSI C允许这样作，在类函数宏的替换部分，#符号用作一个预处理运算符，它可以把语言符号转化程字符串。例如，如果x是一个宏参量，那么#x可以把参数名转化成相应的字符串。该过程称为字符串化（stringizing）.

```
#incldue <stdio.h>
#define PSQR(x) printf("the square of" #x "is %d.\n",(x)*(x))
int main(void)
{
    int y =4;
    PSQR(y);
    PSQR(2+4);
    return 0;
}
```
输出结果：
```
the square of y is 16.
the square of 2+4 is 36.
```
第一次调用宏时使用“y”代替#x；第二次调用时用“2+4"代#x。

## 2.`##`

\##运算符可以用于类函数宏的替换部分。另外，##还可以用于类对象宏的替换部分。这个运算符把两个语言符号组合成单个语言符号。例如：

```
#define XNAME(n) x##n
```
这样宏调用：
```XNAME(4)```
展开后：
```x4```
程序：

```
#include <stdio.h>
#define XNAME(n) x##n
#define PXN(n) printf("x"#n" = %d\n",x##n)
int main(void)
{
    int XNAME(1)=12;//int x1=12;
    PXN(1);//printf("x1 = %d\n", x1);
    return 0;
}
```

输出结果：
x1=12


## 3.可变参数宏 ...和_ _VA_ARGS_ _
__VA_ARGS__ 是一个可变参数的宏，很少人知道这个宏，这个可变参数的宏是新的C99规范中新增的，目前似乎只有gcc支持（VC6.0的编译器不支持）。
实现思想就是宏定义中参数列表的最后一个参数为省略号（也就是三个点）。这样预定义宏_ _VA_ARGS_ _就可以被用在替换部分中，替换省略号所代表的字符串。比如：

```
#define PR(...) printf(__VA_ARGS__)
int main()
{
    int wt=1,sp=2;
    PR("hello\n");
    PR("weight = %d, shipping = %d",wt,sp);
    return 0;
}
```

输出结果：
```
hello
weight = 1, shipping = 2
```

省略号只能代替最后面的宏参数。```#define W(x,...,y)```错误！
