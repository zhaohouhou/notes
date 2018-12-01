# C++ 小白 practice

## 关于析构函数、虚函数

如果某个类可能被继承，那么析构函数应当定义为虚函数，否则释放时如果用基类指针则不能正确析构，可能造成内存泄漏。（详见析构函数调用顺序）

<font color=red>注意：</font> 

- 显示申明构造函数、析构函数一定要有定义，例如空函数体 (base类的虚函数也需要加函数体)。否则链接会报 undefined reference。

- 继承的时候，普通成员函数可以没定义，但是虚函数一定要有定义。（否则类是不能实例化的？）

例：

```c++
class Base
{
public:
    Base(){};
    void f1();//member function
    virtual void f2(){};//virtual function
    virtual void f3()=0;//pure virtual function
    virtual ~Base(){};//virtual destructor function
};
class Derive:public Base
{
public:
    Derive(){};
    void f1();
    void f2(){};
    void f3(){};
    virtual ~Derive(){};
};
```



