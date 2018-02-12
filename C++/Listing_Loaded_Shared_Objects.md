
# 动态库与iOS注入

## iOS注入原理

1) 静态注入

- 使用Tweak等方式建立要注入的动态库

- 替换动态库依赖：otool查看动态库依赖项，使用install_name_tool修改动态库的路径，替换为自己准备的库。

- 将动态链接库注入二进制文件中：
修改可执行文件的Load Commands，增加一个LC_LOAD_DYLIB，写入dylib路径
（先将库拷贝至相应路径，例如.app目录）。

- 重签名打包

完成后，在设备上运行app，会运行注入的库的初始化函数
（`__attribute__((constructor)) static void EntryPoint()`）。

2) 动态注入

基本思想与Win下的CreateRemoteThread类似：
远程根据pid获取进程结构，写入二进制代码执行`dlopen`加载相应的dylib。

## linux获取运行时的共享库加载

如果程序中使用了dlopen，则静态地使用ldd等方式无法获取所有的共享库信息。

程序运行时查看加载的动态库，除了使用lsof等工具，还可以通过自身代码。

linux中使用link_map结构描述加载库，可简单表示为:

```c
struct lmap
{
   void*    base_address;   /* Base address of the shared object */
   char*    path;           /* Absolute file name (path) of the shared object */
   void*    not_needed1;    /* Pointer to the dynamic section of the shared object */
   struct lmap *next, *prev;/* chain of loaded objects */
}
```

而dlopen函数返回的handle正是link_map结构指针，因此可以通过dlopen函数返回值遍历link_map列表。但如果不希望因此加载额外的库，需要调用`dlopen(NULL, RTLD_NOW)`，返回的结构并不直接指向link_map，会稍复杂。【见ref[1]】

------------------

<!--
IOS防篡改：

1. 通过dladdr函数返回结果验证动态库加载信息：
需要先获得函数地址；（类方法和实例方法不同？可能会不成功-显示包含在libobjc.A.dylib？）

2. 获取所有库的image_name，进行比对或筛查：需要预先知道会加载哪些库/不会加载哪些库。

3. 函数地址检查：更改后的代码地址可能发生变化。需要预先获得函数在lib中的偏移量（固定），并获得所在lib的加载地址（随机化）。

-->

## ref:

**[1] http://syprog.blogspot.ru/2011/12/listing-loaded-shared-objects-in-linux.html  Linux查看程序加载的动态库**

http://www.ttlsa.com/linux-command/linux-pmap-ldd/ linux下pmap和ldd查看进程调用的lib

https://zhuanlan.zhihu.com/p/21530388 入侵iOS应用

http://www.it72.com/thread-9145.htm iOS远程注入dylib

http://www.blogfshare.com/inject-with-njb.html iOS dylib注入

https://www.jianshu.com/p/b1822a77b827 iOS dylib注入

http://wiki.jikexueyuan.com/project/ios-security-defense/ iOS 安全攻防

<br/><br/>
