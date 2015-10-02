#smart pointer

smart pointer (可翻译为智能指针)是C++中帮助实现对象管理的一种方式。

###1. 应用场景

C++没有提供自动的内存管理机制（例如java的垃圾回收机制），因而创建的对象需要手动销毁。当程序较为复杂时，会容易出现内存泄漏等问题（我想过早释放和重复释放也是容易出现的）。

考虑情况：当类中存在指针成员时，如何进行管理？一是采用值型的方式管理，每个类对象都保留一份指针指向的对象的拷贝；
另一种更优雅的方式是使用智能指针，从而实现指针指向的对象的共享。

###2. 从一个简单实现入手

http://blog.csdn.net/ruizeng88/article/details/6691191

考虑实现一个smart pointer需要实现哪些功能：

1. 能够自动释放所指向的对象。 
2. 能够像普通纸真一样使用。因此，需要重载*与->运算符。
3. smart pointer必须是高度类型化的，模板恰好提供了这个功能。
4. 其他：析构函数、拷贝构造函数和赋值运算符？

引用计数是一种实现自动释放的方式。

类的定义：

```cpp
template <typename T>  
class smart_ptr{  
public:       
    smart_ptr();  //default constructor 
    smart_ptr(T* p);  
    ~smart_ptr();  
    T& operator*();  
    T* operator->();  

    //add assignment operator and copy constructor  
    smart_ptr(const smart_ptr<T>& sp);  
    smart_ptr<T>& operator=(const smart_ptr<T>& sp);    
private:  
    T* ptr;  
    //add a pointer which points to our object's referenct counter  
    int* ref_cnt;   //是个指针，因为几个smart ptr对象需要共享该域
};  
``` 

对其进行使用的测试代码：（从中可以看出对于实现的一些要求）

```cpp
    #include "person.h"  
    #include "smart_ptr.h"  
    using namespace std;  
    int main(){  
        smart_ptr<person> p1;  
        smart_ptr<person> p2(new person("Cici"));
        //创建，reference+1
        p -> tell();  
        {  
            smart_ptr<person> q = p2;
            //由=操作符创建，reference+1

            q -> tell();  
            p1 = q;
            //=操作符，右值reference+1，而原左值reference应-1

            smart_ptr<person> s(p1);
            //由拷贝构造函数创建，reference+1

            s -> tell();  
            //C++对象在退出作用域时调用析构函数。
            //此时smart_ptr<person>的析构函数不能销毁person("Cici")， 
            //否则下面的r -> tell()会出错。
	}
        r -> tell();  
    }  
```

那么逐项来看实现的方式：

**1) 默认构造函数：**

```cpp
template <typename T>  
smart_ptr<T>::smart_ptr():ptr(0),ref_cnt(0){  
    //create a ref_cnt here though we don't have any object to point to  
    ref_cnt = new int(0);  
    (*ref_cnt)++;  
}  
``` 

**2) 带参构造函数：**

```cpp
template <typename T>  
smart_ptr<T>::smart_ptr(T* p):ptr(p){  
    //we create a reference counter in heap  
    ref_cnt = new int(0);  
    (*ref_cnt)++;  
}  
``` 

**3) 析构函数：**

```cpp
template <typename T>  
smart_ptr<T>::~smart_ptr(){  
    //delete only if our ref count is 0  
    if(--(*ref_cnt) == 0){  
        delete ref_cnt;  
        delete ptr;  
    }  
}  
``` 

**4) 伪装成指针：**

```cpp
template <typename T>  
T&  smart_ptr<T>::operator*(){  
    return *ptr;  
}  
  
template <typename T>  
T* smart_ptr<T>::operator->(){  
    return ptr;  
}  
``` 

**5) 拷贝构造函数：**

```cpp
template <typename T>  
smart_ptr<T>::smart_ptr(const smart_ptr<T>& sp):ptr(sp.ptr),ref_cnt(sp.ref_cnt){  
    (*ref_cnt)++;  
}  
``` 

**5) =操作符：**

