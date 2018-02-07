# 关于objcRuntime

关于Objective-C的一些运行时工具函数的使用。

## dlfcn.h库

dlfcn.h库中包含下面一些有关动态加载的函数：

```c
void *dlopen(const char *filename, int flag); 
//load library and return the handle

char *dlerror(void); 
//returns describtion of the most recent error occurred from dlopen(), dlsym() or dlclose() 

void *dlsym(void *handle, const char *symbol);
//takes a "handle" returned by dlopen() and the null-terminated symbol name, 
//returns the address where that symbol is loaded into memory

int dlclose(void *handle);
//returns 0 on success, and nonzero on error.
```

### 关于初始化函数

- The obsolete(废弃的) symbols `_init()` and `_fini()`: 

  The linker recognizes special symbols `_init()` and `_fini()`. 
  If a dynamic library exports a routine named _init(), 
  then that code is executed after the loading, before dlopen() returns. 
  If the dynamic library exports a routine named _fini(), then it is called just before the library is unloaded. 
  【不推荐，可能不会执行】
  
- **`__attribute__((constructor))`** and **`__attribute__((destructor))`** function attributes:
  
  Constructor routines are executed before dlopen() returns, 
  and destructor routines are executed before dlclose() returns.

### Glibc扩展: dladdr() 和 dlvsym()
 
```c
int dladdr(void *addr, Dl_info *info);
//takes a function pointer and resolves name and file where it is located. 
//Information is stored in the Dl_info structure

void *dlvsym(void *handle, char *symbol, char *version);
//does the same as dlsym() but takes a version string as an additional argument.
```

因此已知函数地址时，可以通过dladdr查看函数来源于哪个库。Dl_info 的数据结构:

```c
typedef struct {
    const char *dli_fname;  /* Pathname of shared object that
                               contains address */
    void       *dli_fbase;  /* Address at which shared object
                               is loaded */
    const char *dli_sname;  /* Name of nearest symbol with address
                               lower than addr */
    void       *dli_saddr;  /* Exact address of symbol named
                               in dli_sname */
} Dl_info;
```

## objcRuntime

### Class、selector、IMP

- class: 对应类的概念

  `typedef struct objc_class *Class; `

- selector：对应方法选择器的概念（从方法名找到方法实现）。Objc同一个类中不会有命名相同的两个方法。

  `typedef struct objc_selector *SEL;`
  
- IMP：implementation，Objc方法代码块的地址，可像C函数一样直接调用。

因此，下面的函数可以用来获得某一函数的代码地址（先根据类名和方法名获取Class和SEL，略）：

```c
IMP class_getMethodImplementation(Class cls, SEL name);
//返回类cls的name方法的调用地址
```


## ref

https://linux.die.net/man/3/dlopen 

http://www.cnblogs.com/liuziyu/p/5733818.html 关于SEL、IMP、Method、isa、Class

<br/><br/>
