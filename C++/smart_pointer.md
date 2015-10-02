#smart pointer

smart pointer (可翻译为智能指针)是C++中帮助实现对象管理的一种方式。

###1. 应用场景

C++没有提供自动的内存管理机制（例如java的垃圾回收机制），因而创建的对象需要手动销毁。当程序较为复杂时，会容易出现内存泄漏等问题（我想过早释放和重复释放也是容易出现的）。

考虑情况：当类中存在指针成员时，如何进行管理？一是采用值型的方式管理，每个类对象都保留一份指针指向的对象的拷贝；
另一种更优雅的方式是使用智能指针，从而实现指针指向的对象的共享。

###2. 从一个简单实现入手

考虑实现一个smart pointer需要实现哪些功能：

1. 能够自动释放所指向的对象。 
2. 能够像普通纸真一样使用。因此，需要重载*与->运算符。
3. smart pointer必须是高度类型化的，模板恰好提供了这个功能。
4. 
其他：析构函数、拷贝构造函数和赋值运算符？

类的定义：

```java
template <typename T>  
class smart_ptr{  
public:  
    //add a default constructor  
    smart_ptr();  
    //  
    smart_ptr(T* p);  
    ~smart_ptr();  
    T& operator*();  
    T* operator->();  
    //add assignment operator and copy constructor  
    smart_ptr(const smart_ptr<T>& sp);  
    smart_ptr<T>& operator=(const smart_ptr<T>& sp);  
    //  
private:  
    T* ptr;  
    //add a pointer which points to our object's referenct counter  
    int* ref_cnt;  
    //  
};  
``` 

为了实现自动释放，引用计数是一种方式。

###3. auto_ptr