```cpp
template <typename T>  
smart_ptr<T>& smart_ptr<T>::operator=(const smart_ptr<T>& sp){  
    if(&sp != this){  
        //we shouldn't forget to handle the ref_cnt our smart_ptr previously pointed to  
        if(--(*ref_cnt) == 0){  
            delete ref_cnt;  
            delete ptr;  
        }  
        //copy the ptr and ref_cnt and increment the ref_cnt  
        ptr = sp.ptr;  
        ref_cnt = sp. ref_cnt;  
        (*ref_cnt)++;  
    }  
    return *this;  
}  
``` 

其他要考虑的因素：线程安全等。

###3. auto_ptr

http://www.cnblogs.com/qytan36/archive/2010/06/28/1766555.html 

auto_ptr是C++标准库中(<utility>)为了解决资源泄漏的问题提供的一个智能指针类模板。
auto_ptr的几个要注意的地方：

1) Transfer of Ownership

auto_ptr与boost库中的share_ptr不同的，auto_ptr没有考虑引用计数，因此一个对象只能由一个auto_ptr所拥有，在给其他auto_ptr赋值的时候，会转移这种拥有关系。

2) 从上可知由于在赋值，参数传递的时候会转移所有权，因此不要轻易进行此类操作。

3) 使用auto_ptr作为成员变量，以避免资源泄漏。

   为了防止资源泄漏，我们通常在构造函数中申请，析构函数中释放，但是只有构造函数调用成功，析构函数才会被调用，换句话说，如果在构造函数中产生了异常，那么析构函数将不会调用，这样就会造成资源泄漏的隐患。

   比如，如果该类有2个成员变量，指向两个资源，在构造函数中申请资源A成功，但申请资源B失败，则构造函数失败，那么析构函数不会被调用，那么资源A则泄漏。

  为了解决这个问题，我们可以利用auto_ptr取代普通指针作为成员变量，这样首先调用成功的成员变量的构造函数肯定会调用其析构函数，那么就可以避免资源泄漏问题。

4，不要误用auto_ptr

  1）auto_ptr不能共享所有权，即不要让两个auto_ptr指向同一个对象。

  2）auto_ptr不能指向数组，因为auto_ptr在析构的时候只是调用delete,而数组应该要调用delete[]。

  3）auto_ptr只是一种简单的智能指针，如有特殊需求，需要使用其他智能指针，比如share_ptr。

  4）auto_ptr不能作为容器对象，STL容器中的元素经常要支持拷贝，赋值等操作，在这过程中auto_ptr会传递所有权，那么source与sink元素之间就不等价了。

### 一些知识点：

- C++，析构函数在下边3种情况时被调用：

1.对象生命周期结束，被销毁时；

2.delete指向对象的指针时，或delete指向对象的基类类型指针，而其基类虚构函数是虚函数时；

3.对象i是对象o的成员，o的析构函数被调用时，对象i的析构函数也被调用（自己实现）。

- Java是自动的内存管理机制，并不需要smart pointer。可能需要注意的是其拷贝方式，即注意深拷贝与浅拷贝的区别。若不对clone()方法进行改写，则调用此方法得到的对象即为浅拷贝。Java的常用拷贝方式：
<br></br>

<table border="1">
	<tr><td></td>
	<th>Operator= </th>
	<th>拷贝构造函数</th>
	<th>clone方法</th></tr>
<tr><th>预定义非集合类型(例如int)</th>
	<td>深拷贝</td>
	<td>如果支持拷贝构造函数的类型，则是深拷贝</td>
	<td>不支持</td></tr>
<tr><th>自定义类型</th>
	<td>浅拷贝</td>
	<td>取决于实现</td>
	<td>取决于实现</td></tr>
<tr><th>预定义集合类型</th>
	<td>浅拷贝</td>
	<td>会逐个调用每个元素的operator=方法</td>
<td>会逐个调用每个元素的operator=方法</td></tr>
</table>

可以改写从Object继承而来的clone方法，并使其访问权限为public，来实现深拷贝。

- 关于C++初始化：

初始化列表先于构造函数的函数体。另外，C++初始化类成员按照声明的顺序，而不是按照出现在初始化列表中的顺序。
