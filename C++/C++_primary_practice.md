# C++ 小白 practice

## class & struct

In C++, a structure is the same as a class except for:

1. Members of a class are private by default and members of a struct are public by default.
2. When deriving a struct from a class/struct, default access-specifier for a base class/struct is public. And when deriving a class, default access specifier is private.

(所以strut不再是单纯的数据结构, 基本也和类一样, 有析构神码的)

## 关于构造函数(constructor)

### class member的初始化

Initialization of members in classes works identically to initialization of local variables in functions.

For **objects**, their default constructor is called. For example, for std::string, the default constructor sets it to an empty string. If the object's class does not have a default constructor, it will be a compile error if you do not explicitly initialize it.

For **primitive types** (pointers, ints, etc), they are not initialized -- they contain whatever arbitrary junk happened to be at that memory location previously.
(即, C++默认不会把类或结构体成员初始化为"0",如果需要默认值为0的指针或基本类型变量, 需要显式在类的定义或构造函数中写明.)

For **references** (e.g. std::string&), it is illegal not to initialize them, and your compiler will complain and refuse to compile such code. References must always be initialized.

In summary, 当没有显式地初始化时, 成员变量的值:

```c++
    int *ptr;  // Contains junk
    string name;  // Empty string
    string *pname;  // Contains junk
    string &rname;  // Compile error
    const string &crname;  // Compile error
    int age;  // Contains junk
```

## 关于析构函数(destructor)

- C++ 中, 当离开某个 object 的作用域的时候就会调用其析构函数.
- C++ 对象析构的时候, member class 对象(不是指针)也被析构
- C++ 的模板类(list, queue等)会自动管理添加进去的对象. 例如当队列中的对象被pop_front, 并离开作用域, 则其析构会被调用.

excample code:

```c++
#include <queue>
#include <iostream>

// 测试从容器中删除元素时,元素的字段是否能够正常释放
using namespace std;

class ClassElem
{
public:
    ClassElem();
    virtual ~ClassElem();
};
ClassElem::ClassElem()
{
}

ClassElem::~ClassElem()
{
    cout << "ClassElem destructor called!!" << endl;
}

typedef struct StructElem
{
    ClassElem classElem;
} StructElem;

void test1()
{
    // 测试queue中的元素和member能否被析构
    std::queue<T> *q = new std::queue<StructElem>(5);
    for (size_t i = 0; i < 5; i++)
    {
        StructElem elem;
        q->Push(elem);
    }
    delete q;
    // destructor 一共会调用 5 * 2 次
}

void test2()
{
    // 测试离开作用域的struct和class member能否被析构
    {
        StructElem elem;
    }
    // destructor 调用 1 次
}

int main()
{
    test1();
    cout << "test1 done!!\n";
    test2();
    cout << "test2 done!!\n";
}
```

## 关于虚函数

如果某个类可能被继承，那么析构函数应当定义为虚函数，否则释放时如果用基类指针则不能正确析构，可能造成内存泄漏。（详见析构函数调用顺序）

<font color=red>注意：</font> 

- 显式申明构造函数、析构函数一定要有定义，例如空函数体 (base类的虚函数也需要加函数体)。否则链接会报 undefined reference。

- 只有虚函数可以重载.

- 如果没有实现基类的纯虚函数, 子类会被认为是一个抽象类,不能实例化

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
## C/C++内存profile工具:Valgrind +Massif

https://www.valgrind.org/

https://www.valgrind.org/docs/manual/ms-manual.html#ms-manual.running-massif

可以运行时快照内存状态: 哪里申请了内存?占比多少? 从比例变化能看出来哪里可能有问题.

(但是由于Valgrind运行会比较慢, 和运行时的实际表现可能是不一样的,需要注意)

基本使用

- valgrind --tool=massif my_program   # 运行程序并使用 massif 工具分析内存使用. 运行停止后会生成一个massif.out.<pid>文件
- ms_print massif.out.12345  # pretty print 快照文件
- 默认列出call stack的阈值为内存占用百分之1%以上, 有时需要更细粒度的调用栈信息, 可以在 massif 和 ms_print 时使用参数:
    ```
    --threshold=<m.n> [default: 1.0]
    ```

- 如果要分析sub-processes的内存使用, 增加valgrind 参数 --trace-children=yes. (fork()的内存由于共享memory map,是一同监控的)(一般不需要..)

## 编译器 sanity check 工具

clang 3.1 和 gcc 4.8 之后内置了 sanity check 工具.

使用方式: 在编译和链接时添加 `-fsanitize=address` 选项, 运行时如果发现了内存使用的错误会打印到标准输出.

ref:

https://github.com/google/sanitizers/wiki/AddressSanitizer

https://wizardforcel.gitbooks.io/100-gcc-tips/content/address-sanitizer.html
